# 013: Permuter Register-Aware Scoring

**Date:** 2026-05-29
**Status:** Implemented (commit 83e3304). Extends ADR-005 (the permuter, now implemented).

## Decision

Add an instruction-aware hill-climbing objective to `tools/permuter.py` so it can close
register-allocation / scheduling near-misses, not just small byte gaps. New `--score-mode insns`
(default stays `bytes`, byte-identical to prior behavior).

## Why

The raw byte-diff objective has **no gradient** on register-spelling near-misses. One repeated
register swap (e.g. a tagged handle landing in `$t1` where the original used `$t0`) cascades into
thousands of differing bytes via shifted branch displacements, even though the real gap is a handful
of instructions. The hill-climber could not tell a good mutation from noise, so it never engaged on
the whole `REG_ALLOC` failure class (`docs/research/snc-register-allocation.md`, ADR-012). Example:
`gcDoEntitySpawn::operator=` reads **4142** raw differing bytes for a ~6-instruction-per-dance
register residual; `bytes` mode found only size-changing junk (`inf`).

## Design

New module `tools/mips_score.py`: a little-endian MIPS32 decoder + `score_instructions(compiled,
expected, relocs) -> (guide_score, raw_score)`.

- **`guide_score`** (ranking objective). Decode both streams; **mask the cascade** — relocation
  bytes (reused `mask_relocation_bytes`) AND the displacement/target field of branch/jump
  instructions (a branch is "same" if opcode + register operands match, ignoring how far it jumps).
  **Keep** load/store offsets and ALU immediates (real signal). Align the streams (1:1 index, with
  an LCS realignment over opcode keys **only when** 1:1 shows structural diffs — recovers local
  insert/delete shifts). Weighted sum:
  `guide = 10000·|Δlen| + 1000·(structural diffs) + 1·(register-only "locker" diffs)`.
  `guide == 0` iff structurally identical with correct registers (branch displacements aside).
- **`raw_score`** — the relocation-masked raw byte diff (== `score_bytes`). **The win condition.**
- **Conservative decoder.** Only opcodes confidently known to bear GPRs (R-type, the explicit GPR
  I-type / load-store / branch / regimm sets) get their register fields masked as lockers. Unknown,
  FP, and VFPU opcodes (`lwc1/swc1`, Allegrex `lv.q/sv.q`, etc.) fall to `KIND_OTHER` and are
  compared **full-word** — their register fields are never silently equated as cheap lockers.

### SAFETY INVARIANT
Because the guide masks branch displacements, `guide == 0` can occur while `raw > 0` (a
branch-offset-only residual). The search therefore **ranks by guide but declares/saves an EXACT
match only on `raw == 0`.** Acceptance is lexicographic `(guide, raw)` so a true `raw == 0`
candidate that ties an already-0 guide is never discarded. `score_bytes` vs `score_instructions`
raw agreement is asserted (fail-loud).

### Mutation reach
`score_instructions` only helps if the mutator can *produce* a locker-flipping variant. Added
`tools/mutations_regalloc.py` — the allocation-moving transforms the prior set lacked:
introduce/inline temporary, **duplicate-local (live-range split)**, sink/hoist a computation,
rename a local. Registered into the `mutate()` pool.

### CLI
`--score-mode {bytes,insns}` (default `bytes`), `--insn-gate N` (instruction-distance gate for
`insns` mode, replacing the raw 30B last-mile gate). `--no-gate` unchanged.

## Validation (12-agent build + 7-way validation, then review-driven fixes)

- Decoder validated against `objdump`: 0 mismatches over 4000+ instructions. 24 targeted tests pass;
  `bytes` mode byte-identical to before.
- `gcDoEntitySpawn::operator=`: guide 83519 vs raw 4142; a short `insns` run climbs where `bytes`
  found nothing.
- `gcStreamedCinematic::gcStreamedCinematic`: permuter improved raw **1096 → 601**.

### Key finding: it is genuinely LAST-MILE
The reg-aware permuter needs a **structurally-complete** input (the guide's *structural* component
== 0) to be effective — it shuffles registers/scheduling, it cannot fix wrong operations. On
`gcDoEntityCastRay::operator=` (~50 structural diffs remaining) it made **zero** progress, correctly.
So: reconstruct to structural exactness *first* (size exact, 0 structural diffs), *then* permute the
register lockers.

**Recommended future refinement:** gate the permuter on the guide's structural component (not raw
bytes), so it only engages once a candidate is structurally exact and reports clearly when it is not.

## Files
`tools/mips_score.py`, `tools/mutations_regalloc.py`, `tools/test_mips_score.py` (new);
`tools/permuter.py`, `tools/mutations.py` (extended). See `docs/sessions/2026-05-29.md`.

## Related
- ADR-005 / 005a — the original permuter and SNC codegen mutation experiments.
- ADR-012, `docs/research/snc-register-allocation.md` — the REG_ALLOC failure class this targets.
- `docs/research/large-func-attempts-2026-05-29/` — the two functions where it was first used in anger.
