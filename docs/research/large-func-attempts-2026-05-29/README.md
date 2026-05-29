# Large-function attempts — 2026-05-29

Two large functions attacked serially by a workflow (recon → 5 parallel build agents →
2 reg-aware permuter campaigns each). **Neither reached byte-exact.** Both reconstructed to
the correct size; the `.cpp` files here are the best reconstructions, preserved for resumption.
These are NOT matches and are intentionally OUTSIDE `src/` so they are not compiled into the tree.
Both DB entries are marked `failed`.

The guide score below uses the new instruction-aware permuter metric
(`guide = 1000·struct + 1·register-locker` when size is exact); see
`tools/mips_score.py` and the 2026-05-28 session.

## T1 — `gcDoEntityCastRay::operator=` (0x00142c24, 4360B) → `gcDoEntityCastRay_operator_eq.cpp`

- Best: exact size 4360B, **raw 2032/4360**, guide 50434 ≈ **50 structural + 434 register-locker**.
- Reuses the cracked `gcDoEntitySpawn::operator=` recipe (the `gcDesiredValue` release/acquire
  dance, correct call signatures, per-arm `|1`), plus this class's extra members `gcEvent` and a
  `cArrayBase<gcDesiredEntityHelper>`.
- Residual is **register-allocation-dominated** (same class as gcDoEntitySpawn — e.g. the handle
  lands in `$a0` where the original uses `$a2` at 0x168), but ~50 genuine structural diffs remain.
- The reg-aware permuter made **zero** progress here: the ~50 structural diffs block it (it can
  shuffle registers/scheduling but cannot fix wrong operations). **Next:** close the ~50 structural
  diffs first (most likely in the `gcEvent` / `cArrayBase` member handling), reach 0 struct, THEN
  permute the register lockers — that is the order the permuter requires.

## T2 — `gcStreamedCinematic::gcStreamedCinematic(cBase *, bool)` (0x00161c00, 4108B) → `gcStreamedCinematic_ctor.cpp`

- Best: exact size 4108B, **raw 601/4108** (permuter improved it from 1096→601), guide 115171 ≈
  **115 structural + 171 register**.
- Constructor archetype: base-ctor call, vtable store, 31 callees, `gcDesiredValue` member inits.
- **First divergence is in the prologue:** the original saves `s0…ra` at `sp+64…88`; ours saves at
  `sp+56…80` — our **stack frame is 8 bytes (2 words) too small**, which cascades to every
  `sp`-relative offset and accounts for most of the structural diffs. The constructor reserves 2
  more stack locals than our reconstruction (a missing local/temp or a member-init that spills).
- The permuter helped raw (1096→601) but cannot change the frame size. **Next:** find the 2 missing
  stack words (compare the prologue + early body against the asm), then permute the remainder.

## Key learning (carried into tooling guidance)

The reg-aware permuter is a genuine **last-mile** tool: it needs a **structurally-complete** input
(guide structural component == 0) to be effective. With structural diffs present it correctly makes
no progress (T1). A useful future refinement: gate the permuter on the guide's *structural*
component (not raw bytes) so it only engages once a candidate is structurally exact. Both functions
here are harder than gcDoEntitySpawn (extra container/event members; a 31-callee constructor) and
need more reconstruction before the permuter can finish them.
