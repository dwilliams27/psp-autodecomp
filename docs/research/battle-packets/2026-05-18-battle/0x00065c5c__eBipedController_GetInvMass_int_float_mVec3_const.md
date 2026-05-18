# Battle Packet: `eBipedController::GetInvMass(int, float *, mVec3 *) const`

## Target

- Address: `0x00065c5c`
- Size: `148` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `GetInvMass`
- Leaf: `True`

## Queue Metadata

- score: `124`
- priority: `1`
- reason: `has failure_notes; has failure snapshot; near miss 30B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `30`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `30`
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

- session `02ef5779`; src `src/eBipedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/02ef5779/00065c5c__agent_self_reported_failure__src_eBipedController.cpp`

Closest source matches the initial zero-vector store, entry validity test, branch-likely shape, and true mass path instruction order. The remaining mismatch is 30/148 bytes. The false path needs swc1/mfc1/mtv followed by jr with sv.q in the delay slot; a direct inline VFPU return produces those bytes, but SNC still emits an unreachable compiler jr+nop before the true mass block, shifting the branch target. Tried compiler-generated v4sf_t zero stores, eBipedVec4 whole-struct copy, direct inline sv.q, goto-based block layout, explicit if/else returns, volatile entry loads, and sched=1/sched=2. The permuter could not run this target because it could not locate the compiled symbol with its reduced flag set; manual attempts did not get below the 30-byte diff.


## Disassembly

```asm
65c5c:	44806000 	mtc1	zero,$f12
65c60:	44056000 	mfc1	a1,$f12
65c64:	44086000 	mfc1	t0,$f12
65c68:	44096000 	mfc1	t1,$f12
65c6c:	48e50006 	mtv         $a1, S120
65c70:	48e80026 	mtv         $t0, S121
65c74:	48e90046 	mtv         $t1, S122
65c78:	f8e60000 	sv.q        C120, 0x0($a3)
65c7c:	8c850010 	lw	a1,16(a0)
65c80:	8c840030 	lw	a0,48(a0)
65c84:	000528c0 	sll	a1,a1,0x3
65c88:	00852821 	addu	a1,a0,a1
65c8c:	8ca80000 	lw	t0,0(a1)
65c90:	11000005 	beqz	t0,0x65ca8
65c94:	34040000 	li	a0,0x0
65c98:	8ca80004 	lw	t0,4(a1)
65c9c:	11000003 	beqz	t0,0x65cac
65ca0:	308400ff 	andi	a0,a0,0xff
65ca4:	34040001 	li	a0,0x1
65ca8:	308400ff 	andi	a0,a0,0xff
65cac:	5480000a 	bnezl	a0,0x65cd8
65cb0:	8ca40004 	lw	a0,4(a1)
65cb4:	e4cc0000 	swc1	$f12,0(a2)
65cb8:	44046000 	mfc1	a0,$f12
65cbc:	44056000 	mfc1	a1,$f12
65cc0:	44066000 	mfc1	a2,$f12
65cc4:	48e40006 	mtv         $a0, S120
65cc8:	48e50026 	mtv         $a1, S121
65ccc:	48e60046 	mtv         $a2, S122
65cd0:	03e00008 	jr	ra
65cd4:	f8e60000 	sv.q        C120, 0x0($a3)
65cd8:	c48c0020 	lwc1	$f12,32(a0)
65cdc:	3c043f80 	lui	a0,0x3f80
65ce0:	44846800 	mtc1	a0,$f13
65ce4:	460c6b03 	div.s	$f12,$f13,$f12
65ce8:	03e00008 	jr	ra
65cec:	e4cc0000 	swc1	$f12,0(a2)
```

## m2c Starting Point

```c
// Decompiled: eBipedController::GetInvMass(int, float *, mVec3 *) const
// Address: 0x00065c5c, Size: 148B
// Obj: eAll_psp.obj

void eBipedController__GetInvMass_int_floatptr_mVec3ptrconst(void *arg0, f32 *arg2) {
    s32 var_a0;
    s32 var_a0_2;
    void *temp_a1;

    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $t0, S121");
    __asm__ volatile("mtv $t1, S122");
    __asm__ volatile("sv.q C120, ($a3)");
    temp_a1 = arg0->unk30 + (arg0->unk10 * 8);
    var_a0 = 0;
    if (temp_a1->unk0 != 0) {
        var_a0_2 = 0 & 0xFF;
        if (temp_a1->unk4 != NULL) {
            var_a0 = 1;
            goto block_3;
        }
    } else {
block_3:
        var_a0_2 = var_a0 & 0xFF;
    }
    if (var_a0_2 != 0) {
        *arg2 = 1.0f / temp_a1->unk4->unk20;
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
