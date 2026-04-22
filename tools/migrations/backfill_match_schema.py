#!/usr/bin/env python3
"""One-shot migration: populate `src_file` and `symbol_name` for every
currently-matched DB entry. Re-verifies each using the canonical
byte_match.check_byte_match(). Flips anything that can't be verified to
`failed` with a note.

Lookup order for src_file per entry:
  1. The entry's own `src_file` field if already present (re-runs safe).
  2. Most recent session_results/*.json containing the address with
     status="matched" — use its `file` field.
  3. None → flip to `failed`.

Safe to re-run: idempotent on entries that pass verification. No destructive
action on entries whose status isn't `matched`.

Usage:
    python3 tools/migrations/backfill_match_schema.py --dry-run
    python3 tools/migrations/backfill_match_schema.py          # apply
"""

import argparse
import json
import os
import sys
from collections import Counter
from pathlib import Path

_REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(_REPO_ROOT / "tools"))

from byte_match import (
    REASON_OK,
    check_byte_match,
)
from common import load_db, save_db


SESSION_RESULTS_DIR = _REPO_ROOT / "logs" / "session_results"


def build_session_src_map():
    """Walk logs/session_results/*.json and build {addr: (src_file, mtime)}.
    Keeps the most recent matched entry per address."""
    mapping: dict[str, tuple[str, float]] = {}
    for path in SESSION_RESULTS_DIR.glob("*.json"):
        try:
            with open(path) as f:
                entries = json.load(f)
        except (json.JSONDecodeError, OSError):
            continue
        if not isinstance(entries, list):
            continue
        mtime = path.stat().st_mtime
        for e in entries:
            if not isinstance(e, dict):
                continue
            if e.get("status") != "matched":
                continue
            addr = e.get("address")
            src = e.get("file")
            if not addr or not src:
                continue
            prev = mapping.get(addr)
            if prev is None or prev[1] < mtime:
                mapping[addr] = (src, mtime)
    return {addr: src for addr, (src, _) in mapping.items()}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true",
                    help="Don't write the DB; report what would change.")
    args = ap.parse_args()

    db = load_db()
    session_src = build_session_src_map()

    matched = [f for f in db if f.get("match_status") == "matched"]
    print(f"Backfill: {len(matched)} matched entries to verify")
    print(f"Session-results source map: {len(session_src)} addresses")

    reasons: Counter = Counter()
    to_flip: list[tuple[dict, str]] = []  # (func, note)
    to_record: list[tuple[dict, str, str]] = []  # (func, src_file, sym_name)

    for func in matched:
        addr = func["address"]
        class_name = func.get("class_name") or ""

        # Candidate src files in priority order:
        #   1. Already-stored src_file (re-run safe).
        #   2. Most recent session_results `file` for this address.
        #   3. Convention: src/<class_name>.cpp (catches cases where an old
        #      session misrecorded the file AND a correct one was added later).
        candidates: list[str] = []
        for src in (func.get("src_file"), session_src.get(addr)):
            if src and src not in candidates:
                candidates.append(src)
        if class_name:
            convention = f"src/{class_name}.cpp"
            if convention not in candidates:
                candidates.append(convention)

        # Filter to ones that actually exist on disk.
        candidates = [c for c in candidates if os.path.exists(c)]
        if not candidates:
            note = "no existing src file found (via entry, session_results, or convention)"
            to_flip.append((func, note))
            reasons["no_src_file"] += 1
            continue

        # Try each candidate; first successful verification wins. Collect
        # the failure reason from the LAST candidate we tried so the flip
        # note is informative.
        last_reason: str | None = None
        last_note: str | None = None
        verified = False
        for src_file in candidates:
            try:
                result = check_byte_match(func, src_file)
            except RuntimeError as e:
                last_reason = "tooling_error"
                last_note = f"{src_file}: tooling error: {e}"[:300]
                continue

            if result.ok:
                to_record.append((func, src_file, result.sym_name))
                reasons["verified"] += 1
                verified = True
                break

            last_reason = result.reason
            last_note = (
                f"{src_file}: {result.reason}"
                + (f" sym={result.sym_name}" if result.sym_name else "")
                + (f" diff={result.diff_count}" if result.diff_count else "")
            )[:300]

        if not verified:
            to_flip.append((func, last_note or "verification failed"))
            reasons[last_reason or "unknown"] += 1

    print()
    print("Results:")
    for r, c in sorted(reasons.items(), key=lambda kv: -kv[1]):
        print(f"  {r:40s} {c:5d}")

    print()
    print(f"Would record src_file+sym_name on:   {len(to_record)}")
    print(f"Would flip to 'failed':              {len(to_flip)}")

    if args.dry_run:
        print("\n--dry-run: no DB changes written.")
        print("\nSample of flips (first 10):")
        for func, note in to_flip[:10]:
            print(f"  {func['address']} {func['name'][:50]:50s} : {note[:80]}")
        return

    # Apply
    for func, src_file, sym_name in to_record:
        func["src_file"] = src_file
        func["symbol_name"] = sym_name

    for func, note in to_flip:
        func["match_status"] = "failed"
        notes = func.setdefault("failure_notes", [])
        notes.append({
            "session": "backfill_migration",
            "notes": f"[backfill] {note}",
        })
        func["failure_notes"] = notes[-5:]

    save_db(db)
    print(f"\nDB updated: {len(to_record)} entries gained src_file+symbol_name,")
    print(f"            {len(to_flip)} entries flipped to 'failed'.")


if __name__ == "__main__":
    main()
