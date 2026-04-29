#!/usr/bin/env python3
"""One-shot script: synthesize logs/attempts.jsonl from existing
logs/match_*.jsonl event streams.

Phase-2 attempts.jsonl is written natively by the orchestrator going
forward. This extractor backfills history so the A/B reporting tool
sees a single, consistent corpus across all runs — the orchestrator
already emits enough information that no log-mining heuristics are
needed; this is a straight projection.

Per (session, function) record schema follows
docs/direction/003-multi-agent-ab-architecture.md §3. Fields the
historical logs don't carry (token usage, cost) come through as None
rather than zeroed — silent zeros would skew cost-per-match averages.

Usage:
    python3 tools/ab_attempts_extract.py                 # write logs/attempts_backfill.jsonl
    python3 tools/ab_attempts_extract.py -o logs/attempts.jsonl
    python3 tools/ab_attempts_extract.py --dry-run       # parse but don't write
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

_REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(_REPO_ROOT / "tools"))

from ab_schedule import identity_key


# Counts surface at end of run so corrupt lines don't silently
# disappear from the A/B corpus.
_PARSE_ERRORS: dict = {"corrupt_lines": 0, "files_with_errors": set()}


def _iter_log_events(path: Path):
    with open(path) as f:
        for lineno, line in enumerate(f, 1):
            line = line.strip()
            if not line:
                continue
            try:
                yield json.loads(line)
            except json.JSONDecodeError as e:
                _PARSE_ERRORS["corrupt_lines"] += 1
                _PARSE_ERRORS["files_with_errors"].add(path.name)
                print(f"warn: {path.name}:{lineno}: corrupt JSON ({e}); skipping",
                      file=sys.stderr)


def extract_attempts_from_log(log_path: Path):
    """Walk one match_*.jsonl and yield attempts.jsonl-shaped records.

    State machine per session_id: capture run_id + start fields from
    `session_start`, accumulate per-function `verify_failed`/
    `function_result` results, then emit per-attempt records on
    `session_done` (success path) or terminal session-error events.
    """
    run_id = None
    sessions: dict[str, dict] = {}

    for ev in _iter_log_events(log_path):
        et = ev.get("event")
        if et == "run_start":
            run_id = ev.get("run_id") or log_path.stem
            continue

        if et == "session_start":
            sid = ev.get("session_id")
            if not sid:
                continue
            sessions[sid] = {
                "run_id": run_id or log_path.stem,
                "session_id": sid,
                "backend": ev.get("backend") or "",
                "model": ev.get("model") or "",
                "effort": ev.get("effort") or "",
                "identity": ev.get("identity") or "",
                "variant": ev.get("variant") or "",
                "functions": list(ev.get("functions") or []),
                "ts": ev.get("timestamp"),
                "verify_reasons": {},
                "function_results": {},
                "session_duration_s": 0.0,
                "session_error_kind": None,
                "agent_refused": False,
                "prep_error": None,
            }
            continue

        sid = ev.get("session_id")
        if sid is None or sid not in sessions:
            continue
        sess = sessions[sid]

        if et == "verify_failed":
            addr = ev.get("address")
            if addr:
                sess["verify_reasons"][addr] = ev.get("reason") or "verify_failed"
        elif et == "verify_error":
            addr = ev.get("address")
            if addr:
                sess["verify_reasons"][addr] = "verify_tooling_error"
        elif et == "rejected_extern_c_class_method":
            addr = ev.get("address")
            if addr:
                sess["verify_reasons"][addr] = "rejected_extern_c_class_method"
                sess.setdefault("rejected_xc", set()).add(addr)
        elif et == "rejected_assembly_only":
            # File-scoped rejection: the matched_funcs whose declared
            # src is in the rejected set get flipped to failed in the
            # orchestrator. We can't determine which addrs from this
            # event alone, but the function_result event will record
            # the post-rejection status.
            pass
        elif et == "function_result":
            addr = ev.get("address")
            if addr:
                sess["function_results"][addr] = ev.get("status") or ""
        elif et == "session_done":
            sess["session_duration_s"] = float(ev.get("duration_s") or 0.0)
            yield from _emit_session(sess)
            del sessions[sid]
        elif et == "session_error" or et == "system_error" or et == "prep_error":
            sess["session_duration_s"] = float(ev.get("duration_s") or 0.0)
            kind = ev.get("kind") or et
            if kind == "refusal":
                sess["agent_refused"] = True
                sess["session_error_kind"] = "refusal"
            elif et == "prep_error":
                sess["prep_error"] = ev.get("error") or "prep_error"
                sess["session_error_kind"] = "prep_error"
            elif et == "system_error":
                sess["session_error_kind"] = "system_error"
            else:
                sess["session_error_kind"] = "agent_fail"
            yield from _emit_session(sess)
            del sessions[sid]

    # Orphaned sessions (no session_done / session_error) — emit as
    # truncated records so they're still visible in the A/B corpus
    # rather than silently dropped.
    for sess in sessions.values():
        sess.setdefault("session_error_kind", "truncated_log")
        yield from _emit_session(sess)


def _emit_session(sess: dict):
    n = max(1, len(sess["functions"]))
    share_s = round(sess["session_duration_s"] / n, 3)
    rejected_xc = sess.get("rejected_xc") or set()
    for func in sess["functions"]:
        addr = func.get("address")
        verified = sess["function_results"].get(addr)
        verify_reason = sess["verify_reasons"].get(addr)
        # Pre-Phase-2 logs don't record claimed_status; the verify_reason
        # presence is a reasonable proxy for "agent claimed match → got
        # rejected at verify". Otherwise we leave it null rather than
        # guessing.
        if verified == "matched":
            claimed = "matched"
        elif verify_reason:
            claimed = "matched"
        elif verified == "failed":
            claimed = "failed"
        else:
            claimed = None

        yield {
            "ts": sess["ts"],
            "run_id": sess["run_id"],
            "session_id": sess["session_id"],
            "backend": sess["backend"],
            "model": sess["model"],
            "effort": sess.get("effort") or "",
            "identity": (
                sess.get("identity")
                or identity_key(
                    sess["backend"],
                    sess["model"],
                    sess.get("effort") or "",
                )
            ),
            "variant": sess["variant"],
            "address": addr,
            "name": func.get("name"),
            "class_name": func.get("class_name") or "",
            "size": func.get("size", 0),
            "is_leaf": bool(func.get("is_leaf", False)),
            "obj_file": func.get("obj_file") or "",
            "claimed_status": claimed,
            "verified_status": verified,
            "verify_reason": verify_reason,
            "rejected_extern_c": addr in rejected_xc,
            "session_duration_s": round(sess["session_duration_s"], 3),
            "session_share_s": share_s,
            "input_tokens": None,
            "output_tokens": None,
            "cached_tokens": None,
            "cost_usd": None,
            "had_usage_data": False,
            "agent_refused": bool(sess.get("agent_refused")),
            "prep_error": sess.get("prep_error"),
            "session_error_kind": sess.get("session_error_kind"),
        }


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-o", "--output", default="logs/attempts_backfill.jsonl",
                    help="Output path (default: logs/attempts_backfill.jsonl)")
    ap.add_argument("--dry-run", action="store_true",
                    help="Parse and report counts; don't write the output.")
    args = ap.parse_args()

    log_dir = _REPO_ROOT / "logs"
    log_paths = sorted(log_dir.glob("match_*.jsonl"))
    # match_latest.jsonl is a symlink to one of the dated files; skip it
    # to avoid double-counting attempts from the same run.
    log_paths = [p for p in log_paths if not p.is_symlink()]
    if not log_paths:
        print("No match_*.jsonl logs found.", file=sys.stderr)
        sys.exit(1)

    total = 0
    by_identity: dict[str, int] = {}

    out_path = (_REPO_ROOT / args.output).resolve()
    out_handle = None
    if not args.dry_run:
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_handle = open(out_path, "w")

    try:
        for log_path in log_paths:
            for rec in extract_attempts_from_log(log_path):
                total += 1
                by_identity[rec["identity"] or rec["backend"] or "?"] = (
                    by_identity.get(rec["identity"] or rec["backend"] or "?", 0) + 1
                )
                if out_handle:
                    out_handle.write(json.dumps(rec) + "\n")
    finally:
        if out_handle:
            out_handle.close()

    print(f"Scanned {len(log_paths)} log files.")
    print(f"Attempt records: {total}")
    for ident, n in sorted(by_identity.items(), key=lambda kv: -kv[1]):
        print(f"  {ident:30s} {n}")
    if _PARSE_ERRORS["corrupt_lines"]:
        print(f"\nWARNING: {_PARSE_ERRORS['corrupt_lines']} corrupt JSON line(s) "
              f"skipped across {len(_PARSE_ERRORS['files_with_errors'])} file(s).",
              file=sys.stderr)
    if args.dry_run:
        print("\n--dry-run: nothing written.")
    else:
        print(f"\nWrote {out_path}")


if __name__ == "__main__":
    main()
