# Battle Packet: `eBipedController::GetLocalToWorld(int, mOCS *) const`

## Target

- Address: `0x00065cf0`
- Size: `132` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `GetLocalToWorld`
- Leaf: `True`

## Queue Metadata

- score: `116`
- priority: `1`
- reason: `has failure_notes; has failure snapshot; near miss 18B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `18`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `18`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Placement

- Canonical source: `src/eBipedController.cpp`
- Header(s): `include/eBipedController.h`
- Allowed source(s): `src/eBipedController.cpp`
- Split-TU prefix: `src/eBipedController_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eBipedController.h`

```cpp
#ifndef EBIPEDCONTROLLER_H
#define EBIPEDCONTROLLER_H

class mVec3;
class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eContact;
class eContactCollector;

class eBipedController {
public:
    static cBase *New(cMemPool *, cBase *);

    int Read(cFile &, cMemPool *);

    void ApplyImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyForce(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &);
    void ApplyPositionedForce(int, const mVec3 &, const mVec3 &);
    int GetCollisionMask(void) const;
    void OnSnappedTo(void);
    void GetVelocity(int, mVec3 *, mVec3 *) const;
    void InvalidateCacheEntries(eGeom *);
    void EnableWallWalk(bool);
    const cType *GetType(void) const;

    void CollectContact(eContactCollector *, eContact *, int);
    void ProcessContact(eContactCollector *, eContact *, int);
    void ProcessGroundOnlyContact(eContactCollector *, eContact *, int);

    static void CollectContactCallBack(eContactCollector *, void *, eContact *, int);
    static void ProcessContactCallBack(eContactCollector *, void *, eContact *, int);
    static void ProcessGroundOnlyContactCallBack(eContactCollector *, void *, eContact *, int);

    char _pad0[0x10];
    int bodyIndex;          // 0x10
    char _pad1[0x1C];       // 0x14-0x2F
    void *bodyEntries;      // 0x30
    char _pad2[0xE0];       // 0x34-0x113
    int collisionMask;      // 0x114
};

#endif
```

## Matched Same-Class Neighbors

- `0x0006487c` 76B `eBipedController::Write(cFile &) const` — `src/eMovie.cpp`
- `0x000648c8` 188B `eBipedController::Read(cFile &, cMemPool *)` — `src/eBipedController.cpp`
- `0x00064b1c` 276B `eBipedController::~eBipedController(void)` — `src/eBipedController.cpp`
- `0x00065ddc` 100B `eBipedController::SetPosition(int, const mVec3 &)` — `src/eBipedController.cpp`
- `0x00065e40` 84B `eBipedController::ApplyImpulse(int, const mVec3 &, const mVec3 &)` — `src/eBipedController.cpp`

## Matched Method Exemplars

### Exemplar 1: `eKeyframedController::GetLocalToWorld(int, mOCS *) const`

- Address: `0x00075128`
- Size: `40` bytes
- Source: `src/eKeyframedController.cpp`

```cpp
// eKeyframedController::GetLocalToWorld(int, mOCS *) const @ 0x00075128
void eKeyframedController::GetLocalToWorld(int, mOCS *out) const {
    v4sf_t row0;
    v4sf_t row1;
    v4sf_t row2;
    v4sf_t row3;
    __asm__ volatile(
        "vmidt.q M000\n"
        "vmov.q %0, C000\n"
        "vmov.q %1, C010\n"
        "vmov.q %2, C020\n"
        "vmov.q %3, C030\n"
        : "=v"(row0), "=v"(row1), "=v"(row2), "=v"(row3)
        :
        : "memory");
    v4sf_t *rows = (v4sf_t *)out;
    rows[0] = row0;
    rows[1] = row1;
    rows[2] = row2;
    rows[3] = row3;
}
```


## Prior Failure Notes

### Note 1

- session `02ef5779`; src `src/eBipedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/02ef5779/00065cf0__agent_self_reported_failure__src_eBipedController.cpp`

Closest source reconstructs the bodyEntries/bodyIndex validity check and both VFPU paths in C++ with v4sf_t stores. The identity path and body-matrix copy path use compiler-generated sv.q stores so return delay slots are filled correctly. Remaining mismatch is 18/132 bytes: SNC keeps the computed entry pointer in a3 and the boolean in a1, then emits bnez+nop to the copy path. Expected keeps the entry pointer in a1, boolean in a0, and emits bnezl with lw a0,0(a1) in the delay slot. Tried if/else inversion, explicit returns, memory barriers, volatile entry loads, sched=1/sched=2 pragmas, and register-bound entry/index locals. Permuter ran for 300 seconds, generated 6720 candidates, compiled 3410, improved its own baseline once but did not match.


## Disassembly

```asm
65cf0:	8c850010 	lw	a1,16(a0)
65cf4:	8c840030 	lw	a0,48(a0)
65cf8:	000528c0 	sll	a1,a1,0x3
65cfc:	00852821 	addu	a1,a0,a1
65d00:	8ca70000 	lw	a3,0(a1)
65d04:	10e00005 	beqz	a3,0x65d1c
65d08:	34040000 	li	a0,0x0
65d0c:	8ca70004 	lw	a3,4(a1)
65d10:	10e00003 	beqz	a3,0x65d20
65d14:	308400ff 	andi	a0,a0,0xff
65d18:	34040001 	li	a0,0x1
65d1c:	308400ff 	andi	a0,a0,0xff
65d20:	5480000b 	bnezl	a0,0x65d50
65d24:	8ca40000 	lw	a0,0(a1)
65d28:	f3838080 	vmidt.q     M000
65d2c:	d0008086 	vmov.q      C120, C000
65d30:	d0008187 	vmov.q      C130, C010
65d34:	d0008288 	vmov.q      C200, C020
65d38:	d0008389 	vmov.q      C210, C030
65d3c:	f8c60000 	sv.q        C120, 0x0($a2)
65d40:	f8c70010 	sv.q        C130, 0x10($a2)
65d44:	f8c80020 	sv.q        C200, 0x20($a2)
65d48:	03e00008 	jr	ra
65d4c:	f8c90030 	sv.q        C210, 0x30($a2)
65d50:	d8860030 	lv.q        C120, 0x30($a0)
65d54:	f8c60030 	sv.q        C120, 0x30($a2)
65d58:	d8860000 	lv.q        C120, 0x0($a0)
65d5c:	f8c60000 	sv.q        C120, 0x0($a2)
65d60:	d8860010 	lv.q        C120, 0x10($a0)
65d64:	f8c60010 	sv.q        C120, 0x10($a2)
65d68:	d8860020 	lv.q        C120, 0x20($a0)
65d6c:	03e00008 	jr	ra
65d70:	f8c60020 	sv.q        C120, 0x20($a2)
```

## m2c Starting Point

```c
// Decompiled: eBipedController::GetLocalToWorld(int, mOCS *) const
// Address: 0x00065cf0, Size: 132B
// Obj: eAll_psp.obj

void eBipedController__GetLocalToWorld_int_mOCSptrconst(void *arg0) {
    s32 var_a0;
    s32 var_a0_2;
    void *temp_a1;

    temp_a1 = arg0->unk30 + (arg0->unk10 * 8);
    var_a0 = 0;
    if (temp_a1->unk0 != 0) {
        var_a0_2 = 0 & 0xFF;
        if (temp_a1->unk4 != 0) {
            var_a0 = 1;
            goto block_3;
        }
    } else {
block_3:
        var_a0_2 = var_a0 & 0xFF;
    }
    if (var_a0_2 != 0) {
        __asm__ volatile("lv.q C120, 0x30($a0)");
        __asm__ volatile("sv.q C120, 0x30($a2)");
        __asm__ volatile("lv.q C120, ($a0)");
        __asm__ volatile("sv.q C120, ($a2)");
        __asm__ volatile("lv.q C120, 0x10($a0)");
        __asm__ volatile("sv.q C120, 0x10($a2)");
        __asm__ volatile("lv.q C120, 0x20($a0)");
        __asm__ volatile("sv.q C120, 0x20($a2)");
        return;
    }
    __asm__ volatile("vmidt.q M000");
    __asm__ volatile("vmov.q C120, C000");
    __asm__ volatile("vmov.q C130, C010");
    __asm__ volatile("vmov.q C200, C020");
    __asm__ volatile("vmov.q C210, C030");
    __asm__ volatile("sv.q C120, ($a2)");
    __asm__ volatile("sv.q C130, 0x10($a2)");
    __asm__ volatile("sv.q C200, 0x20($a2)");
    __asm__ volatile("sv.q C210, 0x30($a2)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
