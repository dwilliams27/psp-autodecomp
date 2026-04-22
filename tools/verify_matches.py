#!/usr/bin/env python3
"""Bulk audit: for each DB entry marked `matched`, look up its stored
`src_file` and run tools.byte_match.check_byte_match. Mismatches are
reported (and optionally flipped to `untried` under --fix). Tooling
errors prevent `--fix` and make the tool exit non-zero so CI/scripts
notice.

Usage:
    python3 tools/verify_matches.py              # report only
    python3 tools/verify_matches.py --verbose    # per-function output
    python3 tools/verify_matches.py --fix        # flip mismatches to 'untried'
"""

import argparse
import os
import sys

from common import load_db, save_db
from byte_match import CompileFailed, check_byte_match


def verify_all(verbose: bool = False, fix: bool = False) -> int:
    """Returns a non-zero code when the DB is in a state the operator
    needs to fix (mismatches found, or tooling errors that prevent a
    clean audit). 0 means "everything verified, no action needed.\""""
    functions = load_db()
    matched = [f for f in functions if f["match_status"] == "matched"]
    if not matched:
        print("No matched functions in the database.")
        return 0

    verified = 0
    problems: list[tuple[dict, str]] = []
    compile_failures: list[tuple[dict, str]] = []
    tooling_errors: list[tuple[dict, str]] = []

    for func in matched:
        src_file = func.get("src_file")
        if not src_file:
            problems.append((func, "no src_file in DB entry — run the backfill migration"))
            continue
        if not os.path.exists(src_file):
            problems.append((func, f"src_file missing: {src_file}"))
            continue

        try:
            result = check_byte_match(func, src_file)
        except CompileFailed as e:
            # Compile failure means the current source the DB points at
            # won't build. That's a per-function failure — the match is
            # no longer verifiable from this source. Collect separately
            # so the fix loop can flip these to `failed` (not `untried`).
            compile_failures.append((func, str(e)[:200]))
            print(f"  ✗ {func['address']}  {func['size']:>4}B  {func['name']} — compile_failed")
            continue
        except RuntimeError as e:
            tooling_errors.append((func, str(e)[:200]))
            continue

        if result.ok:
            verified += 1
            if func.get("symbol_name") != result.sym_name:
                func["symbol_name"] = result.sym_name
            if verbose:
                print(f"  ✓ {func['address']}  {func['size']:>4}B  {func['name']}")
        else:
            note = f"{result.reason}"
            if result.sym_name:
                note += f" (sym={result.sym_name})"
            if result.diff_count:
                note += f" diff_count={result.diff_count}"
            problems.append((func, note))
            print(f"  ✗ {func['address']}  {func['size']:>4}B  {func['name']} — {note}")

    if tooling_errors:
        print()
        print(f"TOOLING ERRORS ({len(tooling_errors)}): audit cannot decide these.")
        for func, err in tooling_errors[:10]:
            print(f"  ! {func['address']}  {func['size']:>4}B  {func['name']} — {err}")
        if len(tooling_errors) > 10:
            print(f"  ... and {len(tooling_errors) - 10} more")

    total = len(matched)
    print()
    print("=" * 60)
    print("VERIFICATION RESULTS")
    print("=" * 60)
    print(f"Total matched in DB:    {total}")
    print(f"Byte-exact verified:    {verified}")
    print(f"Problems:               {len(problems)}")
    print(f"Compile failures:       {len(compile_failures)}")
    print(f"Tooling errors:         {len(tooling_errors)}")
    print("=" * 60)

    if fix:
        if tooling_errors:
            raise RuntimeError(
                f"{len(tooling_errors)} tooling errors (nm/objcopy failures, "
                f"etc.) — refusing to --fix. Resolve these first so we don't "
                f"flip real matches."
            )
        changed = 0
        for func, _ in problems:
            func["match_status"] = "untried"
            changed += 1
        for func, _ in compile_failures:
            func["match_status"] = "failed"
            changed += 1
        if changed or verified:
            save_db(functions)
        print(f"\n--fix: {len(problems)} → untried, {len(compile_failures)} → failed.")

    # Exit code: non-zero whenever the operator has work to do.
    return 1 if (problems or compile_failures or tooling_errors) else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--verbose", action="store_true",
                    help="Show each matched function as it's verified.")
    ap.add_argument("--fix", action="store_true",
                    help="Flip mismatches to 'untried' and compile-failures "
                         "to 'failed' (refuses if tooling errors exist).")
    args = ap.parse_args()
    return verify_all(verbose=args.verbose, fix=args.fix)


if __name__ == "__main__":
    sys.exit(main())
