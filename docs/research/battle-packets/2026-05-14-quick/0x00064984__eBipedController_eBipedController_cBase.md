# Battle Packet: `eBipedController::eBipedController(cBase *)`

## Target

- Address: `0x00064984`
- Size: `408` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `111`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 19B; Constructor retry family; manageable size`
- failure_action: `prep`
- near_miss_bytes: `19`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `19`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x00046c48` `matched` 32B `ePhysicsController::ePhysicsController(cBase *)`
- `0x001b9c64` `untried` 68B `__vec_new`

Callers:
- `0x00208ddc` `matched` 124B `eBipedController::New(cMemPool *, cBase *) static`

## Placement

- Canonical source: `src/eBipedController.cpp`
- Header(s): `include/eBipedController.h`
- Allowed source(s): `src/eBipedController.cpp`
- Split-TU prefix: `src/eBipedController_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

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

- session `2b8ba537`

Implemented the constructor as canonical C++ with an ePhysicsController base initializer, class-local layout, placement-new use from New, real field stores, __vec_new for the saved contacts array, and VFPU inline asm only for the vector constant groups/matrix copies. Tried sched=1 and sched=2, native V4SF matrix struct copies, scalar C initialization, hardcoded VFPU stack stores, explicit D_00038568/D_00038538 accesses, and local matrix temporaries to force the original stack frame. Best result is 19/408 bytes different; the remaining mismatch is concentrated in the static identity matrix setup/copy region and prologue side effects. The best hand-shaped VFPU version gets the instruction count right but SNC does not allocate the original 0x70 frame when the stack locals are only referenced through inline asm, causing prologue/epilogue and a few address-register bytes to differ. The version that forces the 0x70 frame through native C locals regresses to scalar swc1 initialization and about 90 bytes of diff. The permuter ran for 301 seconds (8424 generated, 3944 compiled) with no improvement beyond the 19-byte baseline.


## Disassembly

```asm
64984:	27bdff90 	addiu	sp,sp,-112
64988:	afb00060 	sw	s0,96(sp)
6498c:	afbf0064 	sw	ra,100(sp)
64990:	0c011b12 	jal	0x46c48
64994:	00808025 	move	s0,a0
64998:	3c040038 	lui	a0,0x38
6499c:	24842d58 	addiu	a0,a0,11608
649a0:	ae040004 	sw	a0,4(s0)
649a4:	ae000010 	sw	zero,16(s0)
649a8:	44806000 	mtc1	zero,$f12
649ac:	44046000 	mfc1	a0,$f12
649b0:	44056000 	mfc1	a1,$f12
649b4:	3c06bf80 	lui	a2,0xbf80
649b8:	44866800 	mtc1	a2,$f13
649bc:	44066800 	mfc1	a2,$f13
649c0:	48e40006 	mtv         $a0, S120
649c4:	48e50026 	mtv         $a1, S121
649c8:	48e60046 	mtv         $a2, S122
649cc:	fa060020 	sv.q        C120, 0x20($s0)
649d0:	ae000030 	sw	zero,48(s0)
649d4:	a2000034 	sb	zero,52(s0)
649d8:	a2000035 	sb	zero,53(s0)
649dc:	ae000038 	sw	zero,56(s0)
649e0:	a200003c 	sb	zero,60(s0)
649e4:	e60c0040 	swc1	$f12,64(s0)
649e8:	ae000044 	sw	zero,68(s0)
649ec:	ae000048 	sw	zero,72(s0)
649f0:	e60c0090 	swc1	$f12,144(s0)
649f4:	44046000 	mfc1	a0,$f12
649f8:	44056000 	mfc1	a1,$f12
649fc:	44066000 	mfc1	a2,$f12
64a00:	48e40006 	mtv         $a0, S120
64a04:	48e50026 	mtv         $a1, S121
64a08:	48e60046 	mtv         $a2, S122
64a0c:	fa0600a0 	sv.q        C120, 0xA0($s0)
64a10:	3c060004 	lui	a2,0x4
64a14:	8cc78568 	lw	a3,-31384(a2)
64a18:	3c050004 	lui	a1,0x4
64a1c:	14e0001d 	bnez	a3,0x64a94
64a20:	24a48538 	addiu	a0,a1,-31432
64a24:	34070001 	li	a3,0x1
64a28:	acc78568 	sw	a3,-31384(a2)
64a2c:	3c063f80 	lui	a2,0x3f80
64a30:	44866800 	mtc1	a2,$f13
64a34:	44066800 	mfc1	a2,$f13
64a38:	44076000 	mfc1	a3,$f12
64a3c:	44086000 	mfc1	t0,$f12
64a40:	48e60006 	mtv         $a2, S120
64a44:	48e70026 	mtv         $a3, S121
64a48:	48e80046 	mtv         $t0, S122
64a4c:	fba60030 	sv.q        C120, 0x30($sp)
64a50:	44066000 	mfc1	a2,$f12
64a54:	44076800 	mfc1	a3,$f13
64a58:	44086000 	mfc1	t0,$f12
64a5c:	48e60007 	mtv         $a2, S130
64a60:	48e70027 	mtv         $a3, S131
64a64:	48e80047 	mtv         $t0, S132
64a68:	fba70040 	sv.q        C130, 0x40($sp)
64a6c:	44066000 	mfc1	a2,$f12
64a70:	44076000 	mfc1	a3,$f12
64a74:	44086800 	mfc1	t0,$f13
64a78:	48e60008 	mtv         $a2, S200
64a7c:	48e70028 	mtv         $a3, S201
64a80:	48e80048 	mtv         $t0, S202
64a84:	fba80050 	sv.q        C200, 0x50($sp)
64a88:	f8a68538 	sv.q        C120, -0x7AC8($a1)
64a8c:	f8870010 	sv.q        C130, 0x10($a0)
64a90:	f8880020 	sv.q        C200, 0x20($a0)
64a94:	d8a68538 	lv.q        C120, -0x7AC8($a1)
64a98:	fba60000 	sv.q        C120, 0x0($sp)
64a9c:	d8870010 	lv.q        C130, 0x10($a0)
64aa0:	fba70010 	sv.q        C130, 0x10($sp)
64aa4:	d8870020 	lv.q        C130, 0x20($a0)
64aa8:	fba70020 	sv.q        C130, 0x20($sp)
64aac:	fa0600b0 	sv.q        C120, 0xB0($s0)
64ab0:	dba60010 	lv.q        C120, 0x10($sp)
64ab4:	fa0600c0 	sv.q        C120, 0xC0($s0)
64ab8:	dba60020 	lv.q        C120, 0x20($sp)
64abc:	fa0600d0 	sv.q        C120, 0xD0($s0)
64ac0:	a20000e0 	sb	zero,224(s0)
64ac4:	44046000 	mfc1	a0,$f12
64ac8:	44056000 	mfc1	a1,$f12
64acc:	44066000 	mfc1	a2,$f12
64ad0:	48e40006 	mtv         $a0, S120
64ad4:	48e50026 	mtv         $a1, S121
64ad8:	48e60046 	mtv         $a2, S122
64adc:	fa0600f0 	sv.q        C120, 0xF0($s0)
64ae0:	e60c0110 	swc1	$f12,272(s0)
64ae4:	ae000120 	sw	zero,288(s0)
64ae8:	26040130 	addiu	a0,s0,304
64aec:	3c070021 	lui	a3,0x21
64af0:	24e78928 	addiu	a3,a3,-30424
64af4:	34050008 	li	a1,0x8
64af8:	0c06e719 	jal	0x1b9c64
64afc:	34060030 	li	a2,0x30
64b00:	ae0002b0 	sw	zero,688(s0)
64b04:	ae0002b4 	sw	zero,692(s0)
64b08:	02001025 	move	v0,s0
64b0c:	8fb00060 	lw	s0,96(sp)
64b10:	8fbf0064 	lw	ra,100(sp)
64b14:	03e00008 	jr	ra
64b18:	27bd0070 	addiu	sp,sp,112
```

## m2c Starting Point

```c
// Decompiled: eBipedController::eBipedController(cBase *)
// Address: 0x00064984, Size: 408B
// Obj: eAll_psp.obj

? __vec_new(?, void *, ?, ?, ?);                    /* extern */
? ePhysicsController__ePhysicsController_cBaseptr(); /* extern */

void *eBipedController__eBipedController_cBaseptr(void *arg0) {
    ePhysicsController__ePhysicsController_cBaseptr();
    arg0->unk4 = 0x382D58;
    arg0->unk10 = 0;
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, 0x20($s0)");
    arg0->unk30 = 0;
    arg0->unk34 = 0;
    arg0->unk35 = 0;
    arg0->unk38 = 0;
    arg0->unk3C = 0;
    arg0->unk40 = 0.0f;
    arg0->unk44 = 0;
    arg0->unk48 = 0;
    arg0->unk90 = 0.0f;
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, 0xa0($s0)");
    if (*(s32 *)0x38568 == 0) {
        *(s32 *)0x38568 = 1;
        __asm__ volatile("mtv $a2, S120");
        __asm__ volatile("mtv $a3, S121");
        __asm__ volatile("mtv $t0, S122");
        __asm__ volatile("sv.q C120, 0x30($sp)");
        __asm__ volatile("mtv $a2, S130");
        __asm__ volatile("mtv $a3, S131");
        __asm__ volatile("mtv $t0, S132");
        __asm__ volatile("sv.q C130, 0x40($sp)");
        __asm__ volatile("mtv $a2, S200");
        __asm__ volatile("mtv $a3, S201");
        __asm__ volatile("mtv $t0, S202");
        __asm__ volatile("sv.q C200, 0x50($sp)");
        __asm__ volatile("sv.q C120, -0x7ac8($a1)");
        __asm__ volatile("sv.q C130, 0x10($a0)");
        __asm__ volatile("sv.q C200, 0x20($a0)");
    }
    __asm__ volatile("lv.q C120, -0x7ac8($a1)");
    __asm__ volatile("sv.q C120, ($sp)");
    __asm__ volatile("lv.q C130, 0x10($a0)");
    __asm__ volatile("sv.q C130, 0x10($sp)");
    __asm__ volatile("lv.q C130, 0x20($a0)");
    __asm__ volatile("sv.q C130, 0x20($sp)");
    __asm__ volatile("sv.q C120, 0xb0($s0)");
    __asm__ volatile("lv.q C120, 0x10($sp)");
    __asm__ volatile("sv.q C120, 0xc0($s0)");
    __asm__ volatile("lv.q C120, 0x20($sp)");
    __asm__ volatile("sv.q C120, 0xd0($s0)");
    arg0->unkE0 = 0;
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, 0xf0($s0)");
    arg0->unk110 = 0.0f;
    arg0->unk120 = 0;
    __vec_new(0, arg0 + 0x130, 8, 0x30, 0x208928);
    arg0->unk2B0 = 0;
    arg0->unk2B4 = 0;
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
