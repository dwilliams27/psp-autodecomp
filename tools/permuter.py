#!/usr/bin/env python3
"""SNC-aware source permuter for last-mile byte matching.

Mechanically closes byte gaps by randomly mutating C source and comparing
compiled output against original EBOOT bytes. Designed for near-miss functions
where agents get the structure right but can't nail exact instruction ordering
or register allocation.

Usage:
    python3 tools/permuter.py src/eTextureMap.cpp 0x0007ae64 --time 600
    python3 tools/permuter.py src/eTextureMap.cpp 0x0007ae64 --workers 8 --save-best
    python3 tools/permuter.py src/eTextureMap.cpp 0x0007ae64 --symbol __0f...
    python3 tools/permuter.py src/eTextureMap.cpp eTextureMap::Read --time 300

See docs/decisions/005-snc-permuter.md for design rationale.
"""

import argparse
import multiprocessing
import os
import random
import shutil
import subprocess
import sys
import tempfile
import time

from common import (EBOOT_PATH, TEXT_FILE_OFFSET,
                    load_db, find_function,
                    mask_relocation_bytes)
from byte_match import extract_section, symbols_with_bytes_and_relocs
from mutations import mutate


# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

WIBO = "extern/wibo"
SNC = "extern/snc/pspsnc.exe"
BASE_CFLAGS = ["-c", "-O2", "-G0", "-Iextern/include", "-Iinclude"]

# Compiler flag variations (low-priority mutation)
FLAG_VARIANTS = [
    ["-Xsched=2"],
    ["-Xsched=1"],
    ["-Xsched=2", "-Xmopt=0"],
    ["-Xsched=1", "-Xmopt=0"],
]


class SymbolSelectionError(RuntimeError):
    """Raised when a compiled object cannot be mapped to one target symbol."""


# ---------------------------------------------------------------------------
# Compilation and scoring
# ---------------------------------------------------------------------------

def _detect_suffix(source):
    """Detect whether source is C++ or C."""
    return ".cpp" if "class " in source or "::" in source else ".c"


def _detect_sched(src_path):
    """Auto-detect sched flag from source filename."""
    basename = os.path.basename(src_path)
    sched1_prefixes = (
        "eTextureMap", "eBumpOffsetMap", "eDynamicMeshMorphTarget",
        "eCollisionConstraint", "eCompoundShape", "eInputKeyboard",
        "eDynamicModel",
    )
    for prefix in sched1_prefixes:
        if basename.startswith(prefix):
            return ["-Xsched=1"]
    return ["-Xsched=2"]


def compile_source(src_path, o_path, cflags):
    """Compile a source file with SNC. Returns True on success."""
    cmd = [WIBO, SNC] + cflags + ["-o", o_path, src_path]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        return result.returncode == 0
    except subprocess.TimeoutExpired:
        print(f"WARNING: SNC timed out (30s) on {src_path}", file=sys.stderr)
        return False


def extract_text_bytes(o_path):
    """Extract raw .text section bytes from a compiled .o file."""
    data = extract_section(o_path, ".text")
    return data or None


def select_symbol_bytes(symbols, target_size, target_symbol=None):
    """Select exactly one symbol from `symbols_with_bytes_and_relocs()` output.

    Prefer an explicit DB-authoritative symbol. If none is available, fall
    back only when exactly one emitted symbol has the requested size. Ambiguous
    size matches fail closed; choosing the first same-size symbol poisons the
    permuter's score and wastes the whole run.
    """
    if target_symbol:
        entry = symbols.get(target_symbol)
        if entry is None:
            candidates = ", ".join(sorted(symbols)[:8]) or "<none>"
            raise SymbolSelectionError(
                f"target symbol {target_symbol!r} not found; "
                f"available symbols: {candidates}"
            )
        return target_symbol, entry

    size_matches = [
        (name, entry) for name, entry in sorted(symbols.items())
        if len(entry[0]) == target_size
    ]
    if len(size_matches) == 1:
        return size_matches[0]
    if len(size_matches) > 1:
        candidates = ", ".join(name for name, _entry in size_matches[:8])
        raise SymbolSelectionError(
            f"ambiguous target: {len(size_matches)} symbols are "
            f"{target_size}B ({candidates}); pass --symbol"
        )

    if not symbols:
        raise SymbolSelectionError("no text symbols found")
    candidates = ", ".join(
        f"{name}({len(entry[0])}B)" for name, entry in sorted(symbols.items())[:8]
    )
    raise SymbolSelectionError(
        f"no symbol of size {target_size}B; available symbols: {candidates}"
    )


def locate_symbol_bytes(o_path, text_bytes, target_size, target_symbol=None):
    """Return (symbol_name, func_bytes, relocations) for the target function."""
    symbols = symbols_with_bytes_and_relocs(o_path)
    if symbols:
        name, (func_bytes, _sym_off, relocs) = select_symbol_bytes(
            symbols, target_size, target_symbol=target_symbol)
        return name, func_bytes, relocs

    # Last-resort compatibility for stripped single-function objects. Normal
    # SNC objects should have symbols, so do not use this when the caller asked
    # for a specific symbol.
    if text_bytes is not None and not target_symbol and len(text_bytes) == target_size:
        return "<raw .text>", text_bytes, []

    raise SymbolSelectionError("could not isolate target function")


def get_symbol_bytes(o_path, text_bytes, target_size, target_symbol=None, warn=False):
    """Compatibility wrapper returning (func_bytes, relocations)."""
    _name, func_bytes, relocs = locate_symbol_bytes(
        o_path, text_bytes, target_size, target_symbol=target_symbol)
    return func_bytes, relocs


def score_bytes(compiled_bytes, expected_bytes, relocations):
    """Score compiled bytes against expected. Returns differing byte count.

    Returns float('inf') for compile failures or size mismatches.
    """
    if compiled_bytes is None or len(compiled_bytes) != len(expected_bytes):
        return float("inf")

    if relocations:
        compiled_masked = mask_relocation_bytes(compiled_bytes, relocations)
        expected_masked = mask_relocation_bytes(expected_bytes, relocations)
    else:
        compiled_masked = compiled_bytes
        expected_masked = expected_bytes

    return sum(1 for a, b in zip(compiled_masked, expected_masked) if a != b)


def permuter_suitability_reason(func_size, symbol_size, baseline_diff):
    """Return skip reason when the target is not a last-mile permuter case."""
    if symbol_size != func_size:
        delta = abs(symbol_size - func_size)
        return (
            f"symbol size {symbol_size}B does not match DB size "
            f"{func_size}B (delta {delta}B)"
        )
    if baseline_diff == float("inf"):
        return "baseline could not be scored"
    max_reasonable_diff = max(128, int(func_size * 0.20))
    if baseline_diff > max_reasonable_diff:
        return (
            f"baseline diff {baseline_diff}B exceeds last-mile gate "
            f"{max_reasonable_diff}B"
        )
    return None


# ---------------------------------------------------------------------------
# Worker
# ---------------------------------------------------------------------------

def _worker_init(expected_bytes, func_size, base_cflags, target_sym):
    """Initialize worker process globals."""
    global _w_expected, _w_func_size, _w_cflags, _w_target_sym
    _w_expected = expected_bytes
    _w_func_size = func_size
    _w_cflags = base_cflags
    _w_target_sym = target_sym


def _worker_eval(args):
    """Evaluate a single candidate. Returns (score, source, status)."""
    source, suffix, seed, flag_override = args
    random.seed(seed)

    cflags = flag_override if flag_override else _w_cflags

    workdir = tempfile.mkdtemp(prefix="permuter_")
    src_path = os.path.join(workdir, "candidate" + suffix)
    o_path = src_path + ".o"

    try:
        with open(src_path, "w") as f:
            f.write(source)

        if not compile_source(src_path, o_path, cflags):
            return float("inf"), source, "compile_fail"

        text_bytes = extract_text_bytes(o_path)
        try:
            func_bytes, relocs = get_symbol_bytes(
                o_path, text_bytes, _w_func_size, _w_target_sym
            )
        except SymbolSelectionError:
            return float("inf"), source, "symbol_fail"

        sc = score_bytes(func_bytes, _w_expected, relocs)
        return sc, source, "ok"
    finally:
        shutil.rmtree(workdir, ignore_errors=True)


# ---------------------------------------------------------------------------
# Main search loop
# ---------------------------------------------------------------------------

def generate_candidates(source, batch_size, cflags, suffix, flag_mutate_chance=0.05):
    """Generate a batch of (mutated_source, suffix, seed, flag_override) tuples."""
    candidates = []
    attempts = 0
    max_attempts = batch_size * 5

    while len(candidates) < batch_size and attempts < max_attempts:
        attempts += 1
        result = mutate(source)
        if result is None:
            continue
        mutated, name = result

        # Occasionally try a flag variation
        flag_override = None
        if random.random() < flag_mutate_chance:
            flag_override = [f for f in cflags
                             if not f.startswith("-Xsched=")
                             and not f.startswith("-Xmopt=")]
            flag_override.extend(random.choice(FLAG_VARIANTS))

        candidates.append((mutated, suffix, random.getrandbits(64), flag_override))

    return candidates


def run_search(source, func_addr, func_size, cflags, eboot_data,
               target_symbol=None, time_limit=300, num_workers=None,
               save_best_path=None, save_improved=False, no_gate=False):
    """Run the permuter search loop.

    Returns (best_score, best_source, stats_dict).
    """
    if num_workers is None:
        num_workers = os.cpu_count() or 4

    batch_size = num_workers * 2
    suffix = _detect_suffix(source)

    # Score the original
    with tempfile.NamedTemporaryFile(suffix=suffix, mode="w", delete=False) as f:
        f.write(source)
        orig_src_path = f.name
    orig_o_path = orig_src_path + ".o"

    try:
        if not compile_source(orig_src_path, orig_o_path, cflags):
            print("ERROR: Original source does not compile.", file=sys.stderr)
            return float("inf"), source, {}

        text_bytes = extract_text_bytes(orig_o_path)
        if text_bytes is None:
            raise RuntimeError(
                f"Failed to extract .text section from {orig_o_path}. "
                f"Check that objcopy supports the .o format."
            )

        selected_symbol, func_bytes, relocs = locate_symbol_bytes(
            orig_o_path, text_bytes, func_size, target_symbol
        )

        start_offset = func_addr + TEXT_FILE_OFFSET
        expected_bytes = eboot_data[start_offset:start_offset + func_size]
        baseline = score_bytes(func_bytes, expected_bytes, relocs)
    finally:
        for p in [orig_src_path, orig_o_path]:
            try:
                os.unlink(p)
            except FileNotFoundError:
                pass

    stats = {
        "baseline": baseline,
        "total": 0,
        "compiled": 0,
        "improvements": 0,
        "best_scores": [baseline],
        "selected_symbol": selected_symbol,
        "symbol_size": len(func_bytes),
    }

    if baseline == 0:
        print("Already an exact match!")
        return 0, source, stats

    skip_reason = permuter_suitability_reason(
        func_size, len(func_bytes), baseline)
    if skip_reason and not no_gate:
        stats["skipped"] = skip_reason
        print(f"PERMUTER_SKIPPED_NOT_LAST_MILE: {skip_reason}")
        return baseline, source, stats

    if baseline == float("inf"):
        raise RuntimeError(
            "Could not score original source against EBOOT. "
            "Symbol size mismatch or relocation error."
        )

    print(f"Target symbol: {selected_symbol}")
    print(f"Baseline: {baseline} bytes differ ({func_size}B function)")
    print(f"Workers: {num_workers}  |  Time limit: {time_limit}s")
    print()

    best_score = baseline
    best_source = source

    start_time = time.time()
    last_print = start_time

    # Pass only the sliced expected bytes to workers, not the full EBOOT
    start_offset = func_addr + TEXT_FILE_OFFSET
    expected_slice = eboot_data[start_offset:start_offset + func_size]

    pool = multiprocessing.Pool(
        num_workers,
        initializer=_worker_init,
        initargs=(expected_slice, func_size, cflags, target_symbol),
    )

    try:
        while True:
            elapsed = time.time() - start_time
            if elapsed >= time_limit:
                break

            candidates = generate_candidates(best_source, batch_size, cflags, suffix)
            if not candidates:
                candidates = generate_candidates(source, batch_size, cflags, suffix)
                if not candidates:
                    break

            for sc, src, status in pool.imap_unordered(_worker_eval, candidates):
                stats["total"] += 1

                if status == "compile_fail":
                    continue
                stats["compiled"] += 1

                if sc < best_score:
                    best_score = sc
                    best_source = src
                    stats["improvements"] += 1
                    stats["best_scores"].append(best_score)

                    elapsed = time.time() - start_time
                    print(f"  [{elapsed:6.1f}s] Improved: {best_score} bytes differ "
                          f"(was {stats['best_scores'][-2]})")

                    if save_best_path and (best_score == 0 or save_improved):
                        with open(save_best_path, "w") as f:
                            f.write(best_source)

                    if best_score == 0:
                        elapsed = time.time() - start_time
                        print(f"\n  EXACT MATCH found in {elapsed:.1f}s "
                              f"after {stats['total']} candidates!")
                        return 0, best_source, stats

            # Periodic status
            now = time.time()
            if now - last_print >= 10:
                elapsed = now - start_time
                rate = stats["compiled"] / elapsed if elapsed > 0 else 0
                print(f"  [{elapsed:6.1f}s] {stats['compiled']}/{stats['total']} compiled  "
                      f"| best={best_score}  | {rate:.1f} candidates/sec")
                last_print = now

    finally:
        pool.terminate()
        pool.join()

    elapsed = time.time() - start_time
    rate = stats["compiled"] / elapsed if elapsed > 0 else 0

    print()
    print(f"Search complete: {elapsed:.1f}s elapsed")
    print(f"  Candidates: {stats['total']} generated, {stats['compiled']} compiled")
    print(f"  Rate: {rate:.1f} candidates/sec")
    print(f"  Best: {best_score} bytes differ (started at {baseline})")
    print(f"  Improvements: {stats['improvements']}")

    return best_score, best_source, stats


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="SNC-aware source permuter for last-mile byte matching"
    )
    parser.add_argument("source", help="Source file to permute")
    parser.add_argument("target",
                        help="Function address (0x...) or name (e.g. eTextureMap::Read)")
    parser.add_argument("--time", type=int, default=300,
                        help="Time limit in seconds (default: 300)")
    parser.add_argument("--workers", type=int, default=None,
                        help="Number of parallel workers (default: CPU count)")
    parser.add_argument("--save-best", action="store_true",
                        help="Overwrite source file when an exact match is found")
    parser.add_argument("--save-improved", action="store_true",
                        help="With --save-best/--save-to, also save non-exact improvements")
    parser.add_argument("--save-to",
                        help="Save exact match to this path (instead of overwriting)")
    parser.add_argument("--symbol",
                        help="Exact compiled symbol to score (defaults to DB mangled_symbol)")
    parser.add_argument("--no-gate", action="store_true",
                        help="Run even when the baseline is outside the last-mile gate")
    parser.add_argument("--sched", choices=["1", "2"], default=None,
                        help="Override -Xsched flag (default: auto-detect from filename)")
    parser.add_argument("--mopt", choices=["0"], default=None,
                        help="Add -Xmopt=0 flag")

    args = parser.parse_args()

    if not os.path.exists(args.source):
        print(f"Error: source file not found: {args.source}", file=sys.stderr)
        sys.exit(1)

    functions = load_db()

    func = find_function(functions, args.target)
    func_addr = int(func["address"], 16)
    func_size = func["size"]
    print(f"Target: {func['name']} at {func['address']} ({func_size}B)")
    target_symbol = args.symbol or func.get("mangled_symbol") or func.get("symbol_name")
    if target_symbol:
        print(f"Symbol: {target_symbol}")
    else:
        print("Symbol: <unique-size fallback>")

    if args.sched:
        sched_flag = [f"-Xsched={args.sched}"]
    else:
        sched_flag = _detect_sched(args.source)
    cflags = BASE_CFLAGS + sched_flag
    if args.mopt:
        cflags.append(f"-Xmopt={args.mopt}")
    print(f"Flags: {' '.join(cflags)}")

    with open(args.source, "r") as f:
        source = f.read()

    with open(EBOOT_PATH, "rb") as f:
        eboot_data = f.read()

    save_path = None
    if args.save_best:
        save_path = args.source
    elif args.save_to:
        save_path = args.save_to

    best_score, best_source, stats = run_search(
        source=source,
        func_addr=func_addr,
        func_size=func_size,
        cflags=cflags,
        eboot_data=eboot_data,
        time_limit=args.time,
        num_workers=args.workers,
        save_best_path=save_path,
        save_improved=args.save_improved,
        no_gate=args.no_gate,
        target_symbol=target_symbol,
    )

    if best_score == 0:
        print(f"\nSource {'saved to ' + save_path if save_path else '(not saved — use --save-best)'}")
        sys.exit(0)
    elif best_score < stats.get("baseline", float("inf")):
        print(f"\nImproved but not matched. "
              f"{'Saved to ' + save_path if save_path and args.save_improved else 'Use --save-improved to save non-exact results.'}")
        sys.exit(1)
    else:
        print("\nNo improvement found.")
        sys.exit(1)


if __name__ == "__main__":
    main()
