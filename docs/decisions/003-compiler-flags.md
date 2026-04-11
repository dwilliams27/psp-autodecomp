# 003: Compiler Flags — Per-Obj-File Scheduling Analysis

**Date:** 2026-04-10
**Status:** Verified

## Decision

The binary was not compiled with a single set of flags. Different `.obj` compilation units use different `-Xsched` values. The Makefile defaults to `-Xsched=2` (SNC default) with per-class overrides to `-Xsched=1` for specific engine classes.

## Confirmed Flags

All code: `-c -O2 -G0`

| Obj File | Functions | `-Xsched` | Notes |
|---|---|---|---|
| gcAll_psp.obj | 4,963 | 2 | Game code (gc prefix). Uniform. |
| cAll_psp.obj | 438 | 2 | Core code (c prefix). Uniform. |
| nwAll_psp.obj | 150 | 2 | Network code (nw prefix). Uniform. |
| gMain_psp.obj | 260 | 2 | Assumed (no non-trivial matches yet). |
| mAll_psp.obj | 58 | 2 | Assumed (no matches yet). |
| eAll_psp.obj | 3,125 | **Mixed** | See below. |

## eAll_psp.obj Mixed Scheduling

Within `eAll_psp.obj`, classes split by address range:

**sched=2** (address range ~0x01d000–0x040000):
- eAudio, eCamera, eRenderTarget, eShape, ePortal, ePhysics, eSprite, eSoundDataMap

**sched=1** (address range ~0x06e000–0x08f000):
- eTextureMap, eBumpOffsetMap, eDynamicMeshMorphTarget, eCollisionConstraint, eInputKeyboard

## Evidence

Definitive proof from two structurally identical functions in eAll_psp.obj:

**eRenderTarget::Write** (0x000310e4) — sched=2:
```
addiu sp,sp,-32
sw s1,12(sp)      ; save s1
move s1,a0        ; interleaved
sw s0,8(sp)       ; save s0
move s0,a1        ; interleaved
move a0,sp
sw ra,16(sp)      ; save ra (deferred)
```

**eTextureMap::Write** (0x0007ae00) — sched=1:
```
addiu sp,sp,-32
sw s0,8(sp)       ; save s0 (grouped)
sw s1,12(sp)      ; save s1 (grouped)
sw ra,16(sp)      ; save ra (grouped)
move s0,a1
move s1,a0
```

Both functions have identical structure (3 callee saves + function calls). The only difference is prologue instruction scheduling.

## Additional Per-File Flags

Some individual files need extra flags beyond the sched override:
- `gcLoadingScreen_Read.cpp` → `-Xxopt=5`
- `gcUIWidget_InsertIntoDialog.cpp` → `-Xmopt=0`

These are discovered during matching when a function's logic is correct but bytes differ.

## Impact on Synthetic Data Pipeline

The training data generator must:
1. Compile each synthetic function with BOTH `-Xsched=1` and `-Xsched=2`
2. Tag each training pair with its sched flag
3. For real matched pairs, determine the correct flag from the function's obj_file and class

## Method

Classification was done by disassembling prologue bytes from `expected/bin/*.bin` (extracted from the original binary) and checking whether register saves are grouped (sched=1) or interleaved with user code (sched=2). Functions with fewer than 3 callee-saved registers are ambiguous between the two modes. Compiled test objects with both flags confirmed byte-exact matches against the expected binaries.
