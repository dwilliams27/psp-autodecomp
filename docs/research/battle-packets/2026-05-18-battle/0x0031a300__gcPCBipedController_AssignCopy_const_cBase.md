# Battle Packet: `gcPCBipedController::AssignCopy(const cBase *)`

## Target

- Address: `0x0031a300`
- Size: `456` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcPCBipedController`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `155`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 15B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `15`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `15`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0022b310` `untried` 528B `dcast dcast<gcPCBipedController *>(const cBase *)`

## Placement

- Canonical source: `src/gcPCBipedController.cpp`
- Header(s): `include/gcPCBipedController.h`
- Allowed source(s): `src/gcPCBipedController.cpp`
- Split-TU prefix: `src/gcPCBipedController_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcPCBipedController.h`

Not found.

## Matched Same-Class Neighbors

- `0x00152754` 76B `gcPCBipedController::Write(cFile &) const` — `src/gcPCBipedController.cpp`
- `0x001527a0` 188B `gcPCBipedController::Read(cFile &, cMemPool *)` — `src/gcStateHandler.cpp`
- `0x0015285c` 84B `gcPCBipedController::gcPCBipedController(cBase *)` — `src/gcPCBipedController.cpp`
- `0x00152f88` 92B `gcPCBipedController::OnSnappedTo(const mOCS &, bool)` — `src/gcPCBipedController.cpp`
- `0x0031a4c8` 124B `gcPCBipedController::New(cMemPool *, cBase *) static` — `src/gcPCBipedController.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcStateMachine::AssignCopy(const cBase *)`

- Address: `0x00259834`
- Size: `436` bytes
- Source: `src/gcStateMachine.cpp`

```cpp
// ── gcStateMachine::AssignCopy(const cBase *) @ 0x00259834 ──
void gcStateMachine::AssignCopy(const cBase *base) {
    const gcStateMachine *other = 0;

    if (base != 0) {
        if (D_0009A3E8 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_0009A3E8 = cType::InitializeType(0, 0, 0xB6, D_000385E4,
                                               &gcStateMachine::New,
                                               (const char *)0x36D954,
                                               (const char *)0x36D964, 5);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A3E8;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcStateMachine *)base;
        }
    }
    operator=(*other);
}
```

### Exemplar 2: `gcEntity::AssignCopy(const cBase *)`

- Address: `0x0025d774`
- Size: `436` bytes
- Source: `src/gcEntity.cpp`

```cpp
void gcEntity::AssignCopy(const cBase *base) {
    const gcEntity *other = 0;

    if (base != 0) {
        if (D_0009A408 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_0009A408 = cType::InitializeType(0, 0, 0x8C, D_000385E4,
                                               &gcEntity::New,
                                               (const char *)0x36D968,
                                               (const char *)0x36D974, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A408;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcEntity *)base;
        }
    }
    operator=(*other);
}
```

### Exemplar 3: `gcStaticInstance::AssignCopy(const cBase *)`

- Address: `0x00287ce0`
- Size: `436` bytes
- Source: `src/gcStaticInstance.cpp`

```cpp
void gcStaticInstance::AssignCopy(const cBase *base) {
    const gcStaticInstance *other = 0;

    if (base != 0) {
        if (D_0009F560 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_0009F560 = cType::InitializeType(0, 0, 0x67, D_000385E4,
                                               &gcStaticInstance::New,
                                               (const char *)0x36DA34,
                                               (const char *)0x36DA48, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F560;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcStaticInstance *)base;
        }
    }
    operator=(*other);
}
```

### Exemplar 4: `gcProfileString::AssignCopy(const cBase *)`

- Address: `0x00286600`
- Size: `420` bytes
- Source: `src/gcProfileString.cpp`

```cpp
void gcProfileString::AssignCopy(const cBase *base) {
    const gcProfileString *other = 0;

    if (base != 0) {
        if (D_0009F558 == 0) {
            if (D_0009F458 == 0) {
                if (D_0009F454 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                       0, 0, 0, 0);
                }
                D_0009F458 = cType::InitializeType(0, 0, 0x171, D_0009F454,
                                                   0, 0, 0, 0);
            }
            D_0009F558 = cType::InitializeType(0, 0, 0x1EE, D_0009F458,
                                               &gcProfileString::New,
                                               0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F558;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcProfileString *)base;
        }
    }
    operator=(*other);
}
```


## Prior Failure Notes

### Note 1

- session `9d0e3fd2`

Improved AssignCopy from the prior 173/456-byte diff to a 15/456-byte diff. The prefix copy, post-loop scalar/halfword copies, VFPU copies at 0xA0 and 0xC0, and loop size/branch offset now line up; the remaining mismatch is inside the 0x28 record-copy loop where SNC still chooses t0 for loaded word values and t1 for destination pointers, while the original uses t1 for the values and t0 for destination pointers after the first word. I tried byte-pointer staging, struct-field access, reordered direct stores, fixed-register local variables, memory barriers, and typed word20 accesses. The useful breakthrough was a +r empty asm barrier on the 0x08 source/destination pointers plus typed final word access, which removed the extra t3/source-0x20 address materialization.

The permuter could not be used on this multi-function source because tools/permuter.py failed to locate the target function in the compiled .o. A future retry should focus only on the t0/t1 allocation swap for the five stores at offsets 0x0C..0x1C; all other bytes are effectively in place. The current source intentionally keeps the closest 15-byte-diff loop form.

### Note 2

- session `a4d9eabb`

Implemented AssignCopy in real C++ and iterated through several source shapes: a straightforward field-copy version, a typed-layout version, a hybrid layout/pointer version, and multiple manually staged loop variants. The best result keeps the prefix copy and the tail around 0x70/0x88/0xA0/0xB0/0xC0 close to the target, but compare_func still reports 173/456 bytes different. Current whole-file status: GetType/New/Write/dtor all match; only AssignCopy is off.

The stubborn mismatch is the 0x28..0x6F loop copying two 0x24-byte records. The original uses a very specific pointer dance with alternating t0/t1/t2 destination/source temporaries; SNC keeps choosing a different register allocation even when the source is manually staged with intermediate pointers and reordered declarations. A secondary drift is that small source-shape changes can make the post-loop 0x78/0x8C/0xB0 address setup collapse back to more compact loads/stores, so the current version preserves that tail while trying to expand the middle loop. Next retry should focus narrowly on the loop: compare against the expected disassembly from expected/build/func/0031a300.o and experiment with struct-field vs byte-pointer source for only that block, rather than changing the already-close tail again.


## Disassembly

```asm
31a300:	27bdfff0 	addiu	sp,sp,-16
31a304:	afb00000 	sw	s0,0(sp)
31a308:	00808025 	move	s0,a0
31a30c:	afbf0004 	sw	ra,4(sp)
31a310:	0c08acc4 	jal	0x22b310
31a314:	00a02025 	move	a0,a1
31a318:	00402025 	move	a0,v0
31a31c:	24850008 	addiu	a1,a0,8
31a320:	8ca60000 	lw	a2,0(a1)
31a324:	8ca80004 	lw	t0,4(a1)
31a328:	26070008 	addiu	a3,s0,8
31a32c:	8ca90008 	lw	t1,8(a1)
31a330:	ace60000 	sw	a2,0(a3)
31a334:	8ca6000c 	lw	a2,12(a1)
31a338:	ace80004 	sw	t0,4(a3)
31a33c:	8ca80010 	lw	t0,16(a1)
31a340:	ace90008 	sw	t1,8(a3)
31a344:	8ca50014 	lw	a1,20(a1)
31a348:	ace6000c 	sw	a2,12(a3)
31a34c:	ace80010 	sw	t0,16(a3)
31a350:	ace50014 	sw	a1,20(a3)
31a354:	8c850020 	lw	a1,32(a0)
31a358:	ae050020 	sw	a1,32(s0)
31a35c:	90850024 	lbu	a1,36(a0)
31a360:	a2050024 	sb	a1,36(s0)
31a364:	90850025 	lbu	a1,37(a0)
31a368:	a2050025 	sb	a1,37(s0)
31a36c:	34070000 	li	a3,0x0
31a370:	26060028 	addiu	a2,s0,40
31a374:	24850028 	addiu	a1,a0,40
31a378:	8ca80000 	lw	t0,0(a1)
31a37c:	24a90008 	addiu	t1,a1,8
31a380:	acc80000 	sw	t0,0(a2)
31a384:	90a80004 	lbu	t0,4(a1)
31a388:	24ca0008 	addiu	t2,a2,8
31a38c:	a0c80004 	sb	t0,4(a2)
31a390:	90a80005 	lbu	t0,5(a1)
31a394:	a0c80005 	sb	t0,5(a2)
31a398:	8d280000 	lw	t0,0(t1)
31a39c:	24a9000c 	addiu	t1,a1,12
31a3a0:	ad480000 	sw	t0,0(t2)
31a3a4:	8d290000 	lw	t1,0(t1)
31a3a8:	24c8000c 	addiu	t0,a2,12
31a3ac:	24aa0010 	addiu	t2,a1,16
31a3b0:	ad090000 	sw	t1,0(t0)
31a3b4:	8d490000 	lw	t1,0(t2)
31a3b8:	24c80010 	addiu	t0,a2,16
31a3bc:	24aa0014 	addiu	t2,a1,20
31a3c0:	ad090000 	sw	t1,0(t0)
31a3c4:	8d490000 	lw	t1,0(t2)
31a3c8:	24c80014 	addiu	t0,a2,20
31a3cc:	24aa0018 	addiu	t2,a1,24
31a3d0:	ad090000 	sw	t1,0(t0)
31a3d4:	8d490000 	lw	t1,0(t2)
31a3d8:	24c80018 	addiu	t0,a2,24
31a3dc:	24aa001c 	addiu	t2,a1,28
31a3e0:	ad090000 	sw	t1,0(t0)
31a3e4:	8d490000 	lw	t1,0(t2)
31a3e8:	24c8001c 	addiu	t0,a2,28
31a3ec:	ad090000 	sw	t1,0(t0)
31a3f0:	8ca80020 	lw	t0,32(a1)
31a3f4:	24e70001 	addiu	a3,a3,1
31a3f8:	acc80020 	sw	t0,32(a2)
31a3fc:	24a50024 	addiu	a1,a1,36
31a400:	2ce80002 	sltiu	t0,a3,2
31a404:	1500ffdc 	bnez	t0,0x31a378
31a408:	24c60024 	addiu	a2,a2,36
31a40c:	8c850070 	lw	a1,112(a0)
31a410:	24860078 	addiu	a2,a0,120
31a414:	ae050070 	sw	a1,112(s0)
31a418:	8c850074 	lw	a1,116(a0)
31a41c:	26080078 	addiu	t0,s0,120
31a420:	ae050074 	sw	a1,116(s0)
31a424:	8cc50000 	lw	a1,0(a2)
31a428:	34070000 	li	a3,0x0
31a42c:	ad050000 	sw	a1,0(t0)
31a430:	8c85007c 	lw	a1,124(a0)
31a434:	26060080 	addiu	a2,s0,128
31a438:	ae05007c 	sw	a1,124(s0)
31a43c:	24850080 	addiu	a1,a0,128
31a440:	84a80000 	lh	t0,0(a1)
31a444:	84a90002 	lh	t1,2(a1)
31a448:	84aa0004 	lh	t2,4(a1)
31a44c:	a4c80000 	sh	t0,0(a2)
31a450:	a4c90002 	sh	t1,2(a2)
31a454:	a4ca0004 	sh	t2,4(a2)
31a458:	24e70001 	addiu	a3,a3,1
31a45c:	24c60006 	addiu	a2,a2,6
31a460:	18e0fff7 	blez	a3,0x31a440
31a464:	24a50006 	addiu	a1,a1,6
31a468:	c48c0088 	lwc1	$f12,136(a0)
31a46c:	2485008c 	addiu	a1,a0,140
31a470:	e60c0088 	swc1	$f12,136(s0)
31a474:	8ca50000 	lw	a1,0(a1)
31a478:	2606008c 	addiu	a2,s0,140
31a47c:	acc50000 	sw	a1,0(a2)
31a480:	94850090 	lhu	a1,144(a0)
31a484:	248600b0 	addiu	a2,a0,176
31a488:	a6050090 	sh	a1,144(s0)
31a48c:	d88600a0 	lv.q        C120, 0xA0($a0)
31a490:	fa0600a0 	sv.q        C120, 0xA0($s0)
31a494:	260500b0 	addiu	a1,s0,176
31a498:	8cc60000 	lw	a2,0(a2)
31a49c:	aca60000 	sw	a2,0(a1)
31a4a0:	c48c00b4 	lwc1	$f12,180(a0)
31a4a4:	e60c00b4 	swc1	$f12,180(s0)
31a4a8:	c48c00b8 	lwc1	$f12,184(a0)
31a4ac:	e60c00b8 	swc1	$f12,184(s0)
31a4b0:	d88600c0 	lv.q        C120, 0xC0($a0)
31a4b4:	fa0600c0 	sv.q        C120, 0xC0($s0)
31a4b8:	8fb00000 	lw	s0,0(sp)
31a4bc:	8fbf0004 	lw	ra,4(sp)
31a4c0:	03e00008 	jr	ra
31a4c4:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcPCBipedController::AssignCopy(const cBase *)
// Address: 0x0031a300, Size: 456B
// Obj: gcAll_psp.obj

void *dcastdcast_gcPCBipedControllerptr__constcBaseptr(?); /* extern */

void gcPCBipedController__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 var_a3_2;
    u32 var_a3;
    void *temp_a1;
    void *temp_a3;
    void *temp_v0;
    void *var_a1;
    void *var_a1_2;
    void *var_a2;
    void *var_a2_2;

    temp_v0 = dcastdcast_gcPCBipedControllerptr__constcBaseptr(arg1);
    temp_a1 = temp_v0 + 8;
    temp_a3 = arg0 + 8;
    arg0->unk8 = (s32) temp_v0->unk8;
    temp_a3->unk4 = (s32) temp_a1->unk4;
    temp_a3->unk8 = (s32) temp_a1->unk8;
    temp_a3->unkC = (s32) temp_a1->unkC;
    temp_a3->unk10 = (s32) temp_a1->unk10;
    temp_a3->unk14 = (s32) temp_a1->unk14;
    arg0->unk20 = (s32) temp_v0->unk20;
    arg0->unk24 = (u8) temp_v0->unk24;
    arg0->unk25 = (u8) temp_v0->unk25;
    var_a3 = 0;
    var_a2 = arg0 + 0x28;
    var_a1 = temp_v0 + 0x28;
    do {
        var_a2->unk0 = (s32) var_a1->unk0;
        var_a2->unk4 = (u8) var_a1->unk4;
        var_a2->unk5 = (u8) var_a1->unk5;
        var_a2->unk8 = (s32) var_a1->unk8;
        var_a2->unkC = (s32) var_a1->unkC;
        var_a2->unk10 = (s32) var_a1->unk10;
        var_a2->unk14 = (s32) var_a1->unk14;
        var_a2->unk18 = (s32) var_a1->unk18;
        var_a2->unk1C = (s32) var_a1->unk1C;
        var_a3 += 1;
        var_a2->unk20 = (s32) var_a1->unk20;
        var_a1 += 0x24;
        var_a2 += 0x24;
    } while (var_a3 < 2U);
    arg0->unk70 = (s32) temp_v0->unk70;
    arg0->unk74 = (s32) temp_v0->unk74;
    var_a3_2 = 0;
    arg0->unk78 = (s32) temp_v0->unk78;
    var_a2_2 = arg0 + 0x80;
    arg0->unk7C = (s32) temp_v0->unk7C;
    var_a1_2 = temp_v0 + 0x80;
    do {
        var_a2_2->unk0 = (s16) var_a1_2->unk0;
        var_a2_2->unk2 = (s16) var_a1_2->unk2;
        var_a2_2->unk4 = (s16) var_a1_2->unk4;
        var_a3_2 += 1;
        var_a2_2 += 6;
        var_a1_2 += 6;
    } while (var_a3_2 <= 0);
    arg0->unk88 = (f32) temp_v0->unk88;
    arg0->unk8C = (s32) temp_v0->unk8C;
    arg0->unk90 = (u16) temp_v0->unk90;
    __asm__ volatile("lv.q C120, 0xa0($a0)");
    __asm__ volatile("sv.q C120, 0xa0($s0)");
    arg0->unkB0 = (s32) temp_v0->unkB0;
    arg0->unkB4 = (f32) temp_v0->unkB4;
    arg0->unkB8 = (f32) temp_v0->unkB8;
    __asm__ volatile("lv.q C120, 0xc0($a0)");
    __asm__ volatile("sv.q C120, 0xc0($s0)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
