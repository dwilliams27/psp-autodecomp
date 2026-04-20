# 006: Branch-Likely (bnel) Compiler Divergence

**Date:** 2026-04-12
**Status:** Verified

## Finding

Our SNC (pspsnc 1.2.7503.0) and the original game compiler disagree on when to apply the branch-likely optimization (`bnel`/`beql` vs `bne`/`beq` + `nop`). Both compilers use `bnel` extensively — they have slightly different heuristics for **when** to apply it.

This creates a class of functions that are **structurally correct but unmatchable** with our current toolchain: the decompiled C is right, every instruction is right, but 2 instructions differ because our compiler fills a branch delay slot that the original left empty.

## Evidence

**gcStateVTableEntry::Set** (0x0010c684, 96B, gcAll_psp.obj):

Our SNC produces:
```
lw    t1, 48(a3)
bnel  t1, t0, .skip     # branch-likely: delay slot annulled if not taken
move  a3, a2             # delay slot: only executes when branch taken
move  a2, a3             # fall-through
```

Original EBOOT has:
```
lw    t1, 48(a3)
bne   t1, t0, .done     # plain branch: delay slot always executes
nop                      # wasted delay slot
move  a2, a3             # fall-through
```

Same logic, same instruction count (24 words = 96 bytes both ways), 5 bytes differ.

## Exhaustive elimination

Confirmed this is not controllable via source or flags:

**Flags tested (all produce bnel):**
- `-Xsched=1`, `-Xsched=2`
- `-Xmopt=0` through `-Xmopt=4`
- `-Xxopt=0` through `-Xxopt=5`
- `-Xfcm=0` through `-Xfcm=3`
- All combinations of the above

**Source restructurings tested (all produce bnel):**
- Merged `&&` conditions
- goto-based flow
- asm barriers before/after comparison
- Barrier inside if body
- Temp variable for comparison value
- Inverted condition with else branch
- Unconditional assignment after negated check

**Conclusion:** The bnel transform is hardwired in SNC 1.2.7503.0 at -O2 for this code pattern. No flag or source variation suppresses it.

## Scope of the problem

Scan of the full binary and our matched functions:

| Metric | Count |
|---|---|
| Functions in EBOOT with `bnel` | 2,802 / 8,562 (33%) |
| Matched functions with `bnel` | 22 / 206 (11%, all byte-exact) |

Our SNC correctly produces `bnel` for all 22 matched functions that need it. The compilers agree on bnel usage in the vast majority of cases. The disagreement on `gcStateVTableEntry::Set` is a rare edge case where the heuristics diverge.

## Impact

- **003 (compiler flags) unaffected.** The sched/mopt/xopt analysis remains valid. This is an optimizer heuristic difference, not a flag issue.
- **004 (finetune targets) unaffected.** The target strategy is sound — this is exactly the kind of discovery 004 was designed to surface.
- **Matching workflow:** Functions hitting this pattern should be marked `unmatchable_bnel` (not `failed`) so they aren't retried endlessly. See "Diagnostic criteria" below for how to identify them.

## Diagnostic criteria for bnel divergence

A function should ONLY be classified as unmatchable due to bnel if ALL of the following are true:

1. **The byte diff is exactly 5-8 bytes** (one bnel+delay-slot vs bne+nop = 2 words = 8 bytes max, but relocation masking may reduce the visible diff)
2. **The differing instructions are specifically `bnel`/`beql` vs `bne`/`beq` + `nop`** — confirmed via disassembly, not guessed
3. **The permuter was run for ≥5 minutes** with no improvement (eliminates source-level fixes)
4. **At least 3 source restructurings were manually tested** targeting the specific branch
5. **The function otherwise matches perfectly** — all other bytes are identical after relocation masking

If any of these criteria are not met, the function is **not confirmed unmatchable** and should remain `failed` for retry. In particular:
- Diffs >10 bytes are almost certainly NOT bnel-only issues
- Register allocation diffs (different register numbers, same instruction) are source-controllable
- Statement reordering diffs are source-controllable
- Agents must NOT use this classification to avoid hard work on functions that are merely difficult

---

## EDIT (2026-04-19): Confirmed and refined

This doc's core claim — that `gcStateVTableEntry::Set` has a real bnel-heuristic divergence — was verified via precise byte-level analysis on 2026-04-19.

**Confirmed details of the Set divergence:**

Byte-level diff of our compiled output vs original EBOOT bytes (96-byte function, all 24 instructions compared). Only 4 instructions differ:

| Offset | Ours | Original | Classification |
|--------|------|----------|----------------|
| 0x14 | `lui a3, 0x0` | `lui a3, 0x4` | Relocation (masked) |
| 0x1c | `addiu a3, a3, 0` | `addiu a3, a3, 0x8890` | Relocation (masked) |
| **0x34** | **`bnel t1, t0, +3`** | **`bne t1, t0, +2`** | **REAL: heuristic** |
| **0x38** | **`move a3, a2`** | **`nop`** | **REAL: delay slot** |

The real divergence matches decision 006's description exactly: our compiler emits `bnel + move a3, a2` where the original emits `bne + nop`.

**What this fits under Experiment F's rule set** (see decision 010): the delay-slot `move a3, a2` has **zero register overlap** with the branch's compare registers `{t1, t0}`. Per F, the original compiler's heuristic for this pattern is "74% regular, 26% likely" — so choosing regular is the expected majority behavior. Our compiler appears to always promote in this pattern (0% regular), missing a specific "no-overlap move" rejection rule.

**Clarification of scope**: Decision 010 section F originally reported "zero cases where we emit bnel and the original emits bne+nop in the matched corpus." That statement is technically correct but misleading — the analysis only examined the 462 matched `.o` files, which by definition cannot contain bnel divergences (they would not match). The divergence lives in the **40 failed** functions, which F's analysis did not cover. See the EDIT in decision 010 for the corresponding correction.

**Status of this doc:** the rule set, diagnostic criteria, and scope framing in the original body of this doc remain valid for the specific pattern it describes. The "rare edge case" characterization should be tempered — the pattern is rare per-branch but accumulates across functions.
