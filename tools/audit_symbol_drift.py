#!/usr/bin/env python3
"""Audit matched DB entries for emitted-symbol drift.

For each `match_status == matched` entry, compile its recorded `src_file`
once, inspect the text symbols in the resulting object, and require the
DB's authoritative `mangled_symbol` to be emitted exactly. If the exact
symbol is absent but a same-class/same-method heuristic symbol exists,
the entry is reported as symbol-name drift.

This is intentionally a one-off triage tool: it does not modify the DB.
"""

from __future__ import annotations

import argparse
import json
import os
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from byte_match import (  # noqa: E402
    CompileFailed,
    compile_src,
    sym_heuristically_encodes_func,
    symbols_with_bytes_and_relocs,
)
from common import load_db, save_db  # noqa: E402


STATUS_UNMATCHABLE_SYMBOL_MANGLING = "unmatchable_symbol_mangling"


def _record(func, category, **extra):
    rec = {
        "category": category,
        "address": func.get("address"),
        "name": func.get("name"),
        "src_file": func.get("src_file"),
        "mangled_symbol": func.get("mangled_symbol"),
        "recorded_symbol_name": func.get("symbol_name"),
    }
    rec.update(extra)
    return rec


def audit(mark_unmatchable_symbol_mangling=False):
    functions = load_db()
    matched = [f for f in functions if f.get("match_status") == "matched"]
    by_src = {}
    records = []
    ok = 0
    changed = 0

    for func in matched:
        if not func.get("mangled_symbol"):
            records.append(_record(func, "missing_mangled_symbol"))
            continue
        src_file = (func.get("src_file") or "").lstrip("./")
        if not src_file:
            records.append(_record(func, "no_src_file"))
            continue
        if not os.path.exists(src_file):
            records.append(_record(func, "src_file_missing"))
            continue
        by_src.setdefault(src_file, []).append(func)

    compiled = {}
    for src_file, funcs in sorted(by_src.items()):
        try:
            o_path = compile_src(src_file)
            syms = symbols_with_bytes_and_relocs(o_path)
            compiled[src_file] = (o_path, syms, None)
        except CompileFailed as e:
            compiled[src_file] = (None, {}, ("compile_failed", str(e)[:500]))
        except RuntimeError as e:
            compiled[src_file] = (None, {}, ("tooling_error", str(e)[:500]))

        o_path, syms, error = compiled[src_file]
        if error:
            category, message = error
            for func in funcs:
                records.append(_record(func, category, error=message))
            continue

        sym_names = set(syms)
        for func in funcs:
            expected = func.get("mangled_symbol")
            if expected in sym_names:
                ok += 1
                continue

            heuristic = sorted(
                sn for sn in sym_names
                if sym_heuristically_encodes_func(sn, func)
            )
            if heuristic:
                records.append(_record(
                    func,
                    "symbol_name_mismatch",
                    emitted_candidates=heuristic,
                    o_file=o_path,
                ))
                if mark_unmatchable_symbol_mangling:
                    func["match_status"] = STATUS_UNMATCHABLE_SYMBOL_MANGLING
                    func["symbol_mangling_expected"] = expected
                    func["symbol_mangling_emitted_candidates"] = heuristic[:8]
                    changed += 1
            else:
                records.append(_record(
                    func,
                    "no_matching_emitted_symbol",
                    emitted_candidate_count=len(sym_names),
                    o_file=o_path,
                ))

    counts = Counter(r["category"] for r in records)
    if changed:
        save_db(functions)

    return {
        "summary": {
            "matched_entries": len(matched),
            "ok": ok,
            "problems": len(records),
            "marked_unmatchable_symbol_mangling": changed,
            "categories": dict(sorted(counts.items())),
        },
        "records": records,
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--json", default="logs/symbol_drift_audit.json",
                    help="Path for full JSON report "
                         "(default: logs/symbol_drift_audit.json).")
    ap.add_argument("--print-mismatches", action="store_true",
                    help="Print every non-ok record to stdout.")
    ap.add_argument("--mark-unmatchable-symbol-mangling", action="store_true",
                    help="Rewrite symbol-name mismatches to "
                         "match_status=unmatchable_symbol_mangling. "
                         "Does not mark byte mismatches or missing symbols.")
    args = ap.parse_args()

    report = audit(
        mark_unmatchable_symbol_mangling=(
            args.mark_unmatchable_symbol_mangling
        )
    )
    os.makedirs(os.path.dirname(args.json) or ".", exist_ok=True)
    with open(args.json, "w") as f:
        json.dump(report, f, indent=2)

    summary = report["summary"]
    print(
        "symbol drift audit: "
        f"{summary['ok']}/{summary['matched_entries']} exact, "
        f"{summary['problems']} problem(s)"
    )
    for category, count in summary["categories"].items():
        print(f"  {category}: {count}")
    if summary["marked_unmatchable_symbol_mangling"]:
        print(
            "marked unmatchable_symbol_mangling: "
            f"{summary['marked_unmatchable_symbol_mangling']}"
        )
    print(f"report: {args.json}")

    if args.print_mismatches:
        for rec in report["records"]:
            emitted = rec.get("emitted_candidates") or []
            emitted_s = f" emitted={emitted[:3]}" if emitted else ""
            print(
                f"{rec['category']} {rec.get('address')} "
                f"{rec.get('name')} expected={rec.get('mangled_symbol')}"
                f"{emitted_s}"
            )

    return 1 if summary["problems"] else 0


if __name__ == "__main__":
    sys.exit(main())
