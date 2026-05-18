# Battle Packet: `eBipedController::GetPosition(int, mVec3 *) const`

## Target

- Address: `0x00065d74`
- Size: `104` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `GetPosition`
- Leaf: `True`

## Queue Metadata

- score: `167`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 14B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `14`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `14`
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

## Prior Failure Notes

### Note 1

- session `1f5da794`; src `src/eBipedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/1f5da794/00065d74__agent_self_reported_failure__src_eBipedController.cpp`

Closest attempt mirrors GetVelocity with the body vector offset changed to 0x30 and the output pointer in a2. Current compare_func result is 14/104 bytes different. The entry computation, null checks, flag register, and branch-likely reload shape were reproduced with index/bodyEntries temporaries plus volatile entry loads; remaining bytes differ in the same zero/true tail scheduling area as GetVelocity.

Tried the same variants as GetVelocity: whole V4SF result assignment, goto restructuring, volatile entry loads, register-bound VFPU result, hardcoded zero-path delay-slot store, hardcoded GPR moves, and inline true-tail asm. The target appears to need the exact compiler-generated sv.q delay-slot behavior without SNC's extra VFPU/register-preservation artifacts. Permuter could not run because it failed to locate the function in the compiled object before starting the search.


## Disassembly

```asm
65d74:	8c850010 	lw	a1,16(a0)
65d78:	8c840030 	lw	a0,48(a0)
65d7c:	000528c0 	sll	a1,a1,0x3
65d80:	00852821 	addu	a1,a0,a1
65d84:	8ca70000 	lw	a3,0(a1)
65d88:	10e00005 	beqz	a3,0x65da0
65d8c:	34040000 	li	a0,0x0
65d90:	8ca70004 	lw	a3,4(a1)
65d94:	10e00003 	beqz	a3,0x65da4
65d98:	308400ff 	andi	a0,a0,0xff
65d9c:	34040001 	li	a0,0x1
65da0:	308400ff 	andi	a0,a0,0xff
65da4:	5480000a 	bnezl	a0,0x65dd0
65da8:	8ca40000 	lw	a0,0(a1)
65dac:	44806000 	mtc1	zero,$f12
65db0:	44046000 	mfc1	a0,$f12
65db4:	44056000 	mfc1	a1,$f12
65db8:	44076000 	mfc1	a3,$f12
65dbc:	48e40006 	mtv         $a0, S120
65dc0:	48e50026 	mtv         $a1, S121
65dc4:	48e70046 	mtv         $a3, S122
65dc8:	03e00008 	jr	ra
65dcc:	f8c60000 	sv.q        C120, 0x0($a2)
65dd0:	d8860030 	lv.q        C120, 0x30($a0)
65dd4:	03e00008 	jr	ra
65dd8:	f8c60000 	sv.q        C120, 0x0($a2)
```

## m2c Starting Point

```c
// Decompiled: eBipedController::GetPosition(int, mVec3 *) const
// Address: 0x00065d74, Size: 104B
// Obj: eAll_psp.obj

void eBipedController__GetPosition_int_mVec3ptrconst(void *arg0) {
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
        __asm__ volatile("sv.q C120, ($a2)");
        return;
    }
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a3, S122");
    __asm__ volatile("sv.q C120, ($a2)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
