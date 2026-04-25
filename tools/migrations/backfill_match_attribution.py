#!/usr/bin/env python3
"""One-shot migration: populate `matched_by_backend`,
`matched_by_session_id`, `matched_by_model`, and `matched_at` on every
already-matched DB entry.

Sources, in priority order:

  1. `git log` for commits with subject `Match N functions (session X)`.
     The body lines (`  - Class::Method`) link short function names back
     to DB rows. Committer date supplies `matched_at`.
  2. `logs/match_*.jsonl` `session_start` events supply
     `(backend, model)` for each known session_id.
  3. Date-based fallback (per docs/direction/003-multi-agent-ab-architecture.md):
     - date <  2026-04-20 (41a4601): claude / claude-opus-4-6
     - 2026-04-20 ≤ date < 2026-04-23 (777bf8e): claude / claude-opus-4-7
     - date ≥ 2026-04-23: claude / claude-opus-4-7 (codex is the minority;
       safer to defer to the logs map than to flip on date alone)

Idempotent: entries that already carry `matched_by_session_id` are
skipped on a re-run. Run twice → second run is a no-op.

Usage:
    python3 tools/migrations/backfill_match_attribution.py --dry-run
    python3 tools/migrations/backfill_match_attribution.py          # apply
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

_REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(_REPO_ROOT / "tools"))

from common import load_db, save_db


_MATCH_SUBJECT_RE = re.compile(
    r"^Match\s+\d+\s+functions\s+\(session\s+([0-9a-f]{6,})\)\s*$",
    re.MULTILINE,
)
_BODY_NAME_RE = re.compile(r"^\s*-\s+(\S.*?)\s*$", re.MULTILINE)

# Cutoffs from docs/direction/003-multi-agent-ab-architecture.md §3.
_OPUS_47_PIN = datetime(2026, 4, 20, tzinfo=timezone.utc)
_CODEX_LANDED = datetime(2026, 4, 23, tzinfo=timezone.utc)


def walk_match_commits():
    """Yield (session_id, commit_date_iso, [short_names]) per match commit.

    Newest-first: that's the order `git log` returns. Callers that
    want first-match-wins (i.e., the most recent commit attributing a
    given function) should iterate in this order without reversing.
    """
    out = subprocess.check_output(
        ["git", "-C", str(_REPO_ROOT), "log", "--all",
         "--format=%H%x1f%cI%x1f%B%x1e"],
        text=True,
    )
    for record in out.split("\x1e"):
        if not record.strip():
            continue
        parts = record.strip().split("\x1f")
        if len(parts) < 3:
            continue
        _sha, iso_date, body = parts[0], parts[1], parts[2]
        m = _MATCH_SUBJECT_RE.search(body)
        if not m:
            continue
        session_id = m.group(1)
        names = _BODY_NAME_RE.findall(body)
        if not names:
            continue
        yield session_id, iso_date, names


def build_session_backend_map():
    """Walk logs/match_*.jsonl for session_start events; build
    {session_id: (backend, model)}. Sparse for pre-codex sessions
    where the orchestrator hadn't yet recorded backend/model.

    Skipped log files and corrupt lines are reported on stderr — a
    silent skip would invisibly raise the date-fallback path's hit
    rate and degrade attribution accuracy.
    """
    log_dir = _REPO_ROOT / "logs"
    mapping: dict[str, tuple[str, str]] = {}
    skipped_files = 0
    corrupt_lines = 0
    if not log_dir.exists():
        return mapping
    for log_path in sorted(log_dir.glob("match_*.jsonl")):
        try:
            with open(log_path) as f:
                for lineno, line in enumerate(f, 1):
                    line = line.strip()
                    if not line:
                        continue
                    try:
                        ev = json.loads(line)
                    except json.JSONDecodeError as e:
                        corrupt_lines += 1
                        print(f"warn: {log_path.name}:{lineno}: corrupt JSON ({e})",
                              file=sys.stderr)
                        continue
                    if ev.get("event") != "session_start":
                        continue
                    sid = ev.get("session_id")
                    backend = ev.get("backend")
                    model = ev.get("model")
                    if not sid or not backend:
                        continue
                    mapping.setdefault(sid, (backend, model or ""))
        except OSError as e:
            skipped_files += 1
            print(f"warn: {log_path.name}: cannot open ({e})", file=sys.stderr)
    if skipped_files or corrupt_lines:
        print(f"warn: log scan summary — {skipped_files} file(s) skipped, "
              f"{corrupt_lines} corrupt line(s) skipped",
              file=sys.stderr)
    return mapping


def fallback_backend_model(commit_date: datetime) -> tuple[str, str]:
    if commit_date < _OPUS_47_PIN:
        return ("claude", "claude-opus-4-6")
    return ("claude", "claude-opus-4-7")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true",
                    help="Report what would change; don't write the DB.")
    args = ap.parse_args()

    db = load_db()
    matched = [f for f in db if f.get("match_status") == "matched"]

    # Index matched entries by short name. A short name can map to
    # multiple addresses (overloads, multi-target functions); we resolve
    # by walking commits newest-first and claiming each entry once.
    by_short_name: dict[str, list[dict]] = {}
    for f in matched:
        short = f["name"].split("(", 1)[0]
        by_short_name.setdefault(short, []).append(f)

    session_backend_map = build_session_backend_map()
    print(f"Loaded {len(matched)} matched entries; "
          f"{len(session_backend_map)} sessions in log map")

    reasons: Counter = Counter()
    updates: list[tuple[dict, dict]] = []  # (func, fields_to_set)
    claimed_addrs: set[str] = set()

    for session_id, iso_date, short_names in walk_match_commits():
        try:
            commit_date = datetime.fromisoformat(iso_date.replace("Z", "+00:00"))
            if commit_date.tzinfo is None:
                commit_date = commit_date.replace(tzinfo=timezone.utc)
        except ValueError:
            reasons["bad_commit_date"] += 1
            continue

        bm = session_backend_map.get(session_id)
        if bm is None:
            backend, model = fallback_backend_model(commit_date)
            bm_source = "fallback"
        else:
            backend, model = bm
            bm_source = "logs"

        for short in short_names:
            candidates = by_short_name.get(short)
            if not candidates:
                continue
            for func in candidates:
                if func["address"] in claimed_addrs:
                    continue
                if func.get("matched_by_session_id"):
                    # Already attributed (prior run or Phase-2 native write).
                    claimed_addrs.add(func["address"])
                    reasons[f"skip_already_attributed"] += 1
                    continue
                fields = {
                    "matched_by_backend": backend,
                    "matched_by_model": model,
                    "matched_by_session_id": session_id,
                    "matched_at": commit_date.strftime("%Y-%m-%dT%H:%M:%SZ"),
                }
                updates.append((func, fields))
                claimed_addrs.add(func["address"])
                reasons[f"attributed_{bm_source}"] += 1
                # Each commit attributes a given short name to exactly
                # one matched DB row — overload disambiguation falls
                # back to "first commit walked wins". Break out so a
                # second occurrence of the short name in the same commit
                # body claims a different (yet-unclaimed) row.
                break

    print()
    print("Results:")
    for reason, n in sorted(reasons.items(), key=lambda kv: -kv[1]):
        print(f"  {reason:40s} {n:5d}")
    unattributed = len(matched) - len(claimed_addrs)
    print(f"  unattributed_matched (no commit found)  {unattributed:5d}")

    if args.dry_run:
        print(f"\n--dry-run: would update {len(updates)} entries.")
        for func, fields in updates[:10]:
            print(f"  {func['address']} {func['name'][:50]:50s} → "
                  f"{fields['matched_by_backend']}/{fields['matched_by_model']} "
                  f"@ {fields['matched_at']}")
        return

    if not updates:
        print("\nNothing to update.")
        return

    for func, fields in updates:
        for k, v in fields.items():
            func[k] = v

    save_db(db)
    print(f"\nDB updated: {len(updates)} entries gained matched_by_* fields.")


if __name__ == "__main__":
    main()
