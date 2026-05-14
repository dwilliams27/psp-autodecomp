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
    python3 tools/verify_matches.py --fix --fix-compile-failures
"""

import argparse
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
import os
import sys

from common import load_db, save_db
from byte_match import CompileFailed, check_byte_match, compile_src


def default_jobs() -> int:
    return max(1, min(8, os.cpu_count() or 1))


def normalize_src_file(src_file: str) -> str:
    normalized = os.path.normpath(src_file)
    if os.path.isabs(normalized):
        try:
            normalized = os.path.relpath(normalized, os.getcwd())
        except ValueError as exc:
            raise ValueError(f"src_file is not under this checkout: {src_file}") from exc
    if normalized.startswith("..") or os.path.isabs(normalized):
        raise ValueError(f"src_file is not repo-relative: {src_file}")
    return normalized


def verify_source_group(src_file: str, funcs: list[tuple[int, dict]]) -> list[tuple[int, str, dict, object]]:
    results: list[tuple[int, str, dict, object]] = []
    try:
        o_path = compile_src(src_file)
    except CompileFailed as e:
        msg = str(e)[:200]
        return [(idx, "compile_failure", func, msg) for idx, func in funcs]
    except RuntimeError as e:
        msg = str(e)[:200]
        return [(idx, "tooling_error", func, msg) for idx, func in funcs]

    for idx, func in funcs:
        try:
            result = check_byte_match(func, src_file, o_path=o_path)
        except CompileFailed as e:
            results.append((idx, "compile_failure", func, str(e)[:200]))
        except RuntimeError as e:
            results.append((idx, "tooling_error", func, str(e)[:200]))
        else:
            results.append((idx, "result", func, result))
    return results


def verify_all(verbose: bool = False, fix: bool = False,
               fix_compile_failures: bool = False,
               jobs: int = None) -> int:
    """Returns a non-zero code when the DB is in a state the operator
    needs to fix (mismatches found, or tooling errors that prevent a
    clean audit). 0 means "everything verified, no action needed.\""""
    if jobs is None:
        jobs = default_jobs()
    if jobs < 1:
        raise ValueError("--jobs must be >= 1")

    functions = load_db()
    matched = [f for f in functions if f["match_status"] == "matched"]
    if not matched:
        print("No matched functions in the database.")
        return 0

    verified = 0
    problems: list[tuple[dict, str]] = []
    compile_failures: list[tuple[dict, str]] = []
    tooling_errors: list[tuple[dict, str]] = []
    by_src: dict[str, list[tuple[int, dict]]] = defaultdict(list)
    result_by_index: dict[int, tuple[str, object]] = {}
    queued_indices: set[int] = set()

    for idx, func in enumerate(matched):
        src_file = func.get("src_file")
        if not src_file:
            problems.append((func, "no src_file in DB entry — run the backfill migration"))
            continue
        try:
            normalized_src_file = normalize_src_file(src_file)
        except ValueError as e:
            tooling_errors.append((func, str(e)))
            continue
        if not os.path.exists(normalized_src_file):
            problems.append((func, f"src_file missing: {normalized_src_file}"))
            continue
        by_src[normalized_src_file].append((idx, func))
        queued_indices.add(idx)

    if by_src:
        print(f"Verifying {len(matched)} matched functions from {len(by_src)} source files with {jobs} jobs.")

    if jobs == 1:
        for src_file, funcs in by_src.items():
            for idx, status, func, payload in verify_source_group(src_file, funcs):
                result_by_index[idx] = (status, payload)
    else:
        with ThreadPoolExecutor(max_workers=jobs) as executor:
            futures = [
                executor.submit(verify_source_group, src_file, funcs)
                for src_file, funcs in by_src.items()
            ]
            for future in as_completed(futures):
                for idx, status, func, payload in future.result():
                    result_by_index[idx] = (status, payload)

    for idx, func in enumerate(matched):
        outcome = result_by_index.get(idx)
        if outcome is None:
            if idx in queued_indices:
                tooling_errors.append((func, "internal verifier error: missing worker result"))
            continue
        status, payload = outcome
        if status == "compile_failure":
            compile_failures.append((func, payload))
            print(f"  ✗ {func['address']}  {func['size']:>4}B  {func['name']} — compile_failed")
            continue
        if status == "tooling_error":
            tooling_errors.append((func, payload))
            continue

        result = payload
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
            if result.expected_sym_name:
                note += f" expected={result.expected_sym_name}"
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
        fixed_compile = 0
        if fix_compile_failures:
            for func, _ in compile_failures:
                func["match_status"] = "failed"
                changed += 1
                fixed_compile += 1
        if changed or verified:
            save_db(functions)
        print(f"\n--fix: {len(problems)} → untried, {fixed_compile} → failed.")
        if compile_failures and not fix_compile_failures:
            print(
                f"--fix: left {len(compile_failures)} compile failures unchanged; "
                "pass --fix-compile-failures to mark them failed."
            )

    # Exit code: non-zero whenever the operator has work to do.
    return 1 if (problems or compile_failures or tooling_errors) else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--verbose", action="store_true",
                    help="Show each matched function as it's verified.")
    ap.add_argument("--fix", action="store_true",
                    help="Flip mismatches to 'untried' and compile-failures "
                         "to 'failed' only with --fix-compile-failures "
                         "(refuses if tooling errors exist).")
    ap.add_argument("--fix-compile-failures", action="store_true",
                    help="With --fix, also flip compile failures to 'failed'. "
                         "Omit for narrow repairs that should only unmatch "
                         "byte/provenance problems.")
    ap.add_argument("--jobs", type=int, default=default_jobs(),
                    help=f"Number of source files to verify in parallel "
                         f"(default: {default_jobs()}, use 1 for sequential).")
    args = ap.parse_args()
    return verify_all(
        verbose=args.verbose,
        fix=args.fix,
        fix_compile_failures=args.fix_compile_failures,
        jobs=args.jobs,
    )


if __name__ == "__main__":
    sys.exit(main())
