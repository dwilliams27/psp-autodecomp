# SNC Register-Allocation Drift

**Status:** confirmed instability  
**Related:** `docs/decisions/012-snc-context-dependent-regalloc.md`, `docs/postmortems/2026-05-05-overnight-dense.md`

## Summary

SNC's general-purpose register allocator is deterministic but context-sensitive. Small changes elsewhere in the same translation unit can change tie-breaks in the allocator's interference graph, causing the same source-level function shape to assign values to different callee-save registers.

The result is usually an `s0`/`s1`/`s2` rename, not a real instruction-selection difference. Source perturbations can move the symptom around, but once the function is in this state there is usually no local source leverage.

ADR-012 documented the broader mechanism: the original `eAll_psp.obj` compiled thousands of engine functions in one translation unit, while this repo compiles smaller per-class or split translation units. SNC's allocator decisions depend on that surrounding TU context.

## Diagnostic Pattern

Treat a diff as register-allocation drift when all of these are true:

- The byte diff is small, typically 8 bytes or less after relocation masking.
- The differing instructions are otherwise identical; only register fields changed.
- The differences are concentrated in callee-save register naming:
  - `sw sN, off(sp)` prologue saves
  - `lw sN, off(sp)` epilogue restores
  - `move sN, sM`
  - arithmetic/logical instructions where only the `s0`/`s1`/`s2` field differs
- Control flow, stack size, calls, immediates, and memory offsets already match.
- A short permuter run does not improve the diff.

If you see this pattern, do not grind it manually — manual reorderings can damage matched siblings
(see "What Not To Do" below). The mechanical recourse is now the **register-aware permuter** (ADR-013):

```
python3 tools/permuter.py <file.cpp> 0xADDR --time 1800 --score-mode insns --no-gate --workers 8
```

`--score-mode insns` masks the branch-offset cascade that hid the gradient under the old byte-diff
objective, so register/scheduling lockers become climbable. **Precondition:** the candidate must be
**structurally complete first** — exact size and zero *structural* diffs (the permuter shuffles
registers/scheduling, it cannot fix wrong operations). Check with a short `insns` run: the printed
guide score is `1000·(structural diffs) + (register lockers)` at exact size, so a guide under ~1000
means structurally clean and worth a long campaign; a large structural component means finish the
reconstruction before permuting. If a long `insns` campaign makes no progress on a structurally-clean
candidate, record `category=REG_ALLOC` in the failure notes and stop.

## Examples

Known examples from the 2026-05-05 overnight run and ADR-012:

- `cFactory::MarkForClean` (`0x0000bbd4`): repeated 7-byte `s1`/`s2` register swap. Two independent sessions reproduced the same dead end and the permuter made no progress.
- `cFactory::CleanGroups` (`0x0000b648`): sibling of `MarkForClean` with the same allocation swap pattern.
- `gcViewport::UpdateAll` (`0x000fdd44`): known `s`-register rename drift in a close-but-not-match function.
- `gcViewport::UpdateUI` (`0x000fdddc`): sibling register-allocation instability.
- `eDynamicModel::SetSkin` (`0x00047f80`): emitted bytes drift by 8-30 bytes depending on TU/header context; also exposed symbol-mangling drift.
- `eDynamicModel::SetGeomFlagsOnOff` sibling effects: adding method declarations to `include/eDynamicModel.h` perturbed allocator choices for already-matched functions in `src/eDynamicModel.cpp`.
- Shape `Collide` dispatchers in ADR-012: identical source produces different register allocation when compiled in different TU contexts.

## What Not To Do

Do not keep trying variable declaration reorderings, cast changes, temporary splits, or memory barriers once the diff is only callee-save register naming. Those source edits can perturb unrelated siblings in the same file and are exactly how matched functions were damaged in the 2026-05-05 run.

Do not add method declarations to canonical headers to make a new class method compile. Use the split-TU pattern:

- Create `src/<Class>_<Method>.cpp`.
- Locally redeclare the class with only the methods this TU references.
- Add an `ODR-WARNING` comment at the top.
- Do not include or edit the canonical `include/<Class>.h` unless the target truly needs data layout declarations, not new methods.

## Classification

Use ordinary `failed` status, with notes that include `category=REG_ALLOC`, the address, the first differing instruction, and whether one permuter pass was run.

Do not mark these as `unmatchable_bnel`; branch-likely divergence is a different compiler behavior documented in `docs/decisions/006-bnel-compiler-divergence.md`.
