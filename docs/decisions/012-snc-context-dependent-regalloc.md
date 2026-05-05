# ADR-012: SNC Context-Dependent Register Allocation

**Date:** 2026-05-05
**Status:** Confirmed finding

## Context

While investigating Collide function failures across shape classes, we discovered that SNC's register allocator produces different output for identical source code depending on which functions were compiled earlier in the same translation unit.

## Finding

The 172B `Collide(const eShape*,...)` generic dispatcher and 140B negate-normal Collide variants are byte-identical across all shape classes in the original binary (except a vtable offset immediate). The source code is identical. The compiler flags are identical. Yet only eSphereShape matches — all other shapes produce 65%+ byte mismatch.

**Root cause:** SNC's register allocator carries internal state across function boundaries within a translation unit. The original `eAll_psp.obj` compiled ~3,000 engine functions in one file. Our per-class `.cpp` files (20-50 functions each) present a completely different compilation context, causing the register allocator to make different choices.

**Evidence:**
- Same code in eSphereShape.cpp (preceded by GetType) → matches
- Same code in eCompoundShape.cpp (preceded by different functions) → 111/172 bytes differ
- Same code in isolated single-function `.cpp` → 113/172 bytes differ (even for eSphereShape!)
- Vtable offset value, class name, inheritance, sched pragma — none affect the outcome
- Only the sequence of preceding functions in the translation unit matters

## Scope of Impact

~40 Collide functions across 7 shape classes (172B dispatchers + 140B negate-normal variants). Approximately 5KB of binary.

Also potentially affects other functions where our per-class file splitting changes the compilation context vs the original monolithic file. Hard to quantify without systematic study.

## Options

1. **Accept as unmatchable at source level.** Mark ~40 functions. Pragmatic, zero effort. Doesn't address the broader issue.

2. **Reconstruct original compilation order.** Merge classes back into monolithic files matching the original function sequence. Fragile, defeats decomposition purpose, requires determining exact original order for ~3K functions.

3. **Patch pspcor.exe register allocator.** Make it context-independent or deterministic. Fixes this class of problem permanently for ALL functions. Heavy reverse-engineering effort, but same work benefits ML2 (cReadBlock prologue, ~600 functions) and any other context-sensitive codegen issues.

## Decision

Accept as unmatchable for now (option 1). Revisit when/if a pspcor.exe patching session is undertaken for ML2 — the register allocator and instruction scheduler are likely adjacent code paths, so fixing both in one session is efficient.

## Related

- ML2 in `docs/enhancements-match-lift.md` — cReadBlock prologue scheduler divergence (same root cause: compiler internals)
- ADR-011 — bnel compiler patch design (different compiler behavior, similar patching approach)
