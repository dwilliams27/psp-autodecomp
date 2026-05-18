# Battle Packet: `eSimulatedController::GetInvMass(int, float *, mVec3 *) const`

## Target

- Address: `0x0006c5d4`
- Size: `120` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSimulatedController`
- Method family: `GetInvMass`
- Leaf: `True`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 15B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `15`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `15`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Placement

- Canonical source: `src/eSimulatedController.cpp`
- Header(s): `include/eSimulatedController.h`
- Allowed source(s): `src/eSimulatedController.cpp`, `src/eSimulatedController_GetInvMass.cpp`
- Split-TU prefix: `src/eSimulatedController_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eSimulatedController.h`

```cpp
#ifndef ESIMULATEDCONTROLLER_H
#define ESIMULATEDCONTROLLER_H

struct mVec3;

class cBase;
class eGeom;
class cFile;
class cMemPool;
class cName;
class cType;
class eDynamicModel;

class ePhysicsController {
public:
    ePhysicsController(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;

    char _basePad[0x10];
};

class eSimulatedController : public ePhysicsController {
public:
    eSimulatedController(cBase *);
    void InvalidateCacheEntries(eGeom *);
    void RemoveContacts(void);
    bool IsSleeping(void) const;
    bool IsInFluid(void) const;
    int GetCollisionMask(void) const;
    void SetCollisionMask(unsigned int);
    void SetPosition(int index, const mVec3 &pos);
    void ApplyTorque(int index, const mVec3 &torque);
    void ApplyForce(int index, const mVec3 &force, const mVec3 &pos);
    int GetSubObjectIndex(const cName &) const;
    void ClearExternalForces(void);
    void GetVelocity(int index, const mVec3 &pos, mVec3 *out) const;
    void SnapConstraints(void);
    void Activate(eDynamicModel *);
    void Deactivate(eDynamicModel *);
    void ApplyUnembedImpulse(int index, const mVec3 &impulse);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;

    char _pad[0x24];
    int collisionMask;      // offset 0x34
    void *bodyEntries;      // offset 0x38
    char _pad2[0x114];      // offset 0x3C
    int contactCount;       // offset 0x150
};

#endif
```

## Matched Same-Class Neighbors

- `0x0006bd5c` 76B `eSimulatedController::Write(cFile &) const` — `src/eSimulatedController.cpp`
- `0x0006bda8` 188B `eSimulatedController::Read(cFile &, cMemPool *)` — `src/eSimulatedController.cpp`
- `0x0006be64` 172B `eSimulatedController::eSimulatedController(cBase *)` — `src/eSimulatedController.cpp`
- `0x0006bf10` 364B `eSimulatedController::~eSimulatedController(void)` — `src/eSimulatedController_dtor.cpp`
- `0x0006c07c` 156B `eSimulatedController::Activate(eDynamicModel *)` — `src/eSimulatedController.cpp`

## Prior Failure Notes

### Note 1

- session `8ff06f7b`; src `src/eSimulatedController_GetInvMass.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/8ff06f7b/0006c5d4__agent_self_reported_failure__src_eSimulatedController_GetInvMass.cpp`

Created a split-TU implementation to avoid adding GetInvMass to include/eSimulatedController.h and perturbing existing matched methods. Started from the prior C++ valid-entry pattern, then changed the zero fallback to numeric ABI registers ($4/$5/$6/$7) so the mtc1/swc1/mfc1/mtv/jr/sv.q sequence matches the original zero path exactly. Also tested sched=1, a valid/maskedValid control-flow variant, an if/else body-path variant, an always_inline+noreturn zero helper, and a naked method annotation; none removed the compiler-generated return block cleanly.

Closest source is 15/120 bytes off but emits a 128-byte symbol, so tools/permuter.py skipped with PERMUTER_SKIPPED_NOT_LAST_MILE. The remaining word-level diff is at 0x34 and 0x64-0x74: SNC emits an extra jr/nop after the explicit zero-path jr before the body path, shifting the final lwc1/swc1/lv.q/jr/sv.q sequence by 8 bytes. The original has bnezl target 0x64 with lw a0,36(a1) in the delay slot; closest source has target 0x6c because of the synthetic return block.

### Note 2

- session `679d28cb`; src `src/eSimulatedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/679d28cb/0006c5d4__agent_self_reported_failure__src_eSimulatedController.cpp`

Closest attempt is real C++ for the entry lookup and valid-body test, plus small inline VFPU sequences for the zero-vector fallback. It compiles cleanly and preserves existing matches in this file, but compare_func still reports 25/120 differing bytes. The remaining core diff is the zero fallback: original has mtc1/swc1/mfc1/mtv followed by jr ra with sv.q in the delay slot, while inline asm keeps sv.q before jr and emits jr/nop. A native v4sf_t assignment variant did put sv.q in the delay slot, but inserted an extra VFPU move/load before mtc1 and worsened the diff to 45 bytes. A naked-function variant could express the delay slot but clobbered argument registers and left extra compiler return code, also worse. Tried sched=2 local pragma, m2c-style masked flag control flow, C scalar mVec3 stores, native v4sf_t store, and explicit return-slot asm. Permuter was attempted for 300s with --save-best, but tools/permuter.py could not isolate this function in the multi-function object and aborted before searching.


## Disassembly

```asm
6c5d4:	8c840038 	lw	a0,56(a0)
6c5d8:	00052900 	sll	a1,a1,0x4
6c5dc:	00a54021 	addu	t0,a1,a1
6c5e0:	00a82821 	addu	a1,a1,t0
6c5e4:	00852821 	addu	a1,a0,a1
6c5e8:	8ca80020 	lw	t0,32(a1)
6c5ec:	11000005 	beqz	t0,0x6c604
6c5f0:	34040000 	li	a0,0x0
6c5f4:	8ca80024 	lw	t0,36(a1)
6c5f8:	11000003 	beqz	t0,0x6c608
6c5fc:	308400ff 	andi	a0,a0,0xff
6c600:	34040001 	li	a0,0x1
6c604:	308400ff 	andi	a0,a0,0xff
6c608:	5480000b 	bnezl	a0,0x6c638
6c60c:	8ca40024 	lw	a0,36(a1)
6c610:	44806000 	mtc1	zero,$f12
6c614:	e4cc0000 	swc1	$f12,0(a2)
6c618:	44046000 	mfc1	a0,$f12
6c61c:	44056000 	mfc1	a1,$f12
6c620:	44066000 	mfc1	a2,$f12
6c624:	48e40006 	mtv         $a0, S120
6c628:	48e50026 	mtv         $a1, S121
6c62c:	48e60046 	mtv         $a2, S122
6c630:	03e00008 	jr	ra
6c634:	f8e60000 	sv.q        C120, 0x0($a3)
6c638:	c48c0044 	lwc1	$f12,68(a0)
6c63c:	e4cc0000 	swc1	$f12,0(a2)
6c640:	d8860050 	lv.q        C120, 0x50($a0)
6c644:	03e00008 	jr	ra
6c648:	f8e60000 	sv.q        C120, 0x0($a3)
```

## m2c Starting Point

```c
// Decompiled: eSimulatedController::GetInvMass(int, float *, mVec3 *) const
// Address: 0x0006c5d4, Size: 120B
// Obj: eAll_psp.obj

void eSimulatedController__GetInvMass_int_floatptr_mVec3ptrconst(void *arg0, s32 arg1, f32 *arg2) {
    s32 var_a0;
    s32 var_a0_2;
    void *temp_a1;

    temp_a1 = arg0->unk38 + (arg1 * 0x30);
    var_a0 = 0;
    if (temp_a1->unk20 != 0) {
        var_a0_2 = 0 & 0xFF;
        if (temp_a1->unk24 != NULL) {
            var_a0 = 1;
            goto block_3;
        }
    } else {
block_3:
        var_a0_2 = var_a0 & 0xFF;
    }
    if (var_a0_2 != 0) {
        *arg2 = temp_a1->unk24->unk44;
        __asm__ volatile("lv.q C120, 0x50($a0)");
        __asm__ volatile("sv.q C120, ($a3)");
        return;
    }
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
