# Battle Packet: `eBipedController::PostUpdate(mOCS *, eDynamicModel *) const`

## Target

- Address: `0x00065454`
- Size: `440` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBipedController`
- Method family: `PostUpdate`
- Leaf: `True`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 5B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `5`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `5`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

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

### Exemplar 1: `eKeyframedController::PostUpdate(mOCS *, eDynamicModel *) const`

- Address: `0x00075550`
- Size: `8` bytes
- Source: `src/eKeyframedController_PostUpdate.cpp`

```cpp
void eKeyframedController::PostUpdate(mOCS *, eDynamicModel *) const {
}
```

### Exemplar 2: `gcMap::PostUpdate(void)`

- Address: `0x000f38fc`
- Size: `28` bytes
- Source: `src/gcMap.cpp`

```cpp
void gcMap::PostUpdate(void) {
    HandleCinematicSkip();
}
```


## Prior Failure Notes

### Note 1

- session `21dc2e56`; src `src/eBipedController.cpp`; snapshot `logs/failure_snapshots/match_20260430_201648/21dc2e56/00065454__agent_self_reported_failure__src_eBipedController.cpp`

Reconstructed `eBipedController::PostUpdate(mOCS *, eDynamicModel *) const` in `src/eBipedController.cpp` using real C++ control flow and native VFPU quad copies for the simple path. The current best source matches the expected 440-byte size and is at `MISMATCH: 5/440 bytes differ` under `python3 tools/compare_func.py --symbol __0fQeBipedControllerKPostUpdateP6EmOCSP6NeDynamicModelK src/eBipedController.cpp`. The stack frame, sched=1 prologue, body-entry validation, inactive flag checks, direct `mOCS` copies, scratch stack materialization, and transform-heavy VFPU sequence all line up; the remaining difference is isolated to the dynamic-model update callback thunk in the `flags & 0x20` branch.

The target thunk uses `lw a0,4(s0); addiu a1,a0,0xB8; lh a0,0(a1); lw a1,4(a1); jalr a1; addu a0,s0,a0`. The best C form still emits the same logic with the record pointer in `a0`, the adjustment in `a1`, and the function pointer in `a2`: `addiu a0,a0,0xB8; lh a1,0(a0); lw a2,4(a0); jalr a2; addu a0,s0,a1`. I tried direct-expression dispatch, `int *` vs typed record access, reordered `fn`/`off` loads, volatile record pointers, memory barriers around the thunk, explicit register variables, and an exact inline-asm thunk. The exact asm locally fixed the thunk but changed surrounding register allocation and reduced the function to a 428-byte symbol, so it was reverted. A 300s permuter run generated 6600 candidates with zero improvement, though its single-function path reported a 21-byte baseline because it did not preserve this function's local `#pragma control sched=1` shape. This is not a `bnel`/`beql` divergence; it remains retryable register-allocation tuning on the callback subexpression.

### Note 2

- session `e0fd4559`

Implemented `PostUpdate(mOCS *, eDynamicModel *) const` with real C++ control flow plus VFPU inline asm only for the transform-heavy path. I reconstructed the body-entry validation, the `inactiveFlag`/`flags & 0x20` checks, the dynamic-model callback gate on `model+0x8C`, the direct quad copies for the simple branch, and the 0x80-byte scratch stack used by the transform branch. The major breakthrough was forcing the scratch area to materialize as `volatile eBipedOCSLocal scratch[2]`, which brought the prologue, stack frame, and VFPU sequence into alignment.

The best state reached `MISMATCH: 5/440 bytes differ`. At that point the entire function matched except for the tiny callback thunk in the `flags & 0x20` branch: the target uses `lw a1,4(a1)` / `jalr a1`, while the current C shape still makes SNC pick `a2` for the function pointer. I tried both a direct-expression rewrite and an exact inline-asm thunk; both made the function materially worse, so I reverted to the current 5-byte miss. This is not a sched-blocker and not a `bnel` case. The next retry should start from the current version and focus only on that callback subexpression, ideally finding a source form that preserves the existing 5-byte match state while moving the thunk back onto `$a1`.


## Disassembly

```asm
65454:	27bdff70 	addiu	sp,sp,-144
65458:	afb00080 	sw	s0,128(sp)
6545c:	afb10084 	sw	s1,132(sp)
65460:	afbf0088 	sw	ra,136(sp)
65464:	00a08825 	move	s1,a1
65468:	8c850010 	lw	a1,16(a0)
6546c:	04a00062 	bltz	a1,0x655f8
65470:	00c08025 	move	s0,a2
65474:	8c840030 	lw	a0,48(a0)
65478:	10800005 	beqz	a0,0x65490
6547c:	34060000 	li	a2,0x0
65480:	8c86fffc 	lw	a2,-4(a0)
65484:	3c074000 	lui	a3,0x4000
65488:	24e7ffff 	addiu	a3,a3,-1
6548c:	00c73024 	and	a2,a2,a3
65490:	00a6302a 	slt	a2,a1,a2
65494:	10c00058 	beqz	a2,0x655f8
65498:	000528c0 	sll	a1,a1,0x3
6549c:	00852821 	addu	a1,a0,a1
654a0:	8ca60000 	lw	a2,0(a1)
654a4:	10c00005 	beqz	a2,0x654bc
654a8:	34040000 	li	a0,0x0
654ac:	8ca60004 	lw	a2,4(a1)
654b0:	10c00003 	beqz	a2,0x654c0
654b4:	308400ff 	andi	a0,a0,0xff
654b8:	34040001 	li	a0,0x1
654bc:	308400ff 	andi	a0,a0,0xff
654c0:	1080004d 	beqz	a0,0x655f8
654c4:	00000000 	nop
654c8:	8ca50000 	lw	a1,0(a1)
654cc:	8ca400a4 	lw	a0,164(a1)
654d0:	8c840070 	lw	a0,112(a0)
654d4:	14800048 	bnez	a0,0x655f8
654d8:	00000000 	nop
654dc:	94a40098 	lhu	a0,152(a1)
654e0:	30840020 	andi	a0,a0,0x20
654e4:	10800014 	beqz	a0,0x65538
654e8:	00000000 	nop
654ec:	9204008c 	lbu	a0,140(s0)
654f0:	30840004 	andi	a0,a0,0x4
654f4:	10800007 	beqz	a0,0x65514
654f8:	00000000 	nop
654fc:	8e040004 	lw	a0,4(s0)
65500:	248500b8 	addiu	a1,a0,184
65504:	84a40000 	lh	a0,0(a1)
65508:	8ca50004 	lw	a1,4(a1)
6550c:	00a0f809 	jalr	a1
65510:	02042021 	addu	a0,s0,a0
65514:	da060040 	lv.q        C120, 0x40($s0)
65518:	fa260030 	sv.q        C120, 0x30($s1)
6551c:	da060010 	lv.q        C120, 0x10($s0)
65520:	fa260000 	sv.q        C120, 0x0($s1)
65524:	da060020 	lv.q        C120, 0x20($s0)
65528:	fa260010 	sv.q        C120, 0x10($s1)
6552c:	da060030 	lv.q        C120, 0x30($s0)
65530:	10000031 	b	0x655f8
65534:	fa260020 	sv.q        C120, 0x20($s1)
65538:	d8a60000 	lv.q        C120, 0x0($a1)
6553c:	fba60000 	sv.q        C120, 0x0($sp)
65540:	d8a60010 	lv.q        C120, 0x10($a1)
65544:	fba60010 	sv.q        C120, 0x10($sp)
65548:	d8a60020 	lv.q        C120, 0x20($a1)
6554c:	fba60020 	sv.q        C120, 0x20($sp)
65550:	d8a60030 	lv.q        C120, 0x30($a1)
65554:	fba60030 	sv.q        C120, 0x30($sp)
65558:	8ca400a4 	lw	a0,164(a1)
6555c:	24840010 	addiu	a0,a0,16
65560:	d8800000 	lv.q        C000, 0x0($a0)
65564:	d8810010 	lv.q        C010, 0x10($a0)
65568:	d8820020 	lv.q        C020, 0x20($a0)
6556c:	d8830030 	lv.q        C030, 0x30($a0)
65570:	d000a006 	vmov.t      C120, R000
65574:	d000a107 	vmov.t      C130, R001
65578:	d000a208 	vmov.t      C200, R002
6557c:	fba60040 	sv.q        C120, 0x40($sp)
65580:	fba70050 	sv.q        C130, 0x50($sp)
65584:	fba80060 	sv.q        C200, 0x60($sp)
65588:	d8890030 	lv.q        C210, 0x30($a0)
6558c:	d0028909 	vneg.t      C210, C210
65590:	fba90070 	sv.q        C210, 0x70($sp)
65594:	27a40040 	addiu	a0,sp,64
65598:	d8800000 	lv.q        C000, 0x0($a0)
6559c:	d8810010 	lv.q        C010, 0x10($a0)
655a0:	d8820020 	lv.q        C020, 0x20($a0)
655a4:	d8830030 	lv.q        C030, 0x30($a0)
655a8:	f109a00a 	vtfm3.t     C220, E000, C210
655ac:	fbaa0070 	sv.q        C220, 0x70($sp)
655b0:	dba00000 	lv.q        C000, 0x0($sp)
655b4:	dba10010 	lv.q        C010, 0x10($sp)
655b8:	dba20020 	lv.q        C020, 0x20($sp)
655bc:	dba30030 	lv.q        C030, 0x30($sp)
655c0:	f106a009 	vtfm3.t     C210, E000, C120
655c4:	f107a006 	vtfm3.t     C120, E000, C130
655c8:	f108a007 	vtfm3.t     C130, E000, C200
655cc:	f18aa008 	vhtfm4.q    C200, E000, C220
655d0:	fa290000 	sv.q        C210, 0x0($s1)
655d4:	fa260010 	sv.q        C120, 0x10($s1)
655d8:	fa270020 	sv.q        C130, 0x20($s1)
655dc:	fa280030 	sv.q        C200, 0x30($s1)
655e0:	d8a60000 	lv.q        C120, 0x0($a1)
655e4:	fa260000 	sv.q        C120, 0x0($s1)
655e8:	d8a60010 	lv.q        C120, 0x10($a1)
655ec:	fa260010 	sv.q        C120, 0x10($s1)
655f0:	d8a60020 	lv.q        C120, 0x20($a1)
655f4:	fa260020 	sv.q        C120, 0x20($s1)
655f8:	8fb00080 	lw	s0,128(sp)
655fc:	8fb10084 	lw	s1,132(sp)
65600:	8fbf0088 	lw	ra,136(sp)
65604:	03e00008 	jr	ra
65608:	27bd0090 	addiu	sp,sp,144
```

## m2c Starting Point

```c
// Decompiled: eBipedController::PostUpdate(mOCS *, eDynamicModel *) const
// Address: 0x00065454, Size: 440B
// Obj: eAll_psp.obj

void eBipedController__PostUpdate_mOCSptr_eDynamicModelptrconst(void *arg0, ? arg1, void *arg2, ? arg3) {
    ? sp40;
    ? (*temp_a1_4)(void *, ?, void *, ?);
    ? var_a3;
    s32 temp_a1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a1_2;
    void *temp_a1_3;
    void *var_a2_2;

    var_a3 = arg3;
    temp_a1 = arg0->unk10;
    if (temp_a1 >= 0) {
        temp_a0 = arg0->unk30;
        var_a2 = 0;
        if (temp_a0 != NULL) {
            var_a3 = 0x3FFFFFFF;
            var_a2 = temp_a0->unk-4 & 0x3FFFFFFF;
        }
        if (temp_a1 < var_a2) {
            temp_a1_2 = temp_a0 + (temp_a1 * 8);
            var_a2_2 = temp_a1_2->unk0;
            var_a0 = 0;
            if (var_a2_2 != NULL) {
                var_a2_2 = temp_a1_2->unk4;
                var_a0_2 = 0 & 0xFF;
                if (var_a2_2 != NULL) {
                    var_a0 = 1;
                    goto block_7;
                }
            } else {
block_7:
                var_a0_2 = var_a0 & 0xFF;
            }
            if (var_a0_2 != 0) {
                temp_a1_3 = temp_a1_2->unk0;
                if (temp_a1_3->unkA4->unk70 == 0) {
                    if (temp_a1_3->unk98 & 0x20) {
                        if (arg2->unk8C & 4) {
                            temp_a0_2 = arg2->unk4;
                            temp_a1_4 = (temp_a0_2 + 0xB8)->unk4;
                            temp_a1_4(arg2 + temp_a0_2->unkB8, temp_a1_4, var_a2_2, var_a3);
                        }
                        __asm__ volatile("lv.q C120, 0x40($s0)");
                        __asm__ volatile("sv.q C120, 0x30($s1)");
                        __asm__ volatile("lv.q C120, 0x10($s0)");
                        __asm__ volatile("sv.q C120, ($s1)");
                        __asm__ volatile("lv.q C120, 0x20($s0)");
                        __asm__ volatile("sv.q C120, 0x10($s1)");
                        __asm__ volatile("lv.q C120, 0x30($s0)");
                    } else {
                        __asm__ volatile("lv.q C120, ($a1)");
                        __asm__ volatile("sv.q C120, ($sp)");
                        __asm__ volatile("lv.q C120, 0x10($a1)");
                        __asm__ volatile("sv.q C120, 0x10($sp)");
                        __asm__ volatile("lv.q C120, 0x20($a1)");
                        __asm__ volatile("sv.q C120, 0x20($sp)");
                        __asm__ volatile("lv.q C120, 0x30($a1)");
                        __asm__ volatile("sv.q C120, 0x30($sp)");
                        __asm__ volatile("lv.q C000, ($a0)");
                        __asm__ volatile("lv.q C010, 0x10($a0)");
                        __asm__ volatile("lv.q C020, 0x20($a0)");
                        __asm__ volatile("lv.q C030, 0x30($a0)");
                        __asm__ volatile("vmov.t C120, R000");
                        __asm__ volatile("vmov.t C130, R001");
                        __asm__ volatile("vmov.t C200, R002");
                        __asm__ volatile("sv.q C120, 0x40($sp)");
                        __asm__ volatile("sv.q C130, 0x50($sp)");
                        __asm__ volatile("sv.q C200, 0x60($sp)");
                        __asm__ volatile("lv.q C210, 0x30($a0)");
                        __asm__ volatile("vneg.t C210, C210");
                        __asm__ volatile("sv.q C210, 0x70($sp)");
                        __asm__ volatile("lv.q C000, ($a0)");
                        __asm__ volatile("lv.q C010, 0x10($a0)");
                        __asm__ volatile("lv.q C020, 0x20($a0)");
                        __asm__ volatile("lv.q C030, 0x30($a0)");
                        __asm__ volatile("vtfm3.t C220, E000, C210");
                        __asm__ volatile("sv.q C220, 0x70($sp)");
                        __asm__ volatile("lv.q C000, ($sp)");
                        __asm__ volatile("lv.q C010, 0x10($sp)");
                        __asm__ volatile("lv.q C020, 0x20($sp)");
                        __asm__ volatile("lv.q C030, 0x30($sp)");
                        __asm__ volatile("vtfm3.t C210, E000, C120");
                        __asm__ volatile("vtfm3.t C120, E000, C130");
                        __asm__ volatile("vtfm3.t C130, E000, C200");
                        __asm__ volatile("vhtfm4.q C200, E000, C220");
                        __asm__ volatile("sv.q C210, ($s1)");
                        __asm__ volatile("sv.q C120, 0x10($s1)");
                        __asm__ volatile("sv.q C130, 0x20($s1)");
                        __asm__ volatile("sv.q C200, 0x30($s1)");
                        __asm__ volatile("lv.q C120, ($a1)");
                        __asm__ volatile("sv.q C120, ($s1)");
                        __asm__ volatile("lv.q C120, 0x10($a1)");
                        __asm__ volatile("sv.q C120, 0x10($s1)");
                        __asm__ volatile("lv.q C120, 0x20($a1)");
                    }
                    __asm__ volatile("sv.q C120, 0x20($s1)");
                }
            }
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
