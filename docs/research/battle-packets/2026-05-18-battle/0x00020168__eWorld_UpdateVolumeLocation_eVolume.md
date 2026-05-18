# Battle Packet: `eWorld::UpdateVolumeLocation(eVolume *)`

## Target

- Address: `0x00020168`
- Size: `496` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eWorld`
- Method family: `UpdateVolumeLocation`
- Leaf: `False`

## Queue Metadata

- score: `162`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 17B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `17`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `17`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0001fb84` `matched` 352B `eWorld::GetRoomFromPos(const eRoom *, const mVec3 &) const`
- `0x0003e094` `untried` 340B `eRoom::Contains(const mVec3 &) const`
- `0x0003f3ac` `untried` 200B `eRoom::AddVolume(eVolume *, const mSphere &)`
- `0x0003f474` `matched` 40B `eRoom::RemoveVolume(eVolume *)`

Callers:
- `0x00020564` `untried` 264B `eWorld::MoveIntoRooms(void)`
- `0x0005d928` `matched` 44B `eVolume::UpdateLocalToWorld(void)`

## Placement

- Canonical source: `src/eWorld.cpp`
- Header(s): `include/eWorld.h`
- Allowed source(s): `src/eWorld.cpp`
- Split-TU prefix: `src/eWorld_*.cpp`

## Class Header: `include/eWorld.h`

```cpp
#ifndef EWORLD_H
#define EWORLD_H

struct mVec3;

class eSound {
public:
    char pad_sound[36];
    eSound *nextSound;  // 0x24
    eSound *prevSound;  // 0x28

    void Stop(void);
};

class eRoom {
public:
    char pad_room[264];
    eRoom *prevRoom;    // 0x108
    eRoom *nextRoom;    // 0x10C

    int Contains(const mVec3 &) const;
    int IsPointInFluidVolume(const mVec3 &) const;
};

class eRoomSet {
public:
    char pad_roomset[80];
    eRoomSet *prevRoomSet;  // 0x50
    eRoomSet *nextRoomSet;  // 0x54
};

class eWorld {
public:
    int field_0x00;
    int field_0x04;
    int field_0x08;
    int field_0x0C;
    eRoom *roomList;       // 0x10
    eRoomSet *roomSetList; // 0x14
    eSound *soundList;     // 0x18
    int field_0x1C;
    int field_0x20;
    mutable int lockCount; // 0x24

    eWorld();
    ~eWorld();
    void LockWorld(bool lock) const;
    void AddSound(eSound *s);
    void AddRoom(eRoom *r);
    void AddRoomSet(eRoomSet *rs);
    void RemoveSound(eSound *s);
    void RemoveRoom(eRoom *r);
    void RemoveRoomSet(eRoomSet *rs);
    const eRoom *GetRoomFromPos(const eRoom *, const mVec3 &) const;
    int IsPointInFluidVolume(const eRoom *, const mVec3 &) const;
    static int GetNextCullId(void);
    static void operator delete(void *);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0001f544` 64B `eWorld::eWorld(void)` — `src/eWorld.cpp`
- `0x0001f584` 116B `eWorld::~eWorld(void)` — `src/eWorld.cpp`
- `0x0001fb84` 352B `eWorld::GetRoomFromPos(const eRoom *, const mVec3 &) const` — `src/eWorld.cpp`
- `0x0001fce4` 76B `eWorld::IsPointInFluidVolume(const eRoom *, const mVec3 &) const` — `src/eWorld.cpp`
- `0x0001fd30` 20B `eWorld::GetNextCullId(void) static` — `src/eWorld.cpp`

## Prior Failure Notes

### Note 1

- session `4754e2dd`; src `src/eWorld.cpp`; snapshot `logs/failure_snapshots/match_20260430_201648/4754e2dd/00020168__agent_self_reported_failure__src_eWorld.cpp`

Implemented a real C++ eWorld::UpdateVolumeLocation(eVolume *) in src/eWorld.cpp, with eVolume offsets reconstructed from eVolume.cpp, native mSphere/VFPU copy for the world position, handle-table decode through D_00038890, room revalidation via eRoom::Contains, RemoveVolume/AddVolume transitions, and pending circular-list remove/insert logic. Read the VFPU, branch-likely, and delay-slot SNC research notes. Current best compiles to the correct 496-byte size and compare_func reports MISMATCH: 17/496 bytes differ for __0fGeWorldUUpdateVolumeLocationP6HeVolume. Existing unrelated AddRoomSet/RemoveRoomSet mismatches in this TU remain unchanged.

Manual variants tried included different sphere/radius assignment order, volatile vs non-volatile HandleValue stack copies, a cHandle::Write-style handle decode with unsigned-short index, void* vs eRoom* decoded result, branch-local raw handle copies, removing the cached pending-list prev pointer to recover the target bnel pattern, AddSound-style list insertion, sched=1 around the function, register binding/zero-instruction asm constraints for the decoded room, and barriers around the sphere stores. The best remaining differences are narrow scheduler/register-allocation issues: prologue ordering around sv.q/swc1/save/lw room, and the handle-table decode still keeps the resolved room/candidate in a1/a2 opposite the target a2/a1 flow. Two permuter runs were performed: the first 300s run improved from 25 to 18 bytes in its scoring and saved the current best; the second 300s run from that best found no further improvement.

### Note 2

- session `e6ade638`

Implemented a real C++ `eWorld::UpdateVolumeLocation` using the observed `eVolume` layout from `src/eVolume.cpp`: world pointer at `0x20`, current room at `0x24`, world-space center at `0x60`, flags at `0x74`, and pending-list links at `0x78/0x7C`. The current version builds the `mSphere` with a VFPU quad copy plus scalar radius overwrite, revalidates the current room via `Contains` and `D_00038890`, removes/re-adds the volume across rooms, and preserves the circular pending-list logic when `AddVolume` fails.

Best result was 492B vs target 496B. The closest source shape used a function-level `HandleValue` plus a branch-local `HandleValue raw`, a stack-resident `mSphere *spherePtr = &sphere`, and empty `memory` barriers around the first handle copy; that produced the correct 64-byte frame and most of the target control flow. The remaining 4-byte gap is in the first room-handle decode path: the original spills through two stack slots and keeps a slightly different result register flow, while several attempts to force that exact spill pattern (`idx` copies, volatile handle structs, extra aliases, and two-stage result pointers) either had no effect or blew the function much farther away.


## Disassembly

```asm
20168:	27bdffc0 	addiu	sp,sp,-64
2016c:	c4ac000c 	lwc1	$f12,12(a1)
20170:	d8a60060 	lv.q        C120, 0x60($a1)
20174:	fba60010 	sv.q        C120, 0x10($sp)
20178:	afb30030 	sw	s3,48(sp)
2017c:	e7ac001c 	swc1	$f12,28(sp)
20180:	8cb30024 	lw	s3,36(a1)
20184:	afb00024 	sw	s0,36(sp)
20188:	afb2002c 	sw	s2,44(sp)
2018c:	afb40034 	sw	s4,52(sp)
20190:	27b20010 	addiu	s2,sp,16
20194:	34140000 	li	s4,0x0
20198:	00808025 	move	s0,a0
2019c:	afb10028 	sw	s1,40(sp)
201a0:	afbf0038 	sw	ra,56(sp)
201a4:	12600023 	beqz	s3,0x20234
201a8:	00a08825 	move	s1,a1
201ac:	02602025 	move	a0,s3
201b0:	0c00f825 	jal	0x3e094
201b4:	02402825 	move	a1,s2
201b8:	afa20020 	sw	v0,32(sp)
201bc:	8fa40020 	lw	a0,32(sp)
201c0:	afa40000 	sw	a0,0(sp)
201c4:	8fa40000 	lw	a0,0(sp)
201c8:	14800003 	bnez	a0,0x201d8
201cc:	3085ffff 	andi	a1,a0,0xffff
201d0:	1000000c 	b	0x20204
201d4:	34060000 	li	a2,0x0
201d8:	3c060004 	lui	a2,0x4
201dc:	00052880 	sll	a1,a1,0x2
201e0:	24c68890 	addiu	a2,a2,-30576
201e4:	00a62821 	addu	a1,a1,a2
201e8:	8ca50000 	lw	a1,0(a1)
201ec:	10a00005 	beqz	a1,0x20204
201f0:	34060000 	li	a2,0x0
201f4:	8ca70030 	lw	a3,48(a1)
201f8:	14e40002 	bne	a3,a0,0x20204
201fc:	00000000 	nop
20200:	00a03025 	move	a2,a1
20204:	14d3000b 	bne	a2,s3,0x20234
20208:	02602025 	move	a0,s3
2020c:	02202825 	move	a1,s1
20210:	0c00fceb 	jal	0x3f3ac
20214:	02403025 	move	a2,s2
20218:	0040a025 	move	s4,v0
2021c:	16800046 	bnez	s4,0x20338
20220:	02602025 	move	a0,s3
20224:	0c00fd1d 	jal	0x3f474
20228:	02202825 	move	a1,s1
2022c:	10000028 	b	0x202d0
20230:	00000000 	nop
20234:	16600019 	bnez	s3,0x2029c
20238:	02602025 	move	a0,s3
2023c:	1220001a 	beqz	s1,0x202a8
20240:	02002025 	move	a0,s0
20244:	8e240078 	lw	a0,120(s1)
20248:	10800017 	beqz	a0,0x202a8
2024c:	02002025 	move	a0,s0
20250:	8e24007c 	lw	a0,124(s1)
20254:	50800014 	beqzl	a0,0x202a8
20258:	02002025 	move	a0,s0
2025c:	8e05001c 	lw	a1,28(s0)
20260:	54b10004 	bnel	a1,s1,0x20274
20264:	8e250078 	lw	a1,120(s1)
20268:	ae04001c 	sw	a0,28(s0)
2026c:	8e24007c 	lw	a0,124(s1)
20270:	8e250078 	lw	a1,120(s1)
20274:	aca4007c 	sw	a0,124(a1)
20278:	8e250078 	lw	a1,120(s1)
2027c:	ac850078 	sw	a1,120(a0)
20280:	ae200078 	sw	zero,120(s1)
20284:	ae20007c 	sw	zero,124(s1)
20288:	8e04001c 	lw	a0,28(s0)
2028c:	14910006 	bne	a0,s1,0x202a8
20290:	02002025 	move	a0,s0
20294:	10000003 	b	0x202a4
20298:	ae00001c 	sw	zero,28(s0)
2029c:	0c00fd1d 	jal	0x3f474
202a0:	02202825 	move	a1,s1
202a4:	02002025 	move	a0,s0
202a8:	02602825 	move	a1,s3
202ac:	0c007ee1 	jal	0x1fb84
202b0:	02403025 	move	a2,s2
202b4:	00409825 	move	s3,v0
202b8:	12600005 	beqz	s3,0x202d0
202bc:	02602025 	move	a0,s3
202c0:	02202825 	move	a1,s1
202c4:	0c00fceb 	jal	0x3f3ac
202c8:	02403025 	move	a2,s2
202cc:	0040a025 	move	s4,v0
202d0:	16800019 	bnez	s4,0x20338
202d4:	00000000 	nop
202d8:	92240074 	lbu	a0,116(s1)
202dc:	30840001 	andi	a0,a0,0x1
202e0:	10800015 	beqz	a0,0x20338
202e4:	00000000 	nop
202e8:	c7ac001c 	lwc1	$f12,28(sp)
202ec:	44806800 	mtc1	zero,$f13
202f0:	460d603c 	c.lt.s	$f12,$f13
202f4:	00000000 	nop
202f8:	4501000f 	bc1t	0x20338
202fc:	00000000 	nop
20300:	8e04001c 	lw	a0,28(s0)
20304:	5080000a 	beqzl	a0,0x20330
20308:	ae11001c 	sw	s1,28(s0)
2030c:	8c840078 	lw	a0,120(a0)
20310:	ae240078 	sw	a0,120(s1)
20314:	8e04001c 	lw	a0,28(s0)
20318:	8e250078 	lw	a1,120(s1)
2031c:	ae24007c 	sw	a0,124(s1)
20320:	acb1007c 	sw	s1,124(a1)
20324:	8e24007c 	lw	a0,124(s1)
20328:	10000003 	b	0x20338
2032c:	ac910078 	sw	s1,120(a0)
20330:	ae31007c 	sw	s1,124(s1)
20334:	ae310078 	sw	s1,120(s1)
20338:	8fb00024 	lw	s0,36(sp)
2033c:	8fb10028 	lw	s1,40(sp)
20340:	8fb2002c 	lw	s2,44(sp)
20344:	8fb30030 	lw	s3,48(sp)
20348:	8fb40034 	lw	s4,52(sp)
2034c:	8fbf0038 	lw	ra,56(sp)
20350:	03e00008 	jr	ra
20354:	27bd0040 	addiu	sp,sp,64
```

## m2c Starting Point

```c
// Decompiled: eWorld::UpdateVolumeLocation(eVolume *)
// Address: 0x00020168, Size: 496B
// Obj: eAll_psp.obj

s32 eRoom__AddVolume_eVolumeptr_constmSphereref(s32, void *, ? *); /* extern */
s32 eRoom__Contains_constmVec3refconst(f32, s32, ? *); /* extern */
? eRoom__RemoveVolume_eVolumeptr(s32, void *, void *); /* extern */
s32 eWorld__GetRoomFromPos_consteRoomptr_constmVec3refconst(void *, s32, ? *); /* extern */

void eWorld__UpdateVolumeLocation_eVolumeptr(void *arg0, void *arg1, void *arg2) {
    ? sp10;
    f32 sp1C;
    s32 sp20;
    f32 temp_f12;
    s32 temp_s3;
    s32 temp_v0;
    s32 var_s4;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a1;
    void *var_a2;

    var_a2 = arg2;
    temp_f12 = arg1->unkC;
    __asm__ volatile("lv.q C120, 0x60($a1)");
    __asm__ volatile("sv.q C120, 0x10($sp)");
    sp1C = temp_f12;
    temp_s3 = arg1->unk24;
    var_s4 = 0;
    if (temp_s3 != 0) {
        sp20 = eRoom__Contains_constmVec3refconst(temp_f12, temp_s3, &sp10);
        if (subroutine_arg0 == 0) {
            var_a2 = NULL;
        } else {
            temp_a1 = ((subroutine_arg0 & 0xFFFF) * 4)->unk38890;
            var_a2 = NULL;
            if ((temp_a1 != NULL) && (temp_a1->unk30 == subroutine_arg0)) {
                var_a2 = temp_a1;
            }
        }
        if (var_a2 == temp_s3) {
            var_s4 = eRoom__AddVolume_eVolumeptr_constmSphereref(temp_s3, arg1, &sp10);
            if (var_s4 == 0) {
                eRoom__RemoveVolume_eVolumeptr(temp_s3, arg1);
                goto block_21;
            }
        } else {
            goto block_9;
        }
    } else {
block_9:
        if (temp_s3 == 0) {
            if ((arg1 != NULL) && (arg1->unk78 != NULL)) {
                temp_a0 = arg1->unk7C;
                if (temp_a0 != NULL) {
                    if (arg0->unk1C == arg1) {
                        arg0->unk1C = temp_a0;
                    }
                    arg1->unk78->unk7C = arg1->unk7C;
                    arg1->unk7C->unk78 = (void *) arg1->unk78;
                    arg1->unk78 = NULL;
                    arg1->unk7C = NULL;
                    if (arg0->unk1C == arg1) {
                        arg0->unk1C = NULL;
                    }
                }
            }
        } else {
            eRoom__RemoveVolume_eVolumeptr(temp_s3, arg1, var_a2);
        }
        temp_v0 = eWorld__GetRoomFromPos_consteRoomptr_constmVec3refconst(arg0, temp_s3, &sp10);
        if (temp_v0 != 0) {
            var_s4 = eRoom__AddVolume_eVolumeptr_constmSphereref(temp_v0, arg1, &sp10);
        }
block_21:
        if ((var_s4 == 0) && (arg1->unk74 & 1) && !(sp1C < 0.0f)) {
            temp_a0_2 = arg0->unk1C;
            if (temp_a0_2 == NULL) {
                arg0->unk1C = arg1;
                arg1->unk7C = arg1;
                arg1->unk78 = arg1;
            } else {
                arg1->unk78 = (void *) temp_a0_2->unk78;
                arg1->unk7C = (void *) arg0->unk1C;
                arg1->unk78->unk7C = arg1;
                arg1->unk7C->unk78 = arg1;
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
