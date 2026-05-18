# Battle Packet: `eKeyframedController::GetInvMass(int, float *, mVec3 *) const`

## Target

- Address: `0x00075100`
- Size: `40` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eKeyframedController`
- Method family: `GetInvMass`
- Leaf: `True`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 7B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 0 retry/prep target`
- failure_action: `retry`
- near_miss_bytes: `7`

## Failure Classification

- Primary: `retryable_nearmiss`
- Action: `retry`
- Confidence: `medium`
- Tags: `retryable_nearmiss`
- Near miss: `7`
- Summary: Small byte diff with no stronger compiler-blocker signal.

## Placement

- Canonical source: `src/eKeyframedController.cpp`
- Header(s): `include/eKeyframedController.h`
- Allowed source(s): `src/eKeyframedController.cpp`
- Split-TU prefix: `src/eKeyframedController_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eKeyframedController.h`

```cpp
#ifndef EKEYFRAMEDCONTROLLER_H
#define EKEYFRAMEDCONTROLLER_H

class cMemPool;
class cFile;
class eDynamicModel;
class mVec3;
class mOCS;

class eKeyframedController {
public:
    int Read(cFile &, cMemPool *);
    void Activate(eDynamicModel *);
    void Deactivate(eDynamicModel *);
    void __attribute__((naked)) GetVelocity(int, const mVec3 &, mVec3 *) const;
    void GetVelocity(int, mVec3 *, mVec3 *) const;
    void GetInvMass(int, float *, mVec3 *) const;
    void GetLocalToWorld(int, mOCS *) const;
    void GetPosition(int, mVec3 *) const;
    void SetPosition(int, const mVec3 &);
    void ApplyImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyForce(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedForce(int, const mVec3 &, const mVec3 &);
    void Reset(cMemPool *, bool);
    void PostUpdate(mOCS *, eDynamicModel *) const;
    void AssignCopy(const class cBase *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00074db4` 76B `eKeyframedController::Write(cFile &) const` — `src/eKeyframedController_Write.cpp`
- `0x00074e00` 188B `eKeyframedController::Read(cFile &, cMemPool *)` — `src/eKeyframedController.cpp`
- `0x00074ef4` 124B `eKeyframedController::~eKeyframedController(void)` — `src/eKeyframedController_dtor.cpp`
- `0x00074f70` 16B `eKeyframedController::Reset(cMemPool *, bool)` — `src/eContactCollector_ClearContactseContactCollector.cpp`
- `0x00074f80` 8B `eKeyframedController::Activate(eDynamicModel *)` — `src/eKeyframedController_Activate.cpp`

## Prior Failure Notes

### Note 1

- session `7f3dc83c`; src `src/eKeyframedController.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/7f3dc83c/00075100__agent_self_reported_failure__src_eKeyframedController.cpp`

Closest retained attempt is the existing v4sf_t output shape: scalar C emits mtc1/swc1, three separate mfc1 statements bind a0/a1/a2, the inline mtv block builds C120, and the final C vector assignment lets SNC put sv.q in the jr ra delay slot. It remains a 7/40 byte mismatch because SNC inserts an extra vmov.q C120,C120 before jr ra.

I tested binding the vector variable directly to C120 and using it as an asm input instead of an output. SNC still emitted the same extra vector move and warned that the vector was potentially uninitialized, so this did not improve the prior best. This is the same known delay-slot/VFPU construction blocker from earlier notes: pure inline sv.q would remove the vmov but cannot fill the jr delay slot, while compiler-generated sv.q needs a vector value and currently materializes it with vmov.

### Note 2

- session `a0376310`; src `src/eKeyframedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/a0376310/00075100__agent_self_reported_failure__src_eKeyframedController.cpp`

Closest source uses C for *inv_mass = 0.0f, fixed a0/a1/a2 register outputs for the mfc1 values, and a v4sf_t asm output so SNC owns the final vector store. This produces the exact mtc1/swc1/mfc1/mtv sequence and places the final sv.q in the jr delay slot, but declaring the v4sf_t as an asm output requires an extra vmov.q C120,C120 before jr. Without referencing the vector output, SNC treats the vector as uninitialized and emits an unwanted lv.q before the scalar setup instead. Tried TI zero assignment, V4SF aggregate zero initialization, uninitialized V4SF store, v4sf_t asm output with vmov.q, early-clobber output, and a VS-scalar-field struct copy. The VS struct path spills through the stack and scalar VFPU stores; the best remaining diff is the single extra vmov.q in the v4sf_t output shape (compare reports 7/40 differing bytes, emitted body is 44B). Permuter was attempted for 300s with --save-best, but tools/permuter.py invokes SNC without -Xvfpumatrix/-Xvfpuscalar and fails to compile this VFPU source, so no automated mutations ran.

### Note 3

- session `46282ec4`

GetInvMass(int, float *, mVec3 *) const — 40B. Same blocker as GetPosition: sv.q must land in the jr ra delay slot, but the VFPU zero-quad construction (mtc1/mfc1/mtv) must be inline asm (no C equivalent), so SNC refuses to schedule the inline sv.q into the delay slot. The only scalar C part is the leading *invMass = 0.0f (swc1), which works. Tried: writing *invMass = 0.0f then the mVec3-ctor-style inline-asm block — produces 44B with sv.q placed before jr ra + nop in delay slot. Also considered placement-new and v4sf_t assignment — none avoid the delay-slot requirement. Pure-asm solution banned by CLAUDE.md.


## Disassembly

```asm
75100:	44806000 	mtc1	zero,$f12
75104:	e4cc0000 	swc1	$f12,0(a2)
75108:	44046000 	mfc1	a0,$f12
7510c:	44056000 	mfc1	a1,$f12
75110:	44066000 	mfc1	a2,$f12
75114:	48e40006 	mtv         $a0, S120
75118:	48e50026 	mtv         $a1, S121
7511c:	48e60046 	mtv         $a2, S122
75120:	03e00008 	jr	ra
75124:	f8e60000 	sv.q        C120, 0x0($a3)
```

## m2c Starting Point

```c
// Decompiled: eKeyframedController::GetInvMass(int, float *, mVec3 *) const
// Address: 0x00075100, Size: 40B
// Obj: eAll_psp.obj

void eKeyframedController__GetInvMass_int_floatptr_mVec3ptrconst(f32 *arg2) {
    *arg2 = 0.0f;
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, ($a3)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
