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

## Update (2026-05-31) — this ADR conflated TWO mechanisms

A characterization workflow (empirical prefix experiment + pspcor RE + testbed bisection,
validated against the byte-exact oracle) showed the "context-dependent register allocation"
bucket is actually **two distinct compiler subsystems**, with very different fixes:

**Mechanism A — sticky cross-function INSTRUCTION-SCHEDULER latch (the Collide dispatchers).**
This is the original ADR-012 testbed (~40 functions / ~5 KB: the 172B dispatchers + 140B
negate-normal variants). It is NOT register allocation. A preceding **extern** function in the
TU whose body contains an explicit `jr $ra` (a compiler-unrecognized control transfer) flips a
sticky scheduler-state latch that makes the dispatcher's scheduling/delay-slot fill match the
original monolithic-TU output. **Reproducible with NO binary patch — VALIDATED byte-exact:**
- eMeshShape::Collide @0x4fdf8: isolated 111/172 → **0/172** with one jr-predecessor.
- eCapsuleShape::Collide @0x6aa7c: isolated 110/172 → **0/172**.
- Answers the ADR's open question: NOT H-count, NOT H-pressure (stub-count sweeps N=1..50 and
  pressure sweeps both no-op). Discriminator pinned: a predecessor with `jr $ra` → match;
  `nop\nnop` → fail; `move $t0,$t1` → fail. Latch saturates at one (1/2/3 predecessors equal).
- Recipe: place a predecessor with a `jr $ra` tail before the dispatcher in the TU. Cleanest
  form = a **real sibling method of the class that has an asm/jr tail** (as eSphereShape's
  matched dispatcher sits after GetInertialTensor). Fallback = a minimal stub
  `void z(){__asm__ volatile("jr $ra\nnop\n");}` — but that stub is artificial scaffolding whose
  body is hand-written `jr` asm (asm-ban-adjacent — it is NOT a claimed match, only a scheduler
  seed; prefer a real sibling predecessor; operator should rule on the stub form).
- Confounder noted: eCompoundShape/eDynamicModel compile whole-file under `-Xsched=1`
  (Makefile); keep dispatcher pragmas consistent.

**Mechanism B — genuine per-function REGISTER ALLOCATION (the real byte prize).** The true
GRA callee-save coloring class (cFactory::WriteGroups 0xbd44 = pure s1↔s3 9-byte swap;
eSimulatedController::ApplyPositionedImpulse 47B; eDynamicModel::SnapControllerTo 57B;
eBoxShape::GetProjectedMinMax 77B; and the bulk of the 1,733 functions ≥500B holding ~80% of
remaining bytes). The jr-prefix does NOT help (WriteGroups 9→97 *worse*; ApplyPositionedImpulse
47→39; SnapControllerTo 57→53). pspcor RE: the TN allocator + GRA live-range graph is
reset/rebuilt per program-unit; coloring depends only on the function's own IR with **no cheap
cross-function knob and no word-sized settable seed**. This class genuinely requires the
**pspcor allocator patch (option 3 above)** — an operator-gated binary-RE project (mangler-scale,
see ADR-014's path-A experience), not a source/grouping fix.

**Net:** Mechanism A is a clean ~5 KB no-patch win (pending the stub-vs-real-predecessor purity
call). Mechanism B — the actual 80%-of-bytes mountain — is blocked behind a pspcor allocator
patch, the same class of deep RE as the (ultimately structural/risky) ADR-014 mangler patch.
