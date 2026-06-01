#!/usr/bin/env python3
"""Select a slice of byte-exact-matching targets for the parallel workflow engine.

This reproduces the per-round target selection the matching conductor ran by hand
during the 2026-06-01 self-managed run (see docs/sessions/2026-06-01.md and the
matching-conductor skill). It emits the exact JSON array that
`.claude/workflows/overnight_match_parallel.js` consumes via its `args` input.

Two modes:
  fresh    — smallest untried real functions (status==untried), the bread-and-butter
             rounds. Walk size bands upward across rounds by raising --min-size, or
             pin an explicit [--min-size,--max-size] band.
  regalloc — failed-pool functions whose failure_notes look like a register-allocation
             / scheduler coloring residual (ADR-012 Mechanism-B). Run these every ~3rd
             round; the engine's step-6b source-coloring toolkit + reg-aware permuter
             target exactly this class.

Exclusions (the hard-won "don't bother" families, each with a reason) are applied in
both modes. A per-run reserve file (default logs/overnight_excluded.txt, gitignored)
records what's already been handed out so re-runs don't re-select near-misses; pass
--no-reserve for a dry run that doesn't append.

The DB `match_status` already excludes banked work (matched rows are never `untried`),
so the reserve file only guards against re-attempting within a single run.

Usage:
  python3 tools/select_match_targets.py --mode fresh --count 14 --min-size 40
  python3 tools/select_match_targets.py --mode fresh --min-size 108 --max-size 120
  python3 tools/select_match_targets.py --mode regalloc --count 14 --max-size 420
  python3 tools/select_match_targets.py --mode fresh --count 14 --dry-run   # print, don't reserve
  python3 tools/select_match_targets.py --mode fresh --count 14 --out /tmp/slice.json
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from common import load_db  # noqa: E402

REPO = Path(__file__).resolve().parents[1]
DEFAULT_RESERVE = REPO / "logs" / "overnight_excluded.txt"

# mode -> the DB match_status pool it draws from.
MODE_STATUS = {"fresh": "untried", "regalloc": "failed"}

# --- Exclusion families: proven non-source-matchable or out-of-policy. -------
# Each entry is (regex, why). Matched against the function name (case-insensitive).
NAME_EXCLUSIONS = [
    (r"VisitReferences", "ADR-014 mangling divergence (cumulative vs local backref); unmatchable_symbol_mangling"),
    # va_start environment block: __builtin_stdarg_start crashes pspsnc-under-wibo, so the
    # unfolded masked vararg prologue is unreachable. `printf`/`scanf` substrings catch the
    # whole family incl. wide (vswprintf/vsnwprintf) and narrow variants.
    (r"printf|scanf|cStrFormat|cStrAppend|cStr::cStr|cStr::Set|cFilename::cFilename|"
     r"\.\.\.|\bvarargs\b",
     "va_start env-block (__builtin_stdarg_start crashes wibo+pspsnc)"),
    (r"\bCollide\b", "ADR-012 Mechanism-A scheduler-latch class; deferred to real-TU reconstruction"),
    # libgcs soft-float archive helpers (also caught by obj_file below).
    (r"__cmpdf2|__adddf3|__subdf3|__muldf3|__divdf3|__negdf2|__fixdfsi|__floatsidf|"
     r"__extendsfdf2|__truncdfsf2|__unpack_|ultodp|_pow5mult",
     "libgcs soft-float archive object (not our source)"),
    (r"^sce[A-Z]", "Sony SDK syscall stub (null obj_file)"),
]
NAME_EXCLUSION_RE = re.compile("|".join("(?:%s)" % p for p, _ in NAME_EXCLUSIONS))

# failure_notes that indicate a register-allocation / scheduler coloring residual.
# Intentionally loose / recall-favoring: this is a coarse pre-sort for the regalloc batch,
# not a final classifier — the engine's step-6b toolkit + the prior notes do the real work.
REGALLOC_RE = re.compile(
    r"REG_ALLOC|coloring|callee.?save|register alloc|reg-alloc|live.?range|"
    r"scheduler tie|sched.*tie|operand.?order|canonicaliz|a1.?a2|a2.?a3|s0/s1|s1.?s3|s2/s3",
    re.I,
)


def _name(f):
    return f.get("method_name") or f.get("name") or ""


def _addr(f):
    return (f.get("address") or "").lower()


def _size(f):
    return int(f.get("size") or 0)


def _obj(f):
    return f.get("obj_file") or ""


def _status(f):
    return f.get("match_status") or ""


def _note_strings(f):
    """failure_notes entries are dicts with a 'notes' key (orchestrator-written) or plain
    strings (conductor-written hints); normalize to a list of strings."""
    out = []
    for n in (f.get("failure_notes") or []):
        out.append(n if isinstance(n, str) else (n.get("notes", "") if isinstance(n, dict) else str(n)))
    return out


def _notes_text(f):
    return " ".join(_note_strings(f))


def _excluded(f):
    """Skip-reason string if this function is in a skip family (independent of mode), else None."""
    obj = _obj(f)
    if not obj:
        return "null obj_file (SDK syscall stub)"
    if obj.startswith("libgcs"):
        return "libgcs soft-float archive"
    if NAME_EXCLUSION_RE.search(_name(f)):
        return "name in skip family"
    if not (f.get("mangled_symbol") or ""):
        # The engine needs the mangled symbol to extract the expected .o; a target without
        # one is unworkable, so drop it here rather than silently emit an empty symbol.
        return "no mangled_symbol (unworkable)"
    return None


def load_reserved(path: Path):
    reserved = set()
    if path.exists():
        for line in path.read_text().splitlines():
            for tok in line.split():
                if tok.startswith("0x"):
                    reserved.add(tok.lower())
    return reserved


def to_target(f, note_cap=0):
    notes = [s[:note_cap] for s in _note_strings(f) if s] if note_cap else []
    return {
        "address": _addr(f),
        "name": _name(f),
        "method_name": _name(f),
        "size": _size(f),
        "obj_file": _obj(f),
        "is_leaf": f.get("is_leaf"),
        "safe_name": f.get("safe_name") or "",
        "mangled_symbol": f.get("mangled_symbol") or "",
        "failure_notes": notes,
    }


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--mode", choices=["fresh", "regalloc"], required=True)
    ap.add_argument("--count", type=int, default=14, help="targets to select (default 14)")
    ap.add_argument("--min-size", type=int, default=40,
                    help="minimum function size in bytes (default 40; floors out 0-byte labels)")
    ap.add_argument("--max-size", type=int, default=None,
                    help="maximum size (default: none for fresh, 420 for regalloc)")
    ap.add_argument("--reserve-file", default=str(DEFAULT_RESERVE),
                    help="per-run reserve list to read+append (default logs/overnight_excluded.txt)")
    ap.add_argument("--tag", default=None, help="label written next to reserved addresses")
    ap.add_argument("--dry-run", "--no-reserve", dest="dry_run", action="store_true",
                    help="print selection but do NOT append to the reserve file")
    ap.add_argument("--out", default=None, help="also write the JSON array to this path")
    ap.add_argument("--note-cap", type=int, default=700,
                    help="max chars of each failure_note passed to the engine (regalloc mode)")
    args = ap.parse_args()

    max_size = args.max_size if args.max_size is not None else (420 if args.mode == "regalloc" else None)
    reserve_path = Path(args.reserve_file)
    reserved = load_reserved(reserve_path)

    funcs = load_db()
    if not isinstance(funcs, list):
        sys.exit("error: config/functions.json is not a JSON list (unexpected DB shape)")

    want_status = MODE_STATUS[args.mode]
    cands = []
    skipped = {}
    for f in funcs:
        if _status(f) != want_status:
            continue
        if _addr(f) in reserved:
            continue
        if _size(f) < args.min_size:
            continue
        if max_size is not None and _size(f) > max_size:
            continue
        why = _excluded(f)
        if why:
            skipped[why] = skipped.get(why, 0) + 1
            continue
        if args.mode == "regalloc" and not REGALLOC_RE.search(_notes_text(f)):
            continue
        cands.append(f)

    cands.sort(key=lambda f: (_size(f), _addr(f)))
    sel = cands[: args.count]
    note_cap = args.note_cap if args.mode == "regalloc" else 0
    targets = [to_target(f, note_cap) for f in sel]

    # Report to stderr so stdout stays a clean JSON array (pipeable into the engine args).
    print(f"# mode={args.mode} pool={want_status} candidates={len(cands)} selected={len(sel)} "
          f"(min={args.min_size} max={max_size})", file=sys.stderr)
    for f in sel:
        print(f"#   {_addr(f)}  {_size(f):4d}B  {_name(f)[:50]:50s} [{_obj(f)}]", file=sys.stderr)
    if skipped:
        print("# skip-family counts: " + ", ".join(f"{k}={v}" for k, v in sorted(skipped.items())),
              file=sys.stderr)

    # Fail loud rather than hand the engine a silently empty/short slice (stdout otherwise
    # looks like a normal valid JSON array and exit code stays 0).
    if not sel:
        sys.exit(f"error: no candidates matched (mode={args.mode}, min={args.min_size}, "
                 f"max={max_size}); pool/exclusions/reserve exhausted")
    if len(sel) < args.count:
        print(f"WARNING: requested {args.count}, only {len(sel)} available — short round "
              f"(pool/exclusions/reserve exhausted)", file=sys.stderr)

    payload = json.dumps(targets)
    print(payload)
    if args.out:
        Path(args.out).write_text(payload)
        print(f"# wrote {args.out}", file=sys.stderr)

    if not args.dry_run and sel:
        tag = args.tag or args.mode
        reserve_path.parent.mkdir(parents=True, exist_ok=True)
        with reserve_path.open("a") as fh:
            for f in sel:
                fh.write(f"{_addr(f)}  {tag}  {_name(f)[:40]}\n")
        print(f"# reserved {len(sel)} addrs in {reserve_path}", file=sys.stderr)
    elif args.dry_run:
        print("# dry-run: not reserved", file=sys.stderr)


if __name__ == "__main__":
    main()
