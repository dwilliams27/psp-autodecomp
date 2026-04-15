# Post-Mortem: Overnight Finetune Target Run (2026-04-14 → 04-15)

## Summary

| Metric | Value |
|--------|-------|
| Duration | 8h 12m |
| Sessions | 15 |
| Functions attempted | 24 (of 30 finetune targets) |
| Matched | 4 (17%) |
| Failed | 20 (83%) |
| Avg tools/session | 93 |
| Research docs read | 11/15 sessions (73%) |

## What Matched (4 functions)

| Function | Size | Approach |
|----------|------|----------|
| mQuat::Mult | 180B | Pure FPU quaternion multiply — agent wrote C++ with correct expression order per snc-fpu-scheduling.md |
| mBox::Extend | 132B | VFPU min/max with branch logic — agent used inline asm for VFPU ops within C++ control flow |
| mFrustum::GetOrtho | 84B | Scalar FPU function, no VFPU — straightforward C++ |
| eStaticSkyLight::GetDirectLight | 184B | Mixed FPU/VFPU — C++ with inline asm for VFPU dot product and normalize |

**Key success factor**: All 4 matched functions had either (a) no VFPU delay slot requirements, or (b) no branch-likely divergence. The research library (especially snc-fpu-scheduling.md) directly contributed to mQuat::Mult matching on the first session.

## What Failed (20 functions) — Root Cause Analysis

### Category 1: Branch-Likely Divergence (8 functions, 40%)

Our SNC (1.2.7503.0) generates `bnezl`/`beqzl` (branch-likely) where the original binary has `beqz`/`bnez` + `nop` (regular branch). This produces 4-8 byte diffs that NO source restructuring can fix.

**Affected**: gcDoReturn::Evaluate (8B off), gcDoReturn::Read (8B off), gcExpressionList::Evaluate, cRedBlackTree::FindNode, eCapsuleShape::GetSupport, eCollisionConstraint::Initialize, cOutStream::Write, nwConnection::UpdatePing

**Agent effort level**: HIGH. The gcDoReturn agent tried 20+ compare iterations, 6 source restructurings, all mopt/xxopt/sched flag combinations, and ran the permuter. The nwConnection agent tried 10 attempts with exhaustive flag testing. Both concluded it's hardwired in SNC 1.2.7503.0.

**Verdict**: This is a compiler version mismatch, not an agent skill issue.

### Category 2: VFPU Delay Slot (5 functions, 25%)

Functions where the original binary has `sv.q` in a `jr $ra` delay slot. Our inline asm `sv.q` cannot fill delay slots (confirmed by research). The only way to get compiler-generated `sv.q` in a delay slot is through V4SF/SceULong128 struct copy, but that produces a different instruction pattern (lv.q+sv.q) than the expected mtc1/mfc1/mtv/sv.q construction pattern.

**Affected**: mBasis::SetQuat, mBasis::Orthonormalize, eBipedController::GetVelocity, eMultiSphereShape::GetProjectedMinMax, gcEntityFollowState ctor

**Agent effort level**: HIGH. The mBasis::SetQuat agent tried 8 attempts, successfully reproduced the mfc1/mtv/sv.q store pattern using the constructor codegen recipe, but hit three additional issues: mul.s operand ordering, FPU register allocation sequence, and epilogue generation. These are ALL SNC version-specific behaviors.

### Category 3: Register Allocation / Scheduling (5 functions, 25%)

Correct instructions but wrong register assignment or instruction order. These are optimizer heuristic differences.

**Affected**: cCRC::Calc, ePath::PathT2Units, eSimulatedController::GetVelocity, eAudioChannel::CalcPanning, eDynamicGeom ctor

**Agent effort level**: MEDIUM-HIGH. Agents tried reordering variable declarations, changing expression structure, splitting/merging operations. Some got close but couldn't nail the exact register allocation.

### Category 2.5: Inline Asm Limitations (2 functions)

mBox::Extend hit inline asm interaction issues. gcLookAtController::SetHPR was too complex (640B) for one session.

## The SNC Version Problem

**Our compiler**: pspsnc 1.2.7503.0 (build: "Apr 29 2010", copyright 2003-2009 SN Systems)
**Game build date**: Jan 13 2011 (from EBOOT strings)
**Game engine**: ViciousEngine by Vicious Cycle Software
**Target firmware**: PSP 6.20

The game was compiled ~9 months after our SNC's build date. There may be a newer SNC version (e.g., 1.2.7600 or 1.2.8000) that was released between Apr 2010 and Jan 2011, with slightly different branch-likely heuristics and FPU register allocation rules.

### Evidence of Version Mismatch

1. **mul.s operand ordering**: Our SNC puts mtc1-loaded constants first (`mul.s f16, f14, f12` = const*var). The original puts variables first (`mul.s f16, f12, f14` = var*const). This is a specific optimizer behavior difference.

2. **Branch-likely threshold**: Our SNC uses bnezl/beqzl aggressively for null-check-and-dereference patterns. The original uses regular branches with nop for some of these same patterns. This suggests a different set of heuristics for when branch-likely is profitable.

3. **FPU register allocation order**: Our SNC allocates 1.0f to f19 (last primary register), original allocates to f4 (first secondary). Different allocation priority ordering.

### How Hard Is Finding Another SNC Version?

**Sources to check**:
1. **decompme/compilers** (GitHub: decompals/decomp.me) — primary source. Check if they have multiple SNC versions. Our version came from here.
2. **Other PSP decomp projects** — the Patapon, Kingdom Hearts BBS, and Dissidia communities may have different SNC versions.
3. **PSP ProDG SDK archives** — SN Systems distributed SNC as part of the ProDG SDK for PSP. Different SDK versions (3.x, 4.x, 5.x, 6.x) included different SNC compiler versions.
4. **archive.org** — PSP SDK packages sometimes surface there.
5. **ViciousEngine source/SDK** — unlikely to be public, but Vicious Cycle used their own engine across multiple PSP titles. Other games using ViciousEngine might have been compiled with the same SNC version.

**Difficulty**: MEDIUM. There are likely 5-10 different SNC versions that existed between 2008-2012. Finding the exact one requires:
1. Obtaining candidate SNC binaries
2. Compiling a test case with each
3. Checking if branch-likely behavior matches the original
4. Checking if mul.s operand ordering matches

A single test case could identify the correct version in minutes once we have the binaries.

**Quick test**: Compile this and check if `bnezl` or `bnez+nop` is generated:
```cpp
void f(int *p, int *out) {
    if (p != 0) { *out = *p; }
}
```
Our SNC: `bnezl $a0, +1` / `lw $v0, 0($a0)` (branch-likely, 8 bytes)
Expected: `beqz $a0, skip` / `nop` / `lw $v0, 0($a0)` (regular, 12 bytes)

## Recommendations

1. **Priority 1: Find alternative SNC versions.** This is the highest-leverage action. Check decompme/compilers, other PSP decomp communities, and archive.org for different SNC builds. A version closer to the game's build date (late 2010) may eliminate the branch-likely and register allocation divergences.

2. **Priority 2: Accept ~8-byte diffs for branch-likely functions.** If an alternative SNC version can't be found, consider a "close enough" threshold for branch-likely functions where the ONLY diff is the branch form. These functions have correct C/C++ source — the diff is purely a compiler version artifact.

3. **Priority 3: Focus overnight runs on non-branch-likely functions.** The 689 matched + the 4 new matches = 693 honest matches. There are 8,500+ untried functions. Run general matching (not finetune targets) to grow the match count on functions that don't hit the branch-likely wall.

4. **Priority 4: Investigate per-obj-file compiler versions.** Different .obj files in the game might have been compiled with different SNC versions (e.g., engine code vs game code, or code compiled at different times during development). Test whether branch-likely behavior differs between obj files.
