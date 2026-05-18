# Battle Packet: `eBipedController::GetVelocity(int, const mVec3 &, mVec3 *) const`

## Target

- Address: `0x00065b54`
- Size: `104` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `GetVelocity`
- Leaf: `True`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 14B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `14`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
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

- session `1f5da794`; src `src/eBipedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/1f5da794/00065b54__agent_self_reported_failure__src_eBipedController.cpp`

Closest attempt is in src/eBipedController.cpp. It uses the matched ApplyImpulse-style body entry check, volatile entry loads to force the original reload of entry[0], and an inline VFPU zero-return tail because compiler-generated V4SF output otherwise inserts an extra VFPU move before mtc1. Current compare_func result is 14/104 bytes different. The scalar prefix matches through the bnezl delay-slot reload; remaining mismatch is in the zero/true tail layout caused by the inline asm return and SNC keeping a dead reload/fallthrough shape around the true C vector store.

Tried C true/false result assignment, goto false-path-first, volatile entry pointers, register-bound VFPU result, hardcoded delay-slot stores, hardcoded GPR zero moves, and an inline true tail. The best semantically clean variant is the current one; a 600s permuter run was attempted but tools/permuter.py aborted before search because it could not isolate this 104-byte target from the compiled multi-function object.


## Disassembly

```asm
65b54:	8c850010 	lw	a1,16(a0)
65b58:	8c840030 	lw	a0,48(a0)
65b5c:	000528c0 	sll	a1,a1,0x3
65b60:	00852821 	addu	a1,a0,a1
65b64:	8ca60000 	lw	a2,0(a1)
65b68:	10c00005 	beqz	a2,0x65b80
65b6c:	34040000 	li	a0,0x0
65b70:	8ca60004 	lw	a2,4(a1)
65b74:	10c00003 	beqz	a2,0x65b84
65b78:	308400ff 	andi	a0,a0,0xff
65b7c:	34040001 	li	a0,0x1
65b80:	308400ff 	andi	a0,a0,0xff
65b84:	5480000a 	bnezl	a0,0x65bb0
65b88:	8ca40000 	lw	a0,0(a1)
65b8c:	44806000 	mtc1	zero,$f12
65b90:	44046000 	mfc1	a0,$f12
65b94:	44056000 	mfc1	a1,$f12
65b98:	44066000 	mfc1	a2,$f12
65b9c:	48e40006 	mtv         $a0, S120
65ba0:	48e50026 	mtv         $a1, S121
65ba4:	48e60046 	mtv         $a2, S122
65ba8:	03e00008 	jr	ra
65bac:	f8e60000 	sv.q        C120, 0x0($a3)
65bb0:	d8860040 	lv.q        C120, 0x40($a0)
65bb4:	03e00008 	jr	ra
65bb8:	f8e60000 	sv.q        C120, 0x0($a3)
```

## m2c Starting Point

```c
// Decompiled: eBipedController::GetVelocity(int, const mVec3 &, mVec3 *) const
// Address: 0x00065b54, Size: 104B
// Obj: eAll_psp.obj

void eBipedController__GetVelocity_int_constmVec3ref_mVec3ptrconst(void *arg0) {
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
        __asm__ volatile("lv.q C120, 0x40($a0)");
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
