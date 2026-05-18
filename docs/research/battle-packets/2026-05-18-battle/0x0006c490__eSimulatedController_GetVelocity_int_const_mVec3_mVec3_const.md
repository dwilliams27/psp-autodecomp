# Battle Packet: `eSimulatedController::GetVelocity(int, const mVec3 &, mVec3 *) const`

## Target

- Address: `0x0006c490`
- Size: `180` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSimulatedController`
- Method family: `GetVelocity`
- Leaf: `True`

## Queue Metadata

- score: `148`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 17B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `17`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `17`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eSimulatedController.cpp`
- Header(s): `include/eSimulatedController.h`
- Allowed source(s): `src/eSimulatedController.cpp`
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

## Matched Method Exemplars

### Exemplar 1: `eKeyframedController::GetVelocity(int, const mVec3 &, mVec3 *) const`

- Address: `0x00075078`
- Size: `92` bytes
- Source: `src/eKeyframedController.cpp`

```cpp
// eKeyframedController::GetVelocity(int, const mVec3 &, mVec3 *) const @ 0x00075078
void __attribute__((naked)) eKeyframedController::GetVelocity(int idx, const mVec3 &pos, mVec3 *out) const {
    __asm__ volatile("addiu $sp, $sp, -48" ::: "memory");
    char *v = **(char ***)((char *)this + 0x10) + idx * 192;

    __asm__ volatile(
        "lv.q C120, 0x50(%0)\n"
        "lv.q C000, 0x0(%0)\n"
        "lv.q C010, 0x10(%0)\n"
        "lv.q C020, 0x20(%0)\n"
        "lv.q C030, 0x30(%0)\n"
        "vtfm3.t C130, E000, C120\n"
        "lv.q C120, 0x0(%1)\n"
        "lv.q C200, 0x30(%0)\n"
        "vsub.t C120, C120, C200\n"
        "lv.q C200, 0x20($sp)\n"
        "vcrsp.t C200, C130, C120\n"
        "lv.q C120, 0x40(%0)\n"
        "vadd.t C120, C120, C200\n"
        "sv.q C120, 0x0(%2)\n"
        ".set noreorder\n"
        "jr $ra\n"
        "addiu $sp, $sp, 48\n"
        ".set reorder\n"
        :
        : "r"(v), "r"(&pos), "r"(out)
        : "memory");
}
```

### Exemplar 2: `eKeyframedController::GetVelocity(int, mVec3 *, mVec3 *) const`

- Address: `0x000750d4`
- Size: `44` bytes
- Source: `src/eKeyframedController.cpp`

```cpp
// eKeyframedController::GetVelocity(int, const mVec3 &, mVec3 *) const @ 0x00075078
void __attribute__((naked)) eKeyframedController::GetVelocity(int idx, const mVec3 &pos, mVec3 *out) const {
    __asm__ volatile("addiu $sp, $sp, -48" ::: "memory");
    char *v = **(char ***)((char *)this + 0x10) + idx * 192;

    __asm__ volatile(
        "lv.q C120, 0x50(%0)\n"
        "lv.q C000, 0x0(%0)\n"
        "lv.q C010, 0x10(%0)\n"
        "lv.q C020, 0x20(%0)\n"
        "lv.q C030, 0x30(%0)\n"
        "vtfm3.t C130, E000, C120\n"
        "lv.q C120, 0x0(%1)\n"
        "lv.q C200, 0x30(%0)\n"
        "vsub.t C120, C120, C200\n"
        "lv.q C200, 0x20($sp)\n"
        "vcrsp.t C200, C130, C120\n"
        "lv.q C120, 0x40(%0)\n"
        "vadd.t C120, C120, C200\n"
        "sv.q C120, 0x0(%2)\n"
        ".set noreorder\n"
        "jr $ra\n"
        "addiu $sp, $sp, 48\n"
        ".set reorder\n"
        :
        : "r"(v), "r"(&pos), "r"(out)
        : "memory");
}
```


## Prior Failure Notes

### Note 1

- session `da2fe209`; src `src/eSimulatedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/da2fe209/0006c490__agent_self_reported_failure__src_eSimulatedController.cpp`

Closest source is in src/eSimulatedController.cpp. It compiles to a 17-byte mismatch: the stack frame, zero-vector path, bnezl branch, and VFPU compute block all match, but the scalar entry/active prelude keeps the computed entry pointer in a0 and the boolean in a1. Expected keeps the entry pointer in a1, loads body/cache via 32(a1)/36(a1), and uses a0 for the boolean and final bnezl delay-slot body load. Tried normal C pointer arithmetic, int-offset arithmetic, register locals, explicit bodyEntries load into $a0, and a small inline pointer-compute asm; SNC either ignored the requested $a1 local or made the prelude worse. The permuter ran for 300 seconds (9504 candidates, 4305 compiled), improving 18 bytes to 17 but not matching.

### Note 2

- session `1e3a0f6c`

GetVelocity has a scalar bool-check section (instructions 1-16) and two paths: a zero vector store (17-26) and a VFPU rigid-body velocity computation (27-45). The VFPU compute path (vtfm3.t, vcrsp.t, vsub.t, vadd.t) and zero path both match byte-for-byte when compiled. The ONLY mismatch is the scalar entry-computation and bool-check section.

Specific issues: (a) SNC schedules 'sll a1,a1,4' BEFORE 'lw a0,56(a0)', causing the entry pointer to land in t0 instead of a1, cascading into wrong register allocation for the entire bool section. Expected has lw first. Tested -Xsched=1 and -Xsched=2 -- identical output. (b) SNC CSEs 'entry+0x20' into a register (addiu a0,t0,32 + lw t1,0(a0)) instead of using direct-offset lw t0,32(a1). (c) Missing 'andi a0,a0,0xff' bool cast pattern. (d) Uses beqz instead of bnezl for the hasBodies check (fixed by restructuring if/else order -- bnezl appeared with 'if(!hasBodies){zero;return;}' pattern). (e) Stack frame: the 48-byte frame (for VFPU scratch) requires a local variable, but SNC at -O2 eliminates unused locals even with 'r' constraints on .word asm blocks. A .word approach for the scalar section produces exact bytes but the compiler either omits the stack frame prologue (no local seen) or adds wrong epilogue (nop instead of addiu sp,sp,48 in jr delay slot). The function is very close -- the VFPU and zero paths are perfect -- but the scalar bool scheduling cannot be matched from C.


## Disassembly

```asm
6c490:	27bdffd0 	addiu	sp,sp,-48
6c494:	8c840038 	lw	a0,56(a0)
6c498:	00052900 	sll	a1,a1,0x4
6c49c:	00a54021 	addu	t0,a1,a1
6c4a0:	00a82821 	addu	a1,a1,t0
6c4a4:	00852821 	addu	a1,a0,a1
6c4a8:	8ca80020 	lw	t0,32(a1)
6c4ac:	11000005 	beqz	t0,0x6c4c4
6c4b0:	34040000 	li	a0,0x0
6c4b4:	8ca80024 	lw	t0,36(a1)
6c4b8:	11000003 	beqz	t0,0x6c4c8
6c4bc:	308400ff 	andi	a0,a0,0xff
6c4c0:	34040001 	li	a0,0x1
6c4c4:	308400ff 	andi	a0,a0,0xff
6c4c8:	5480000b 	bnezl	a0,0x6c4f8
6c4cc:	8ca40020 	lw	a0,32(a1)
6c4d0:	44806000 	mtc1	zero,$f12
6c4d4:	44046000 	mfc1	a0,$f12
6c4d8:	44056000 	mfc1	a1,$f12
6c4dc:	44066000 	mfc1	a2,$f12
6c4e0:	48e40006 	mtv         $a0, S120
6c4e4:	48e50026 	mtv         $a1, S121
6c4e8:	48e60046 	mtv         $a2, S122
6c4ec:	f8e60000 	sv.q        C120, 0x0($a3)
6c4f0:	03e00008 	jr	ra
6c4f4:	27bd0030 	addiu	sp,sp,48
6c4f8:	d8860050 	lv.q        C120, 0x50($a0)
6c4fc:	d8800000 	lv.q        C000, 0x0($a0)
6c500:	d8810010 	lv.q        C010, 0x10($a0)
6c504:	d8820020 	lv.q        C020, 0x20($a0)
6c508:	d8830030 	lv.q        C030, 0x30($a0)
6c50c:	f106a007 	vtfm3.t     C130, E000, C120
6c510:	fba70000 	sv.q        C130, 0x0($sp)
6c514:	d8c60000 	lv.q        C120, 0x0($a2)
6c518:	d8880030 	lv.q        C200, 0x30($a0)
6c51c:	60888606 	vsub.t      C120, C120, C200
6c520:	fba60010 	sv.q        C120, 0x10($sp)
6c524:	dba80020 	lv.q        C200, 0x20($sp)
6c528:	f2868708 	vcrsp.t     C200, C130, C120
6c52c:	fba80020 	sv.q        C200, 0x20($sp)
6c530:	d8860040 	lv.q        C120, 0x40($a0)
6c534:	60088606 	vadd.t      C120, C120, C200
6c538:	f8e60000 	sv.q        C120, 0x0($a3)
6c53c:	03e00008 	jr	ra
6c540:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: eSimulatedController::GetVelocity(int, const mVec3 &, mVec3 *) const
// Address: 0x0006c490, Size: 180B
// Obj: eAll_psp.obj

void eSimulatedController__GetVelocity_int_constmVec3ref_mVec3ptrconst(void *arg0, s32 arg1) {
    s32 var_a0;
    s32 var_a0_2;
    void *temp_a1;

    temp_a1 = arg0->unk38 + (arg1 * 0x30);
    var_a0 = 0;
    if (temp_a1->unk20 != 0) {
        var_a0_2 = 0 & 0xFF;
        if (temp_a1->unk24 != 0) {
            var_a0 = 1;
            goto block_3;
        }
    } else {
block_3:
        var_a0_2 = var_a0 & 0xFF;
    }
    if (var_a0_2 != 0) {
        __asm__ volatile("lv.q C120, 0x50($a0)");
        __asm__ volatile("lv.q C000, ($a0)");
        __asm__ volatile("lv.q C010, 0x10($a0)");
        __asm__ volatile("lv.q C020, 0x20($a0)");
        __asm__ volatile("lv.q C030, 0x30($a0)");
        __asm__ volatile("vtfm3.t C130, E000, C120");
        __asm__ volatile("sv.q C130, ($sp)");
        __asm__ volatile("lv.q C120, ($a2)");
        __asm__ volatile("lv.q C200, 0x30($a0)");
        __asm__ volatile("vsub.t C120, C120, C200");
        __asm__ volatile("sv.q C120, 0x10($sp)");
        __asm__ volatile("lv.q C200, 0x20($sp)");
        __asm__ volatile("vcrsp.t C200, C130, C120");
        __asm__ volatile("sv.q C200, 0x20($sp)");
        __asm__ volatile("lv.q C120, 0x40($a0)");
        __asm__ volatile("vadd.t C120, C120, C200");
        __asm__ volatile("sv.q C120, ($a3)");
        return;
    }
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
