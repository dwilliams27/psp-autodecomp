# Battle Packet: `gcPartialBodyController::Reset(cMemPool *, bool)`

## Target

- Address: `0x00140474`
- Size: `288` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcPartialBodyController`
- Method family: `Reset`
- Leaf: `False`

## Queue Metadata

- score: `129`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 37B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `37`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `37`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0013e82c` `matched` 60B `gcPartialEntityController::Reset(cMemPool *, bool)`
- `0x001ad668` `matched` 52B `memset`

## Placement

- Canonical source: `src/gcPartialBodyController.cpp`
- Header(s): `include/gcPartialBodyController.h`
- Allowed source(s): `src/gcPartialBodyController.cpp`
- Split-TU prefix: `src/gcPartialBodyController_*.cpp`

## Class Header: `include/gcPartialBodyController.h`

Not found.

## Matched Same-Class Neighbors

- `0x0014024c` 76B `gcPartialBodyController::Write(cFile &) const` — `src/gcPartialBodyController.cpp`
- `0x00140298` 188B `gcPartialBodyController::Read(cFile &, cMemPool *)` — `src/gcPartialBodyController.cpp`
- `0x00140354` 104B `gcPartialBodyController::gcPartialBodyController(cBase *)` — `src/gcPartialBodyController.cpp`
- `0x001403bc` 184B `gcPartialBodyController::~gcPartialBodyController(void)` — `src/gcPartialBodyController.cpp`
- `0x00140594` 96B `gcPartialBodyController::OnMemPoolReset(const cMemPool *, unsigned int)` — `src/gcPartialBodyController.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcRoomInstance::Reset(cMemPool *, bool)`

- Address: `0x0013c6e8`
- Size: `176` bytes
- Source: `src/gcRoomInstance.cpp`

```cpp
void gcRoomInstance::Reset(cMemPool *, bool) {
    int value = *(int *)((char *)this + 0x44);
    int valid;
    if (value == 0) {
        valid = 0;
    } else {
        HandleEntry *entry = D_00038890[value & 0xFFFF];
        HandleEntry *found = 0;
        if (entry != 0) {
            if (entry->handle == value) {
                found = entry;
            }
        }
        valid = found != 0;
    }

    if ((valid & 0xFF) != 0) {
        HandleEntry *entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        int *nextRoom = (int *)((char *)this + 0x48);
        int next = *nextRoom;
        __asm__ volatile(
            "addiu $a3, %1, 0x00e8\n"
            "sw %0, 0($a3)"
            :
            : "r"(next), "r"(entry)
            : "a3", "memory");

        value = *(volatile int *)((char *)this + 0x44);
        entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        entry->roomInstance = (int)this;
    }
}
```

### Exemplar 2: `gcEntityCustomAnimation::Reset(cMemPool *, bool)`

- Address: `0x000e9c48`
- Size: `404` bytes
- Source: `src/gcEntityCustomAnimation.cpp`

```cpp
void gcEntityCustomAnimation::Reset(cMemPool *, bool) {
    if (((*(short *)((char *)this + 0x1C) == 0) & 0xFF) != 0) {
        int temp_a0 = *(int *)((char *)this + 0x44);
        int var_a2 = 0;
        if (temp_a0 & 1) {
            var_a2 = 1;
        }
        if (var_a2 == 0) {
            goto valid_from_value;
        }
        var_a2 = 0;
        goto valid_done;
    valid_from_value:
        var_a2 = temp_a0 != 0;
        var_a2 = (var_a2 & 0xFF) != 0;
    valid_done:
        if (var_a2 != 0) {
            var_a2 = 0;
            int temp_a1 = temp_a0 & 1;
            if (temp_a1 != 0) {
                var_a2 = 1;
            }
            char *base;
            if (var_a2 == 0) {
                goto slot_from_value;
            }
            base = 0;
            __asm__ volatile("" : "+r"(base));
            goto slot_done;
        slot_from_value:
            base = (char *)temp_a0;
        slot_done:
            int *slot = (int *)(base + 8);
            int handle = *slot;
            HandleEntry *found;
            if (handle == 0) {
                found = 0;
            } else {
                int var_a3 = (int)D_00038890[handle & 0xFFFF];
                int var_t0 = 0;
                if (var_a3 != 0) {
                    if (*(int *)(var_a3 + 0x30) == handle) {
                        var_t0 = var_a3;
                    }
                }
                found = (HandleEntry *)var_t0;
            }
            if (found != 0) {
                int var_a1 = 0;
                temp_a1 = temp_a0 & 1;
                if (temp_a1 != 0) {
                    var_a1 = 1;
                }
                char *base2;
                if (var_a1 == 0) {
                    goto slot2_from_value;
                }
                base2 = 0;
                __asm__ volatile("" : "+r"(base2));
                goto slot2_done;
            slot2_from_value:
                base2 = (char *)temp_a0;
            slot2_done:
                int *slot2 = (int *)(base2 + 8);
                int handle2 = *slot2;
                HandleEntry *entry2;
                if (handle2 == 0) {
                    entry2 = 0;
                } else {
                    HandleEntry *candidate = D_00038890[handle2 & 0xFFFF];
                    entry2 = 0;
                    if (candidate != 0) {
                        if (candidate->handle == handle2) {
                            entry2 = candidate;
                        }
                    }
                }

                const char *name;
                if (entry2 != 0) {
                    int temp_a0_2 =
                        ((*(short *)((char *)entry2 + 0x1C) == 0) & 0xFF);
                    entry2 = (HandleEntry *)((char *)entry2 + 8);
                    if (temp_a0_2 != 0) {
                        entry2 = (HandleEntry *)0x36DAB8;
                    } else {
                        __asm__ volatile("" ::: "memory");
                    }
                    name = (const char *)entry2;
                } else if (handle2 != 0) {
                    name = (const char *)0x36DAC4;
                } else {
                    name = (const char *)0x36DACC;
                }
                ((cName *)((char *)this + 8))->Set(name);

                ResetDispatchRec *rec =
                    (ResetDispatchRec *)(*(char **)((char *)this + 4) + 0x70);
                short offset = rec->offset;
                void (*fn)(void *) = rec->fn;
                fn((char *)this + offset);
            }
        }
    }
}
```

### Exemplar 3: `gcEntityCustomAttack::Reset(cMemPool *, bool)`

- Address: `0x001257a4`
- Size: `404` bytes
- Source: `src/gcEntityCustomAttack.cpp`

```cpp
void gcEntityCustomAttack::Reset(cMemPool *, bool) {
    if (((*(short *)((char *)this + 0x1C) == 0) & 0xFF) != 0) {
        int temp_a0 = *(int *)((char *)this + 0x44);
        int var_a2 = 0;
        if (temp_a0 & 1) {
            var_a2 = 1;
        }
        if (var_a2 == 0) {
            goto valid_from_value;
        }
        var_a2 = 0;
        goto valid_done;
    valid_from_value:
        var_a2 = temp_a0 != 0;
        var_a2 = (var_a2 & 0xFF) != 0;
    valid_done:
        if (var_a2 != 0) {
            var_a2 = 0;
            int temp_a1 = temp_a0 & 1;
            if (temp_a1 != 0) {
                var_a2 = 1;
            }
            char *base;
            if (var_a2 == 0) {
                goto slot_from_value;
            }
            base = 0;
            __asm__ volatile("" : "+r"(base));
            goto slot_done;
        slot_from_value:
            base = (char *)temp_a0;
        slot_done:
            int *slot = (int *)(base + 0x10);
            int handle = *slot;
            HandleEntry *found;
            if (handle == 0) {
                found = 0;
            } else {
                int var_a3 = (int)D_00038890[handle & 0xFFFF];
                int var_t0 = 0;
                if (var_a3 != 0) {
                    if (*(int *)(var_a3 + 0x30) == handle) {
                        var_t0 = var_a3;
                    }
                }
                found = (HandleEntry *)var_t0;
            }
            if (found != 0) {
                int var_a1 = 0;
                temp_a1 = temp_a0 & 1;
                if (temp_a1 != 0) {
                    var_a1 = 1;
                }
                char *base2;
                if (var_a1 == 0) {
                    goto slot2_from_value;
                }
                base2 = 0;
                __asm__ volatile("" : "+r"(base2));
                goto slot2_done;
            slot2_from_value:
                base2 = (char *)temp_a0;
            slot2_done:
                int *slot2 = (int *)(base2 + 0x10);
                int handle2 = *slot2;
                HandleEntry *entry2;
                if (handle2 == 0) {
                    entry2 = 0;
                } else {
                    HandleEntry *candidate = D_00038890[handle2 & 0xFFFF];
                    entry2 = 0;
                    if (candidate != 0) {
                        if (candidate->handle == handle2) {
                            entry2 = candidate;
                        }
                    }
                }

                const char *name;
                if (entry2 != 0) {
                    int temp_a0_2 =
                        ((*(short *)((char *)entry2 + 0x1C) == 0) & 0xFF);
                    entry2 = (HandleEntry *)((char *)entry2 + 8);
                    if (temp_a0_2 != 0) {
                        entry2 = (HandleEntry *)0x36DAB8;
                    } else {
                        __asm__ volatile("" ::: "memory");
                    }
                    name = (const char *)entry2;
                } else if (handle2 != 0) {
                    name = (const char *)0x36DAC4;
                } else {
                    name = (const char *)0x36DACC;
                }
                ((cName *)((char *)this + 8))->Set(name);

                ResetDispatchRec *rec =
                    (ResetDispatchRec *)(*(char **)((char *)this + 4) + 0x70);
                short offset = rec->offset;
                void (*fn)(void *) = rec->fn;
                fn((char *)this + offset);
            }
        }
    }
}
```

### Exemplar 4: `gcExternalCinematic::Reset(cMemPool *, bool)`

- Address: `0x000eaee0`
- Size: `72` bytes
- Source: `src/gcExternalCinematic.cpp`

```cpp
// gcExternalCinematic::Reset(cMemPool *, bool) @ 0x000eaee0
// gcExternalCinematic::Write(cFile &) const @ 0x000eace0
// gcExternalCinematic::New(cMemPool *, cBase *) static @ 0x00243958
// cObject base, gcCinematic embedded member at +0x44.

class cBase;
class cMemPool;
class cFile;
class cType;

template <class T> T *dcast(const cBase *);

struct copy_word { int v; };
```


## Prior Failure Notes

### Note 1

- session `53c55c53`; src `src/gcPartialBodyController.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/53c55c53/00140474__agent_self_reported_failure__src_gcPartialBodyController.cpp`

Closest source is a C++ Reset implementation in the canonical class file. It preserves all previously matched gcPartialBodyController methods and compiles to the correct 288-byte symbol, but compare_func reports 37/288 bytes different. The remaining differences are concentrated in register allocation/scheduling: current code keeps the body pointer in s2 and count in a1 where the original uses a0/a2, schedules the raw bool mask after callee-save stores instead of before, and emits addiu+lw for the allocator table where the original folds lw 28(a0). Allocation and delete control flow are semantically aligned, including the two memset calls and the beqzl null-allocation store shape after removing the temporary result barrier.

Tried direct bool call, raw extern-C call to the base Reset to avoid bool canonicalization, register hints for flag/count/body/result, split allocTable temporaries, direct field reloads, and allocator/delete restructurings. Ran `python3 tools/permuter.py src/gcPartialBodyController.cpp 0x00140474 --time 300 --save-best`; it generated 6192 candidates, compiled 1947, and found zero improvement from the 37-byte baseline. Not classifying as unmatchable_bnel because the diff is larger than 8 bytes and is not isolated to bnel/beql.


## Disassembly

```asm
140474:	27bdfff0 	addiu	sp,sp,-16
140478:	afb10004 	sw	s1,4(sp)
14047c:	00808825 	move	s1,a0
140480:	30c400ff 	andi	a0,a2,0xff
140484:	afb00000 	sw	s0,0(sp)
140488:	afb20008 	sw	s2,8(sp)
14048c:	afbf000c 	sw	ra,12(sp)
140490:	10800005 	beqz	a0,0x1404a8
140494:	00a08025 	move	s0,a1
140498:	00803025 	move	a2,a0
14049c:	02202025 	move	a0,s1
1404a0:	0c04fa0b 	jal	0x13e82c
1404a4:	02002825 	move	a1,s0
1404a8:	8e240008 	lw	a0,8(s1)
1404ac:	34060000 	li	a2,0x0
1404b0:	24850020 	addiu	a1,a0,32
1404b4:	8ca50000 	lw	a1,0(a1)
1404b8:	8e240040 	lw	a0,64(s1)
1404bc:	54a00001 	bnezl	a1,0x1404c4
1404c0:	8ca6fffc 	lw	a2,-4(a1)
1404c4:	14c0000f 	bnez	a2,0x140504
1404c8:	00000000 	nop
1404cc:	1080002b 	beqz	a0,0x14057c
1404d0:	00000000 	nop
1404d4:	0c0011e8 	jal	0x47a0
1404d8:	00808025 	move	s0,a0
1404dc:	8c440024 	lw	a0,36(v0)
1404e0:	02002825 	move	a1,s0
1404e4:	8c86001c 	lw	a2,28(a0)
1404e8:	24c60030 	addiu	a2,a2,48
1404ec:	84c70000 	lh	a3,0(a2)
1404f0:	8cc60004 	lw	a2,4(a2)
1404f4:	00c0f809 	jalr	a2
1404f8:	00872021 	addu	a0,a0,a3
1404fc:	1000001f 	b	0x14057c
140500:	ae200040 	sw	zero,64(s1)
140504:	1480001d 	bnez	a0,0x14057c
140508:	00000000 	nop
14050c:	8e040024 	lw	a0,36(s0)
140510:	34120000 	li	s2,0x0
140514:	8c86001c 	lw	a2,28(a0)
140518:	340500b0 	li	a1,0xb0
14051c:	24c60028 	addiu	a2,a2,40
140520:	84c70000 	lh	a3,0(a2)
140524:	8cc90004 	lw	t1,4(a2)
140528:	00872021 	addu	a0,a0,a3
14052c:	34060010 	li	a2,0x10
140530:	00003825 	move	a3,zero
140534:	0120f809 	jalr	t1
140538:	00004025 	move	t0,zero
14053c:	00408025 	move	s0,v0
140540:	5200000e 	beqzl	s0,0x14057c
140544:	ae320040 	sw	s2,64(s1)
140548:	ae000000 	sw	zero,0(s0)
14054c:	ae000004 	sw	zero,4(s0)
140550:	ae000010 	sw	zero,16(s0)
140554:	26040008 	addiu	a0,s0,8
140558:	00002825 	move	a1,zero
14055c:	0c06b59a 	jal	0x1ad668
140560:	34060008 	li	a2,0x8
140564:	26040014 	addiu	a0,s0,20
140568:	00002825 	move	a1,zero
14056c:	0c06b59a 	jal	0x1ad668
140570:	34060010 	li	a2,0x10
140574:	02009025 	move	s2,s0
140578:	ae320040 	sw	s2,64(s1)
14057c:	8fb00000 	lw	s0,0(sp)
140580:	8fb10004 	lw	s1,4(sp)
140584:	8fb20008 	lw	s2,8(sp)
140588:	8fbf000c 	lw	ra,12(sp)
14058c:	03e00008 	jr	ra
140590:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcPartialBodyController::Reset(cMemPool *, bool)
// Address: 0x00140474, Size: 288B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, void *, s32); /* extern */
? gcPartialEntityController__Reset_cMemPoolptr_bool(void *, void *, s32); /* extern */
? memset(void *, ?, ?);                             /* extern */

void gcPartialBodyController__Reset_cMemPoolptr_bool(void *arg0, void *arg1, s32 arg2) {
    ? (*temp_a2_2)(void *, void *, ?, s16);
    s16 temp_a3;
    s32 temp_a0;
    s32 var_a2;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a0_4;
    void *temp_a1;
    void *temp_a2;
    void *temp_a2_3;
    void *temp_v0;
    void *var_s2;

    temp_a0 = arg2 & 0xFF;
    if (temp_a0 != 0) {
        gcPartialEntityController__Reset_cMemPoolptr_bool(arg0, arg1, temp_a0);
    }
    var_a2 = 0;
    temp_a1 = arg0->unk8->unk20;
    temp_a0_2 = arg0->unk40;
    if (temp_a1 != NULL) {
        var_a2 = temp_a1->unk-4;
    }
    if (var_a2 == 0) {
        if (temp_a0_2 != NULL) {
            temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_a0_2, temp_a1, var_a2)->unk24;
            temp_a2 = temp_a0_3->unk1C;
            temp_a3 = temp_a2->unk30;
            temp_a2_2 = (temp_a2 + 0x30)->unk4;
            temp_a2_2(temp_a0_3 + temp_a3, temp_a0_2, temp_a2_2, temp_a3);
            arg0->unk40 = NULL;
        }
    } else if (temp_a0_2 == NULL) {
        temp_a0_4 = arg1->unk24;
        var_s2 = NULL;
        temp_a2_3 = temp_a0_4->unk1C;
        temp_v0 = (temp_a2_3 + 0x28)->unk4(temp_a0_4 + temp_a2_3->unk28, 0xB0, 0x10, 0);
        if (temp_v0 != NULL) {
            temp_v0->unk0 = 0;
            temp_v0->unk4 = 0;
            temp_v0->unk10 = 0;
            memset(temp_v0 + 8, 0, 8);
            memset(temp_v0 + 0x14, 0, 0x10);
            var_s2 = temp_v0;
        }
        arg0->unk40 = var_s2;
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
