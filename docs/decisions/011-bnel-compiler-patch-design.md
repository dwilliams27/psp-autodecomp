# 011: Bnel Compiler Patch — Design Spec (Deferred)

**Date:** 2026-04-19
**Status:** Designed, not implemented. Deferred per ROI analysis.

## TL;DR

A surgical binary patch to `extern/snc/pspcor.exe` can eliminate the bnel-heuristic divergence documented in decisions 006 and 010. The patch adds one new failure condition to the `del_slot_can_fill` eligibility checker at VA `0x43afbc`, rejecting branch-likely promotion when the delay-slot candidate is a "scheduling filler" move with no register overlap with the branch's compare registers.

**Current estimated impact**: 2-7 additional matches among the 40 currently-failed functions. Confirmed cases: `gcStateVTableEntry::Set`, `cOutStream::WriteBits` (mixed). Most failed functions are not bnel-divergent — they fail from source-structure, VFPU, or size-mismatch issues that this patch does not address.

**Decision**: Deferred. Implement later if/when:
- The source-quality improvements (`docs/enhancements.md` #1, #2) stop yielding new matches, AND
- Bnel divergence becomes a meaningful fraction of remaining failures (>10-15%), OR
- Match count plateaus and we want "bulletproof into the future" coverage.

## Background

See `docs/decisions/006-bnel-compiler-divergence.md` for the original discovery of the divergence and `docs/decisions/010-compiler-internals-experiments.md` for the seven parallel experiments that mapped the compiler internals.

## Target function

`del_slot_can_fill` at VA `0x43afbc` in `pspcor.exe`. 387 bytes, ends at `0x43b13f`. This is the function `fmds_driver` (and other delay-slot fillers) calls to decide whether a candidate delay-slot instruction is safe to move into a branch's delay slot. Returning 1 allows the promotion from `bne` to `bnel`; returning 0 prevents it.

Current checks (all gated by verbosity `[0x4f8044] > 1` for their debug strings):

| Check | VA | Tests | Fail path |
|-------|-----|-------|-----------|
| store/call (low) | `0x43afe7` | `MOP_info[op]+0x24 & 0x40` | `0x43b11e` |
| store/call (high) | `0x43aff2` | bit 9 of same word (ah & 2) | `0x43b11e` |
| load | `0x43affb` | `MOP_info[op]+0x24 & 0x20` | `0x43b11e`* |
| float op (flag) | `0x43b016` | `MOP_info[op]+0x28 & 0x40` | `0x43b10e` |
| float op (vreg scan) | `0x43b020..0x43b079` | operand[i]→type & 0x4000 | `0x43b10e` |
| div/mult | `0x43b07a..0x43b093` | opcode in {0x352, 0x354, 0x358, 0x359} | `0x43b0fe` |
| target-reg bitmap | `0x43b095..0x43b0e7` | cand's dst reg vs reaching-defs bitmap | `0x43b139` |

Success return: `0x43b0fb` (`xor eax,eax; inc eax; jmp 0x43b13b` → returns 1).
Failure return: `0x43b139` (`xor eax,eax; ret` → returns 0).
Shared debug printer: `0x43b12c` (`push [0x4edf10]; call 0x4a2620; pop;pop; xor eax,eax; ret`).

## Gap: the missing check

Per Experiment F's statistical analysis of 75,365 branches in the original EBOOT:

| Delay-slot pattern | Original's likely rate |
|--------------------|----------------------:|
| move with no cmp-register overlap | **~26%** (74% regular) |
| alu writes cmp_reg | **3.2%** |
| alu reads cmp_reg | **1.7%** |
| store with src=cmp_reg | **0.8%** |

Our compiler currently promotes in all of these cases ~100% of the time. The most common divergence (and the one that hits `gcStateVTableEntry::Set`) is the "move with no cmp-register overlap" case.

The check our compiler is missing, in pseudocode:

```c
if (is_scheduling_filler(cand->ins->op)          // move, or reg,reg,zero; addu reg,reg,zero; ori reg,reg,0; etc.
    && cand->dst != branch->rs
    && cand->dst != branch->rt
    && cand->src != branch->rs
    && cand->src != branch->rt) {
    if (verbose > 1) dprintf("  FAILED because move w/o cmp overlap\n");
    return 0;
}
```

## Patch options

### Option α — Full rule (~55-70 bytes, trampoline)

Implements the full "scheduling-filler move with no cmp overlap → reject" rule. Requires a trampoline because there is no slack near `del_slot_can_fill`.

**Technique**:
1. Overwrite 5 bytes at `0x43b095` (`8b 73 40 39 74`) with `e9 rel32` (5-byte jmp to trampoline).
2. Append trampoline to end of `.text`. Section currently ends at `0x4ae596` (SizeOfCode `0xae000`). Bump VirtualSize and SizeOfRawData to fit a new ~64-byte block.
3. Trampoline code:
   - Re-execute the overwritten instructions (`mov esi,[ebx+0x40]; cmp [esp+0x14],esi`).
   - New check: test if `cand->op` is in the "scheduling filler" set (move/or/addu-with-zero/ori/addi-with-zero).
   - New check: test if `cand->dst` (word at `cand.ins+0x1c`) equals `branch->rs` or `branch->rt`.
   - New check: same for `cand->src`.
   - If filler AND no overlap → jump to `0x43b12c` with new debug string "FAILED because move w/o cmp overlap".
   - Otherwise jump back to `0x43b09a` (`jne 0x43b0a1` — the original next instruction after the overwritten 5 bytes).

**Patch file offset**: `0x43b095 - 0x401000 + 0x1000 = 0x3b095` (verified via PE section parse).

**Risk**: Section extension must update PE section headers (update VirtualSize, SizeOfRawData, and the PE SizeOfCode field). Moderate risk if header math is wrong — compiler may refuse to run or produce corrupted output.

### Option β — Narrower rule (~16 bytes, in-place)

Extends the existing target-reg bitmask check at `0x43b095..0x43b0e7` to treat `{branch.rs, branch.rt}` as additional reaching-def bits. This rejects any candidate whose dst register aliases the compare register — a **strict superset** of F's "alu writes cmp" and "move with dst=cmp" rules but MISSES the "move with no overlap" case.

**Will NOT fix `gcStateVTableEntry::Set`** (no register overlap).

Not recommended as a standalone — use only as a complement to α if it turns out the narrower fix catches enough functions.

### Option γ — Disable bnel entirely (1 byte)

Flip `0x43bc13` byte from `20` → `00` (OR `0x43ba4b` similarly). Globally disables branch-likely promotion.

**REGRESSES all 22 existing bnel matches.** Would make them permanently unmatchable (no C source can produce bnel with this compiler). Not recommended.

### Rejected option: `#pragma control sched=0`

Validated on 2026-04-19: too aggressive. Disables ALL delay-slot filling, inflating function size. Cannot produce byte-exact match with original (which was compiled with scheduling on).

### Rejected option: redirect the `0x41d90c` stub to `0x41d910`

The 3-byte stub at `0x41d90c` always returns 1; `0x41d910` is a 172-byte function nearby. Hypothesis: stubbed-out original check. **Disproven** on 2026-04-19:
- `0x41d910` is actively called by 4 other delay-slot fillers with a different arg2 layout.
- Its opcode range (`0x395..0x466`) doesn't overlap `del_slot_can_fill`'s input range (`0x32e..0x34c`).
- Redirecting would dereference wrong-typed memory and always-fail the opcode check, effectively disabling bnel promotion entirely (regressing the 22 matches).

See `docs/decisions/010-compiler-internals-experiments.md` EDIT section for details.

## Implementation plan (when resumed)

### Phase 1 — Binary patch scaffolding

1. Create `tools/patch_pspcor.py` that takes an input pspcor.exe, applies the patch, and writes patched output. Should be idempotent + produce a `.patched.exe` sibling.
2. Store patched compiler at `extern/snc/pspcor.patched.exe`. Do NOT replace `pspcor.exe`.
3. Add Makefile variable `SNC_COR = extern/snc/pspcor.exe` (or `.patched.exe`) switchable via env var `USE_PATCHED_PSPCOR=1`.

### Phase 2 — Trampoline code

Write the trampoline in a raw Python `bytes` literal. Structure:

```python
# After 5-byte jmp from 0x43b095 lands here:
TRAMPOLINE = bytes([
    # Re-execute overwritten: mov esi,[ebx+0x40]; cmp [esp+0x14],esi
    0x8b, 0x73, 0x40,                                   # mov esi, [ebx+0x40]
    0x39, 0x74, 0x24, 0x14,                              # cmp [esp+0x14], esi

    # New check 1: is cand an "opcode filler"?
    # cand is in edi; cand->ins at [edi+8]; op at [[edi+8]+0]
    0x8b, 0x47, 0x08,                                    # mov eax, [edi+8]
    0x0f, 0xb7, 0x00,                                    # movzx eax, word ptr [eax]
    0x3d, 0x__, 0x__, 0x00, 0x00,                        # cmp eax, MOP_move  (tbd: specific opcode value)
    0x74, 0x__,                                          # je check_regs
    # ...additional opcode checks for or/addu-with-zero/ori/lui-zero...

    # If NOT a filler, jump back to original flow
    0xe9, 0x__, 0x__, 0x__, 0x__,                        # jmp 0x43b09a (just after overwritten bytes)

    # check_regs: verify cand.dst and cand.src don't overlap with branch.rs/rt
    # branch->ins at [ebp+0xc]->ins (need to derive — see Experiment A for arg layout)
    # cand.dst at cand.ins+0x1c (word array of operand regs)
    # ...

    # If no overlap → push debug string, jump to 0x43b12c
    0x68, 0x__, 0x__, 0x__, 0x__,                        # push "FAILED because move w/o cmp overlap\n"
    0xe9, 0x__, 0x__, 0x__, 0x__,                        # jmp 0x43b12c
])
```

Exact byte values need to be filled in after:
- Identifying the numeric MOP codes for `move`/`or`/`addu`/`ori` with zero. Look up in the MOP descriptor table at `0x4e1fe0` via the string-to-function index at `/tmp/pspcor_string_index.json`.
- Confirming the instruction-struct layout offsets for dst/src register numbers. `+0x2a` is dst (word), `+0x1c+` is src array per Experiment A findings. Verify by disassembling existing reg-access code in `del_slot_can_fill`.

### Phase 3 — PE section header patch

To extend `.text`:
1. Parse the PE header. Find the section table at offset `PE_start + 4 + 20 + SizeOfOptionalHeader`.
2. Locate the `.text` section (first section, usually).
3. Bump `VirtualSize` and `SizeOfRawData` by the trampoline size (rounded up to FileAlignment, typically 512).
4. Bump `SizeOfImage` in the optional header (must be multiple of SectionAlignment, typically 4096).
5. Bump `SizeOfCode` in the optional header.
6. Append trampoline bytes to the file at the position immediately after the original `.text` raw data.

Validate with `r2 -qc "iS" pspcor.patched.exe` — section table should show grown `.text`.

### Phase 4 — Add new debug string to `.rdata`

Optional (debug strings are gated by verbose > 1, so not strictly needed). If included, append "`  FAILED because move w/o cmp overlap\n\0`" to `.rdata` via similar section extension. Easier: reuse an existing string like `"  FAILED because store or call\n"` at `0x4b53fc` — the user won't care.

### Phase 5 — Regression test

Run full match suite against the patched compiler:

```bash
USE_PATCHED_PSPCOR=1 python3 tools/compare_func.py --batch src/
```

Accept criteria:
- All 693 existing matches still match (zero regressions)
- At minimum `gcStateVTableEntry::Set` now matches
- At minimum `cOutStream::WriteBits` improves (may not fully match due to other issues)

If any existing match regresses: investigate whether the regression is from the new check (patch too aggressive) or a side effect (patch incorrectly placed).

### Phase 6 — Document and commit

1. Update `docs/decisions/011-bnel-compiler-patch-design.md` with status=Implemented and actual-impact numbers.
2. Commit `tools/patch_pspcor.py` and `extern/snc/pspcor.patched.exe`.
3. Update `CLAUDE.md` with a note about the patched compiler being optional.
4. Update `docs/enhancements.md` to remove this entry.

## Engineering cost estimate

| Phase | Time |
|-------|------|
| 1. Scaffolding | 2-4 hours |
| 2. Trampoline code | 8-16 hours (RE + assembly hand-coding) |
| 3. PE header extension | 2-4 hours |
| 4. Debug string (optional) | 1 hour |
| 5. Regression testing | 2-4 hours |
| 6. Docs + commit | 1 hour |
| **Total** | **~2-4 days** |

Risk-adjusted: add ~50% for the trampoline hand-assembly debugging (expect 1-2 false starts with incorrect register accesses or wrong opcodes).

## Current known bnel-divergent functions (as of 2026-04-19)

| Function | Size | Diff | Status |
|----------|-----:|-----:|--------|
| `gcStateVTableEntry::Set` | 96B | 4 instrs (2 relocations + bnel/nop) | Clean bnel divergence — would match after patch |
| `cOutStream::WriteBits` | 296B | bnel positions diverge at [36,128,220] vs [124] | Mixed — patch would fix 3 positions; 4th position is different issue |

Spot survey of HAS_BNEL candidates (12 untested) may reveal additional cases. Spot survey of NO_BNEL (7 untested) may reveal another clean case like Set.

## When to revisit

Trigger conditions (any one):

1. **Source-quality ceiling hit**: Match count plateaus despite continued agent work. If bnel divergences become >10% of remaining failures, patch ROI improves.
2. **Pattern in new games**: Milestone 2 (Alien Syndrome) may exhibit bnel divergence at higher rates. If yes, patch transfers and ROI multiplies across games.
3. **Training-data corruption concern**: If agents mark too many functions `unmatchable_bnel` and the resulting lost training pairs hurt the fine-tuned model quality.
4. **Boredom / polish**: Once the main milestones are hit, this patch removes a class of edge cases for a cleaner project.

## References

- `docs/decisions/006-bnel-compiler-divergence.md` — original divergence discovery
- `docs/decisions/010-compiler-internals-experiments.md` — seven-experiment compiler internals map, esp. Experiment A (del_slot.c disassembly) and Experiment F (original's statistical heuristic)
- `/tmp/pspcor_string_index.json` — function/string index for pspcor.exe (may need regeneration if stale)
- Experiment A's Ghidra/r2 analysis of `del_slot_can_fill` at `0x43afbc` (task artifacts)
- Experiment F's branch-likely statistical rules
