#!/usr/bin/env python3
"""SNC-aware source permuter for last-mile byte matching.

Mechanically closes byte gaps by randomly mutating C source and comparing
compiled output against original EBOOT bytes. Designed for near-miss functions
where agents get the structure right but can't nail exact instruction ordering
or register allocation.

Usage:
    python3 tools/permuter.py src/eTextureMap.cpp 0x0007ae64 --time 600
    python3 tools/permuter.py src/eTextureMap.cpp 0x0007ae64 --workers 8 --save-best
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
                    OBJCOPY, NM,
                    load_db, find_function,
                    get_text_relocations, mask_relocation_bytes)
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
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as tmp:
        bin_path = tmp.name
    try:
        result = subprocess.run(
            [OBJCOPY, "-O", "binary", "-j", ".text", o_path, bin_path],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            return None
        with open(bin_path, "rb") as f:
            return f.read()
    finally:
        os.unlink(bin_path)


def get_symbol_bytes(o_path, text_bytes, target_size, target_symbol=None, warn=False):
    """Extract the target function's bytes from the compiled .o.

    If .text is exactly target_size, returns all of it (fast path).
    Otherwise uses nm to find the right symbol.

    Returns (func_bytes, relocations) or (None, None).
    """
    if text_bytes is None:
        return None, None

    # Fast path: single function, size matches
    if len(text_bytes) == target_size:
        relocs = get_text_relocations(o_path)
        return text_bytes, relocs

    # Multi-function .o — find the right symbol via nm
    result = subprocess.run([NM, o_path], capture_output=True, text=True)
    if result.returncode != 0:
        if warn:
            print(f"WARNING: nm failed on {o_path}: {result.stderr.strip()}", file=sys.stderr)
        return None, None

    symbols = []
    for line in result.stdout.strip().split("\n"):
        parts = line.split()
        if len(parts) == 3 and parts[1] == "T":
            symbols.append((int(parts[0], 16), parts[2]))
    symbols.sort()

    text_size = len(text_bytes)
    layout = []
    for j, (off, name) in enumerate(symbols):
        next_off = symbols[j + 1][0] if j + 1 < len(symbols) else text_size
        size = next_off - off
        layout.append((off, size, name))

    # Find target by symbol name or by size
    match = None
    if target_symbol:
        for off, size, name in layout:
            if name == target_symbol:
                match = (off, size, name)
                break
    if match is None:
        size_matches = [(off, sz, n) for off, sz, n in layout if sz == target_size]
        if len(size_matches) == 1:
            match = size_matches[0]
        elif len(size_matches) > 1:
            if warn:
                print(f"WARNING: {len(size_matches)} symbols of size {target_size} in {o_path}; "
                      f"picking {size_matches[0][2]}. Use --symbol to disambiguate.",
                      file=sys.stderr)
            match = size_matches[0]
    if match is None:
        return None, None

    sym_off, sym_size, _ = match
    func_bytes = text_bytes[sym_off:sym_off + sym_size]

    # Adjust relocations to be function-relative
    all_relocs = get_text_relocations(o_path)
    func_relocs = [(off - sym_off, rtype) for off, rtype in all_relocs
                   if sym_off <= off < sym_off + sym_size]

    return func_bytes, func_relocs


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
        func_bytes, relocs = get_symbol_bytes(
            o_path, text_bytes, _w_func_size, _w_target_sym
        )

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
               save_best_path=None):
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

        func_bytes, relocs = get_symbol_bytes(
            orig_o_path, text_bytes, func_size, target_symbol, warn=True
        )
        if func_bytes is None:
            raise RuntimeError(
                f"Could not locate target function in compiled .o. "
                f"Found {len(text_bytes)}B .text but expected {func_size}B function."
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

    if baseline == 0:
        print("Already an exact match!")
        return 0, source, {"baseline": 0, "total": 0, "improvements": 0}

    if baseline == float("inf"):
        raise RuntimeError(
            "Could not score original source against EBOOT. "
            "Symbol size mismatch or relocation error."
        )

    print(f"Baseline: {baseline} bytes differ ({func_size}B function)")
    print(f"Workers: {num_workers}  |  Time limit: {time_limit}s")
    print()

    best_score = baseline
    best_source = source

    stats = {
        "baseline": baseline,
        "total": 0,
        "compiled": 0,
        "improvements": 0,
        "best_scores": [baseline],
    }

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

                    if save_best_path:
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
                        help="Overwrite source file with best result")
    parser.add_argument("--save-to",
                        help="Save best result to this path (instead of overwriting)")
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
    )

    if best_score == 0:
        print(f"\nSource {'saved to ' + save_path if save_path else '(not saved — use --save-best)'}")
        sys.exit(0)
    elif best_score < stats.get("baseline", float("inf")):
        print(f"\nImproved but not matched. "
              f"{'Saved to ' + save_path if save_path else 'Use --save-best to save.'}")
        sys.exit(1)
    else:
        print("\nNo improvement found.")
        sys.exit(1)


if __name__ == "__main__":
    main()
