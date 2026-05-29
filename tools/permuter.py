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
import signal
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass

from common import (EBOOT_PATH, TEXT_FILE_OFFSET,
                    load_db, find_function,
                    mask_relocation_bytes)
from byte_match import extract_section, symbols_with_bytes_and_relocs
from mutations import mutate
from mips_score import score_instructions


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

_active_compile_proc = None


@dataclass
class CompileResult:
    ok: bool
    returncode: int | None
    stdout: str = ""
    stderr: str = ""
    timed_out: bool = False

    def diagnostic(self, max_chars=2000):
        parts = []
        if self.timed_out:
            parts.append("timed out after 30s")
        elif self.returncode is not None:
            parts.append(f"exit status {self.returncode}")
        for label, text in (("stdout", self.stdout), ("stderr", self.stderr)):
            text = (text or "").strip()
            if text:
                if len(text) > max_chars:
                    text = text[:max_chars] + "...<truncated>"
                parts.append(f"{label}: {text}")
        return "; ".join(parts) if parts else "no compiler diagnostic"


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


def _terminate_process_group(proc, term_timeout=2.0, kill_timeout=2.0):
    """Terminate a compiler process group without blocking forever."""
    if proc is None or proc.poll() is not None:
        return

    try:
        os.killpg(proc.pid, signal.SIGTERM)
    except ProcessLookupError:
        return

    try:
        proc.communicate(timeout=term_timeout)
        return
    except subprocess.TimeoutExpired:
        pass

    try:
        os.killpg(proc.pid, signal.SIGKILL)
    except ProcessLookupError:
        return

    try:
        proc.communicate(timeout=kill_timeout)
    except subprocess.TimeoutExpired:
        pass


def compile_source(src_path, o_path, cflags):
    """Compile a source file with SNC."""
    global _active_compile_proc
    cmd = [WIBO, SNC] + cflags + ["-o", o_path, src_path]
    proc = None
    try:
        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            start_new_session=True,
        )
        _active_compile_proc = proc
        stdout, stderr = proc.communicate(timeout=30)
        return CompileResult(
            ok=(proc.returncode == 0),
            returncode=proc.returncode,
            stdout=stdout,
            stderr=stderr,
        )
    except subprocess.TimeoutExpired:
        _terminate_process_group(proc)
        print(f"WARNING: SNC timed out (30s) on {src_path}", file=sys.stderr)
        return CompileResult(
            ok=False,
            returncode=None,
            stderr=f"SNC timed out (30s) on {src_path}",
            timed_out=True,
        )
    except KeyboardInterrupt:
        _terminate_process_group(proc, term_timeout=0.5, kill_timeout=0.5)
        raise
    finally:
        if _active_compile_proc is proc:
            _active_compile_proc = None


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


def score_candidate(compiled_bytes, expected_bytes, relocations, score_mode):
    """Score compiled bytes and return ``(rank_score, raw_score)``.

    raw_score is always the relocation-masked byte diff (the WIN CONDITION,
    identical to ``score_bytes``). rank_score is what the hill-climber ranks by:
      - bytes mode: rank_score == raw_score (byte-identical to historical
        behavior).
      - insns mode: rank_score == guide_score from ``score_instructions``, which
        masks the branch-displacement cascade so register-only near-misses get a
        usable gradient. raw_score still gates the exact-match declaration.

    Returns ``(float('inf'), float('inf'))`` for compile failures / size
    mismatches.
    """
    raw_score = score_bytes(compiled_bytes, expected_bytes, relocations)
    if score_mode == "bytes":
        return raw_score, raw_score
    if compiled_bytes is None:
        return float("inf"), float("inf")
    guide_score, raw_from_insns = score_instructions(
        compiled_bytes, expected_bytes, relocations)
    # score_instructions recomputes the same relocation-masked raw diff; keep
    # the score_bytes value as the authoritative win condition. They must agree
    # when both are finite (same masking path) — fail loud if not, per the
    # project's no-silent-fallback norm.
    if raw_score == float("inf"):
        raw_score = raw_from_insns
    elif raw_from_insns != float("inf"):
        assert raw_score == raw_from_insns, (
            f"raw diff disagreement: score_bytes={raw_score} "
            f"score_instructions={raw_from_insns}"
        )
    return guide_score, raw_score


def permuter_suitability_reason(func_size, symbol_size, baseline_diff,
                                score_mode="bytes", insn_gate=None):
    """Return skip reason when the target is not a last-mile permuter case.

    In bytes mode the gate is the raw differing-byte count (default 30B,
    byte-identical to historical behavior). In insns mode the gate is the
    instruction-distance guide score; raw byte diffs are not a meaningful gate
    there because a single register swap cascades branch offsets across the
    whole function, so ``insn_gate`` (a guide_score threshold) is used instead.
    """
    if symbol_size != func_size:
        delta = abs(symbol_size - func_size)
        return (
            f"symbol size {symbol_size}B does not match DB size "
            f"{func_size}B (delta {delta}B)"
        )
    if baseline_diff == float("inf"):
        return "baseline could not be scored"
    if score_mode == "insns":
        if insn_gate is None:
            return None
        if baseline_diff > insn_gate:
            return (
                f"baseline guide score {baseline_diff} exceeds instruction "
                f"gate {insn_gate}"
            )
        return None
    max_reasonable_diff = 30
    if baseline_diff > max_reasonable_diff:
        return (
            f"baseline diff {baseline_diff}B exceeds last-mile gate "
            f"{max_reasonable_diff}B"
        )
    return None


# ---------------------------------------------------------------------------
# Worker
# ---------------------------------------------------------------------------

def _worker_init(expected_bytes, func_size, base_cflags, target_sym, score_mode):
    """Initialize worker process globals."""
    global _w_expected, _w_func_size, _w_cflags, _w_target_sym, _w_score_mode
    _w_expected = expected_bytes
    _w_func_size = func_size
    _w_cflags = base_cflags
    _w_target_sym = target_sym
    _w_score_mode = score_mode
    signal.signal(signal.SIGTERM, _worker_sigterm)


def _worker_sigterm(signum, _frame):
    """Terminate any active compiler child before the worker exits."""
    _terminate_process_group(_active_compile_proc, term_timeout=0.5,
                             kill_timeout=0.5)
    raise SystemExit(128 + signum)


def _worker_eval(args):
    """Evaluate a single candidate.

    Returns (rank_score, raw_score, source, status). rank_score drives the
    hill-climb (guide_score in insns mode, raw byte diff in bytes mode);
    raw_score is always the relocation-masked byte diff and is the only thing
    that may declare an exact match.
    """
    source, suffix, seed, flag_override = args
    random.seed(seed)

    cflags = flag_override if flag_override else _w_cflags

    workdir = tempfile.mkdtemp(prefix="permuter_")
    src_path = os.path.join(workdir, "candidate" + suffix)
    o_path = src_path + ".o"

    try:
        with open(src_path, "w") as f:
            f.write(source)

        compile_result = compile_source(src_path, o_path, cflags)
        if not compile_result.ok:
            return float("inf"), float("inf"), source, "compile_fail"

        text_bytes = extract_text_bytes(o_path)
        try:
            func_bytes, relocs = get_symbol_bytes(
                o_path, text_bytes, _w_func_size, _w_target_sym
            )
        except SymbolSelectionError:
            return float("inf"), float("inf"), source, "symbol_fail"

        rank_score, raw_score = score_candidate(
            func_bytes, _w_expected, relocs, _w_score_mode)
        return rank_score, raw_score, source, "ok"
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
               save_best_path=None, save_improved=False, no_gate=False,
               score_mode="bytes", insn_gate=None):
    """Run the permuter search loop.

    Returns (best_raw_score, best_source, stats_dict). The hill-climber ranks by
    ``score_mode`` (raw byte diff in bytes mode, instruction guide score in
    insns mode), but EXACT MATCH is declared and saved only when the raw
    relocation-masked byte diff reaches 0 (the safety invariant).
    """
    if num_workers is None:
        num_workers = 2

    batch_size = num_workers * 2
    suffix = _detect_suffix(source)

    # Score the original
    with tempfile.NamedTemporaryFile(suffix=suffix, mode="w", delete=False) as f:
        f.write(source)
        orig_src_path = f.name
    orig_o_path = orig_src_path + ".o"

    try:
        compile_result = compile_source(orig_src_path, orig_o_path, cflags)
        if not compile_result.ok:
            raise RuntimeError(
                "Original source does not compile: "
                + compile_result.diagnostic()
            )

        text_bytes = extract_text_bytes(orig_o_path)
        selected_symbol, func_bytes, relocs = locate_symbol_bytes(
            orig_o_path, text_bytes, func_size, target_symbol
        )

        start_offset = func_addr + TEXT_FILE_OFFSET
        expected_bytes = eboot_data[start_offset:start_offset + func_size]
        baseline_rank, baseline_raw = score_candidate(
            func_bytes, expected_bytes, relocs, score_mode)
        baseline = baseline_rank
    finally:
        for p in [orig_src_path, orig_o_path]:
            try:
                os.unlink(p)
            except FileNotFoundError:
                pass

    stats = {
        "baseline": baseline,
        "baseline_raw": baseline_raw,
        "score_mode": score_mode,
        "total": 0,
        "compiled": 0,
        "compile_fail": 0,
        "symbol_fail": 0,
        "improvements": 0,
        "best_scores": [baseline],
        "selected_symbol": selected_symbol,
        "symbol_size": len(func_bytes),
        "saved": False,
    }

    # EXACT MATCH is governed by the raw relocation-masked byte diff only, even
    # in insns mode where the rank score (guide) can hit 0 on a branch-offset
    # residual. raw_score == 0 is the single win condition (safety invariant).
    if baseline_raw == 0:
        if save_best_path:
            with open(save_best_path, "w") as f:
                f.write(source)
            stats["saved"] = True
        print("Already an exact match!")
        stats["exact"] = True
        return 0, source, stats

    skip_reason = permuter_suitability_reason(
        func_size, len(func_bytes), baseline,
        score_mode=score_mode, insn_gate=insn_gate)
    if skip_reason and not no_gate:
        stats["skipped"] = skip_reason
        print(f"PERMUTER_SKIPPED_NOT_LAST_MILE: {skip_reason}")
        return baseline_raw, source, stats

    if baseline == float("inf"):
        raise RuntimeError(
            "Could not score original source against EBOOT. "
            "Symbol size mismatch or relocation error."
        )

    score_label = "guide score" if score_mode == "insns" else "bytes differ"
    print(f"Target symbol: {selected_symbol}")
    print(f"Score mode: {score_mode}")
    if score_mode == "insns":
        print(f"Baseline: {baseline} {score_label} / {baseline_raw} bytes differ "
              f"({func_size}B function)")
    else:
        print(f"Baseline: {baseline} {score_label} ({func_size}B function)")
    print(f"Workers: {num_workers}  |  Time limit: {time_limit}s")
    print()

    # best_rank drives the hill-climb; best_raw is the win condition. In bytes
    # mode they are identical.
    best_rank = baseline
    best_raw = baseline_raw
    best_source = source

    start_time = time.time()
    last_print = start_time

    # Pass only the sliced expected bytes to workers, not the full EBOOT
    start_offset = func_addr + TEXT_FILE_OFFSET
    expected_slice = eboot_data[start_offset:start_offset + func_size]

    pool = multiprocessing.Pool(
        num_workers,
        initializer=_worker_init,
        initargs=(expected_slice, func_size, cflags, target_symbol, score_mode),
    )

    def _fmt_best():
        if score_mode == "insns":
            return f"{best_rank}i/{best_raw}b"
        return str(best_raw)

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

            for rank_sc, raw_sc, src, status in pool.imap_unordered(
                    _worker_eval, candidates):
                stats["total"] += 1

                if status == "compile_fail":
                    stats["compile_fail"] += 1
                    continue
                if status == "symbol_fail":
                    stats["symbol_fail"] += 1
                    continue
                stats["compiled"] += 1

                # Lexicographic (guide, raw): a candidate that ties the guide
                # but lowers raw still counts as progress. This is REQUIRED for
                # correctness: the guide masks branch displacements, so it can
                # hit 0 on a branch-offset-only residual before a true raw==0
                # match arrives. A plain `rank_sc < best_rank` would then drop
                # the genuine exact match (0 not < 0). Tracking raw as the
                # tiebreak guarantees the win condition is never discarded.
                if (rank_sc, raw_sc) < (best_rank, best_raw):
                    prev_best = _fmt_best()
                    best_rank = rank_sc
                    best_raw = raw_sc
                    best_source = src
                    stats["improvements"] += 1
                    stats["best_scores"].append(best_rank)

                    elapsed = time.time() - start_time
                    print(f"  [{elapsed:6.1f}s] Improved: {_fmt_best()} "
                          f"(was {prev_best})")

                    # Save only on a true exact match (raw == 0), or on any
                    # rank improvement when explicitly requested.
                    if save_best_path and (best_raw == 0 or save_improved):
                        with open(save_best_path, "w") as f:
                            f.write(best_source)
                        stats["saved"] = True

                    if best_raw == 0:
                        elapsed = time.time() - start_time
                        print(f"\n  EXACT MATCH found in {elapsed:.1f}s "
                              f"after {stats['total']} candidates!")
                        stats["exact"] = True
                        return 0, best_source, stats

            # Periodic status
            now = time.time()
            if now - last_print >= 10:
                elapsed = now - start_time
                rate = stats["compiled"] / elapsed if elapsed > 0 else 0
                print(f"  [{elapsed:6.1f}s] {stats['compiled']}/{stats['total']} compiled  "
                      f"| best={_fmt_best()}  | {rate:.1f} candidates/sec")
                last_print = now

    finally:
        pool.terminate()
        pool.join()

    elapsed = time.time() - start_time
    rate = stats["compiled"] / elapsed if elapsed > 0 else 0

    print()
    print(f"Search complete: {elapsed:.1f}s elapsed")
    print(f"  Candidates: {stats['total']} generated, {stats['compiled']} compiled")
    print(f"  Failures: {stats['compile_fail']} compile, "
          f"{stats['symbol_fail']} symbol isolation")
    print(f"  Rate: {rate:.1f} candidates/sec")
    print(f"  Best: {_fmt_best()} (started at "
          f"{baseline if score_mode == 'bytes' else f'{baseline}i/{baseline_raw}b'})")
    print(f"  Improvements: {stats['improvements']}")
    stats["exact"] = False
    stats["best_raw"] = best_raw

    # Return the raw byte diff as the primary score so callers' "== 0 means
    # exact" contract holds regardless of score mode.
    return best_raw, best_source, stats


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def _print_result(best_score, stats, save_path=None, saved=False):
    baseline = stats.get("baseline", float("inf"))
    score_mode = stats.get("score_mode", "bytes")
    skipped = stats.get("skipped")
    selected_symbol = stats.get("selected_symbol", "<unknown>")
    fields = [
        f"score_mode={score_mode}",
        f"baseline={baseline}",
        f"baseline_raw={stats.get('baseline_raw', baseline)}",
        f"best={best_score}",
        f"total={stats.get('total', 0)}",
        f"compiled={stats.get('compiled', 0)}",
        f"compile_fail={stats.get('compile_fail', 0)}",
        f"symbol_fail={stats.get('symbol_fail', 0)}",
        f"improvements={stats.get('improvements', 0)}",
        f"exact={str(best_score == 0).lower()}",
        f"saved={str(saved).lower()}",
        f"symbol={selected_symbol}",
    ]
    if skipped:
        fields.append(f"skipped={skipped!r}")
    if save_path:
        fields.append(f"save_path={save_path}")
    print("PERMUTER_RESULT " + " ".join(fields))


def run_main():
    parser = argparse.ArgumentParser(
        description="SNC-aware source permuter for last-mile byte matching"
    )
    parser.add_argument("source", help="Source file to permute")
    parser.add_argument("target",
                        help="Function address (0x...) or name (e.g. eTextureMap::Read)")
    parser.add_argument("--time", type=int, default=300,
                        help="Time limit in seconds (default: 300)")
    parser.add_argument("--workers", type=int, default=None,
                        help="Number of parallel workers (default: 2)")
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
    parser.add_argument("--score-mode", choices=["bytes", "insns"], default="bytes",
                        help="Ranking objective: 'bytes' (default, raw byte diff, "
                             "behavior identical to historical permuter) or 'insns' "
                             "(instruction-aware guide score that masks the "
                             "branch-displacement cascade for REG_ALLOC near-misses). "
                             "Exact match is always declared on raw byte diff == 0.")
    parser.add_argument("--insn-gate", type=int, default=None,
                        help="In --score-mode insns, skip targets whose baseline guide "
                             "score exceeds N (replaces the raw 30B gate, which wrongly "
                             "blocks register-cascade cases). Default: no instruction gate.")
    parser.add_argument("--sched", choices=["1", "2"], default=None,
                        help="Override -Xsched flag (default: auto-detect from filename)")
    parser.add_argument("--mopt", choices=["0"], default=None,
                        help="Add -Xmopt=0 flag")
    parser.add_argument("--fprreserve", type=int, default=None,
                        help="Add -Xfprreserve=N flag")
    parser.add_argument("--gprreserve", type=int, default=None,
                        help="Add -Xgprreserve=N flag")
    parser.add_argument("--vfpumatrix", type=int, default=None,
                        help="Add -Xvfpumatrix=N flag")
    parser.add_argument("--vfpuscalar", type=int, default=None,
                        help="Add -Xvfpuscalar=N flag")
    parser.add_argument("--debug-traceback", action="store_true",
                        help="Print Python traceback for permuter tool errors")

    args = parser.parse_args()

    if args.save_improved and not (args.save_best or args.save_to):
        parser.error("--save-improved requires --save-best or --save-to")

    if args.insn_gate is not None and args.score_mode != "insns":
        parser.error("--insn-gate only applies with --score-mode insns")

    if not os.path.exists(args.source):
        print(f"PERMUTER_ERROR: source file not found: {args.source}",
              file=sys.stderr)
        return 1

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
    if args.fprreserve is not None:
        cflags.append(f"-Xfprreserve={args.fprreserve}")
    if args.gprreserve is not None:
        cflags.append(f"-Xgprreserve={args.gprreserve}")
    if args.vfpumatrix is not None:
        cflags.append(f"-Xvfpumatrix={args.vfpumatrix}")
    if args.vfpuscalar is not None:
        cflags.append(f"-Xvfpuscalar={args.vfpuscalar}")
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

    best_score, _best_source, stats = run_search(
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
        score_mode=args.score_mode,
        insn_gate=args.insn_gate,
    )

    saved = bool(stats.get("saved", False))
    _print_result(best_score, stats, save_path=save_path, saved=saved)

    # best_score is the raw byte diff regardless of mode; compare against the
    # raw baseline so "improved" reflects a real byte-level gain.
    baseline_raw = stats.get("baseline_raw", stats.get("baseline", float("inf")))
    if best_score == 0:
        print(f"\nSource {'saved to ' + save_path if save_path else '(not saved — use --save-best)'}")
        return 0
    elif best_score < baseline_raw:
        print(f"\nImproved but not matched. "
              f"{'Saved to ' + save_path if save_path and args.save_improved else 'Use --save-improved to save non-exact results.'}")
        return 1
    else:
        print("\nNo improvement found.")
        return 1


def main():
    def _sigterm(signum, _frame):
        _terminate_process_group(_active_compile_proc, term_timeout=0.5,
                                 kill_timeout=0.5)
        print(f"PERMUTER_ERROR: terminated by signal {signum}", file=sys.stderr)
        raise SystemExit(128 + signum)

    signal.signal(signal.SIGTERM, _sigterm)
    try:
        return run_main()
    except SystemExit:
        raise
    except KeyboardInterrupt:
        _terminate_process_group(_active_compile_proc, term_timeout=0.5,
                                 kill_timeout=0.5)
        print("PERMUTER_ERROR: interrupted", file=sys.stderr)
        return 130
    except Exception as exc:
        if "--debug-traceback" in sys.argv:
            raise
        print(f"PERMUTER_ERROR: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
