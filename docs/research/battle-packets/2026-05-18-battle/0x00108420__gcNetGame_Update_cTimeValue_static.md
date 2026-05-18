# Battle Packet: `gcNetGame::Update(cTimeValue) static`

## Target

- Address: `0x00108420`
- Size: `608` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcNetGame`
- Method family: `Update`
- Leaf: `False`

## Queue Metadata

- score: `175`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 4B; codegen-specific diagnosis`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `4`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000d5348` `failed` 232B `gcExpressionList::Evaluate(void) const`
- `0x00106810` `failed` 172B `gcGame::GetMapIndex(const cGUIDT<gcMap> &) const`
- `0x00108f54` `untried` 248B `gcNetGame::GetNumActiveConnections(unsigned int) static`
- `0x001a544c` `matched` 176B `nwNetwork::Update(cTimeValue) static`
- `0x001a54fc` `matched` 8B `nwNetwork::GetLobby(void) static`
- `0x001a564c` `matched` 12B `nwNetwork::GetGameSocket(void) static`
- `0x001a8370` `matched` 120B `nwSocket::GetSocket(nwSocketHandle) static`
- `0x001ad668` `matched` 52B `memset`
- `0x001b9c64` `untried` 68B `__vec_new`

Callers:
- `0x000f364c` `untried` 688B `gcMap::Update(cTimeValue)`
- `0x0016f4bc` `untried` 2192B `gcGame::Loop(void)`
- `0x00222078` `untried` 388B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcBackgroundLoader> >(gcRenderLoopT *)`
- `0x00225cc8` `untried` 456B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<eMoviePauseExitCheck> >(gcRenderLoopT *)`
- `0x00225e90` `untried` 400B `void gcRenderLoopT<fileSystemErrorLoop>(gcRenderLoopT *)`
- `0x00226020` `untried` 412B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcUIOpenDialogExitCheck> >(gcRenderLoopT *)`
- `0x00226b00` `untried` 516B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcMapServerReadyExitCheck> >(gcRenderLoopT *)`
- `0x00226ee8` `untried` 524B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcRegionLoaderExitCheck> >(gcRenderLoopT *)`

## Placement

- Canonical source: `src/gcNetGame.cpp`
- Header(s): `include/gcNetGame.h`
- Allowed source(s): `src/gcNetGame.cpp`
- Split-TU prefix: `src/gcNetGame_*.cpp`

## Class Header: `include/gcNetGame.h`

Not found.

## Matched Same-Class Neighbors

- `0x001082e0` 12B `gcNetGame::SetMaxConnections(int) static` — `src/gcNetGame.cpp`
- `0x001082ec` 12B `gcNetGame::GetMaxConnections(void) static` — `src/gcNetGame.cpp`
- `0x001082f8` 8B `gcNetGame::StartServer(void) static` — `src/gcGame.cpp`
- `0x001087e8` 8B `gcNetGame::CheckSynchronization(void) static` — `src/gcGame.cpp`
- `0x00108a30` 260B `gcNetGame::SetAllSendMaskOnOff(unsigned int, unsigned int) static` — `src/gcNetGame.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcBipedController::Update(cTimeValue)`

- Address: `0x00141290`
- Size: `184` bytes
- Source: `src/gcBipedController.cpp`

```cpp
// ── gcBipedController::Update(cTimeValue) @ 0x00141290 ──
void gcBipedController::Update(cTimeValue dt) {
    gcEntityController::Update(dt);
    if (IsOnGround()) {
        int *timer = (int *)((char *)this + 0xB0);
        volatile int zero = 0;
        *timer = 0;
    } else {
        __asm__ volatile("" ::: "memory");
        int t = dt.mTime;
        int current = *(int *)((char *)this + 0xB0);
        volatile int spill0 = t;
        volatile int spill1 = t;
        *(int *)((char *)this + 0xB0) = current + t;
    }

    void *entity = *(void **)((char *)this + 0x20);
    if (!IsWallWalkEnabled() && *(unsigned char *)((char *)entity + 0x50) == 0) {
        float zero = 0.0f;
        char *base = ((char **)*(char **)this)[0x1F8 / 4];
        base += 0x20;
        base += 4;
        *(float *)base = zero;
        base = ((char **)*(char **)this)[0x1F8 / 4];
        base += 0x20;
        base += 8;
        *(float *)base = zero;
    }
}
```

### Exemplar 2: `gcFlyingController::Update(cTimeValue)`

- Address: `0x00153f14`
- Size: `28` bytes
- Source: `src/gcFlyingController.cpp`

```cpp
void gcFlyingController::Update(cTimeValue t) {
    gcEntityController::Update(t);
}
```

### Exemplar 3: `gcGameCore::Update(cTimeValue, bool) static`

- Address: `0x000ccfd8`
- Size: `8` bytes
- Source: `src/gcGameCore.cpp`

```cpp
void gcGameCore::Update(cTimeValue, bool) {
}
```

### Exemplar 4: `gcUIGeom::Update(cTimeValue)`

- Address: `0x0013b8c0`
- Size: `88` bytes
- Source: `src/gcUIGeom.cpp`

```cpp
// ── gcUIGeom::Update(cTimeValue) @ 0x0013b8c0 ──
void gcUIGeom::Update(cTimeValue t) {
    ((gcUI *)((char *)this + 0xF0))->Update(t);
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}
```


## Prior Failure Notes

### Note 1

- session `364fd7d1`; src `src/gcNetGame.cpp`; snapshot `logs/failure_snapshots/match_20260430_201648/364fd7d1/00108420__agent_self_reported_failure__src_gcNetGame.cpp`

Closest source is 4/608 bytes off after relocation masking. I started from the prior 12-byte baseline, tried splitting the layout padding from the frame object, an aligned layout, volatile handle locals, explicit argument lvalues at &u - 4, a raw ABI alias for nwSocket::GetSocket, a volatile integer argument slot, fixed $sp register addressing, and a one-word array argument slot. The raw/volatile forms can make SNC mention sp+4, but they either add extra stack locals, put the store in the jal delay slot, or perturb saved-register allocation and widen the diff. I also read docs/research/snc-delay-slots.md once the delay-slot behavior became the blocker.

The best retained source uses an aligned update layout and an early exprBase assignment so all bytes match except the two GetSocket by-value argument copy sites in the second half. Generated code copies the first handle through sw/lw at sp+0xf8 and the second through sp+0xfc; the target copies both through sp+0x4. A 600s permuter run improved the previous 12-byte source to 6 bytes, a manual ordering merge reduced it to 4 bytes, and a second focused 300s permuter run from the 4-byte baseline produced no improvement. This is not an unmatchable_bnel case: the remaining differing instructions are sw/lw stack-offset immediates, not branch-likely instructions.

### Note 2

- session `8b11703b`

Built gcNetGame::Update(cTimeValue) around the exact lobby-dispatch pattern from the disassembly, then iterated on stack layout and lifetime shaping. The best source uses a top-level layout struct so the scratch block, message block, and eval-record block land at the correct offsets; that got the function down from a 71-byte mismatch to 12 bytes. I also tried raw offset-addressed frame storage, moving the scratch/message/record blocks to function scope, folding the socket handles into the layout, pulling them back out as separate locals, and switching between direct nested GetGameSocket/GetSocket calls and named handle temporaries. Finally I ran `python3 tools/permuter.py src/gcNetGame.cpp "gcNetGame::Update(cTimeValue) static" --time 180 --save-best`; it compiled 3042 candidates over 180.6s and found no improvement over the 18-byte baseline at that stage.

The remaining mismatch is localized to the two `nwNetwork::GetGameSocket()` / `nwSocket::GetSocket(...)` call sites in the second half of the function. The current best object is 12/608 bytes off because SNC still routes the socket-handle argument through extra stack locals at `sp+0xF8`/`sp+0xFC` before the `GetSocket` call, while the target copies directly from the handle temp at `sp+0x100`/`sp+0x104` into the ABI outgoing slot at `sp+0x4`. Everything before that block, including the scratch initialization, expression-eval stack record, message buffer setup, active-connection count, and map-index call, matches the expected instruction shape. The next retry should focus only on those two handle temporaries and compare that block with `objdump` after each small source change.


## Disassembly

```asm
108420:	27bdfee0 	addiu	sp,sp,-288
108424:	afa40000 	sw	a0,0(sp)
108428:	e7b40108 	swc1	$f20,264(sp)
10842c:	afb0010c 	sw	s0,268(sp)
108430:	afb10110 	sw	s1,272(sp)
108434:	afb20114 	sw	s2,276(sp)
108438:	afbf0118 	sw	ra,280(sp)
10843c:	0c069513 	jal	0x1a544c
108440:	8fa40000 	lw	a0,0(sp)
108444:	0c06953f 	jal	0x1a54fc
108448:	00000000 	nop
10844c:	00408825 	move	s1,v0
108450:	1220004d 	beqz	s1,0x108588
108454:	3c040038 	lui	a0,0x38
108458:	8e240000 	lw	a0,0(s1)
10845c:	24840218 	addiu	a0,a0,536
108460:	84850000 	lh	a1,0(a0)
108464:	8c860004 	lw	a2,4(a0)
108468:	00c0f809 	jalr	a2
10846c:	02252021 	addu	a0,s1,a1
108470:	10400045 	beqz	v0,0x108588
108474:	3c040038 	lui	a0,0x38
108478:	afa00018 	sw	zero,24(sp)
10847c:	4480a000 	mtc1	zero,$f20
108480:	34040001 	li	a0,0x1
108484:	afa0001c 	sw	zero,28(sp)
108488:	a3a40020 	sb	a0,32(sp)
10848c:	afa00008 	sw	zero,8(sp)
108490:	afa0000c 	sw	zero,12(sp)
108494:	afa00010 	sw	zero,16(sp)
108498:	afa00014 	sw	zero,20(sp)
10849c:	afa00024 	sw	zero,36(sp)
1084a0:	e7b40028 	swc1	$f20,40(sp)
1084a4:	afa00060 	sw	zero,96(sp)
1084a8:	3c050038 	lui	a1,0x38
1084ac:	8cb0d854 	lw	s0,-10156(a1)
1084b0:	3c070022 	lui	a3,0x22
1084b4:	27a4007c 	addiu	a0,sp,124
1084b8:	261003e8 	addiu	s0,s0,1000
1084bc:	34050002 	li	a1,0x2
1084c0:	34060008 	li	a2,0x8
1084c4:	0c06e719 	jal	0x1b9c64
1084c8:	24e775f0 	addiu	a3,a3,30192
1084cc:	e7b4002c 	swc1	$f20,44(sp)
1084d0:	e7b40030 	swc1	$f20,48(sp)
1084d4:	e7b40034 	swc1	$f20,52(sp)
1084d8:	e7b40038 	swc1	$f20,56(sp)
1084dc:	e7b4003c 	swc1	$f20,60(sp)
1084e0:	e7b40040 	swc1	$f20,64(sp)
1084e4:	e7b40044 	swc1	$f20,68(sp)
1084e8:	e7b40048 	swc1	$f20,72(sp)
1084ec:	e7b4004c 	swc1	$f20,76(sp)
1084f0:	afa00064 	sw	zero,100(sp)
1084f4:	afa00068 	sw	zero,104(sp)
1084f8:	afa0006c 	sw	zero,108(sp)
1084fc:	afa00070 	sw	zero,112(sp)
108500:	afa00074 	sw	zero,116(sp)
108504:	afa00078 	sw	zero,120(sp)
108508:	8e040008 	lw	a0,8(s0)
10850c:	2c840001 	sltiu	a0,a0,1
108510:	308400ff 	andi	a0,a0,0xff
108514:	1480001c 	bnez	a0,0x108588
108518:	3c040038 	lui	a0,0x38
10851c:	3c040038 	lui	a0,0x38
108520:	8c84d7b0 	lw	a0,-10320(a0)
108524:	38840001 	xori	a0,a0,0x1
108528:	2c840001 	sltiu	a0,a0,1
10852c:	308400ff 	andi	a0,a0,0xff
108530:	10800015 	beqz	a0,0x108588
108534:	3c040038 	lui	a0,0x38
108538:	3c120038 	lui	s2,0x38
10853c:	8e44d7b4 	lw	a0,-10316(s2)
108540:	afa000e8 	sw	zero,232(sp)
108544:	afa400e4 	sw	a0,228(sp)
108548:	27a40008 	addiu	a0,sp,8
10854c:	afb000ec 	sw	s0,236(sp)
108550:	27a50024 	addiu	a1,sp,36
108554:	afa400f0 	sw	a0,240(sp)
108558:	27a400e4 	addiu	a0,sp,228
10855c:	afa500f4 	sw	a1,244(sp)
108560:	ae44d7b4 	sw	a0,-10316(s2)
108564:	8ca40000 	lw	a0,0(a1)
108568:	2406fffc 	li	a2,-4
10856c:	00862024 	and	a0,a0,a2
108570:	aca40000 	sw	a0,0(a1)
108574:	0c0354d2 	jal	0xd5348
108578:	26040008 	addiu	a0,s0,8
10857c:	8fa400e4 	lw	a0,228(sp)
108580:	ae44d7b4 	sw	a0,-10316(s2)
108584:	3c040038 	lui	a0,0x38
108588:	8c84d858 	lw	a0,-10152(a0)
10858c:	30840001 	andi	a0,a0,0x1
108590:	10800034 	beqz	a0,0x108664
108594:	00000000 	nop
108598:	0c069593 	jal	0x1a564c
10859c:	00000000 	nop
1085a0:	afa20100 	sw	v0,256(sp)
1085a4:	8fa40100 	lw	a0,256(sp)
1085a8:	afa40004 	sw	a0,4(sp)
1085ac:	0c06a0dc 	jal	0x1a8370
1085b0:	8fa40004 	lw	a0,4(sp)
1085b4:	1040002b 	beqz	v0,0x108664
1085b8:	00000000 	nop
1085bc:	12200029 	beqz	s1,0x108664
1085c0:	00000000 	nop
1085c4:	0c069593 	jal	0x1a564c
1085c8:	00000000 	nop
1085cc:	afa20104 	sw	v0,260(sp)
1085d0:	8fa40104 	lw	a0,260(sp)
1085d4:	afa40004 	sw	a0,4(sp)
1085d8:	0c06a0dc 	jal	0x1a8370
1085dc:	8fa40004 	lw	a0,4(sp)
1085e0:	90440020 	lbu	a0,32(v0)
1085e4:	1080001f 	beqz	a0,0x108664
1085e8:	00000000 	nop
1085ec:	afa000b4 	sw	zero,180(sp)
1085f0:	27a400b8 	addiu	a0,sp,184
1085f4:	00002825 	move	a1,zero
1085f8:	0c06b59a 	jal	0x1ad668
1085fc:	34060010 	li	a2,0x10
108600:	afa000c8 	sw	zero,200(sp)
108604:	8e240000 	lw	a0,0(s1)
108608:	27b0008c 	addiu	s0,sp,140
10860c:	24840258 	addiu	a0,a0,600
108610:	84850000 	lh	a1,0(a0)
108614:	8c860004 	lw	a2,4(a0)
108618:	02252021 	addu	a0,s1,a1
10861c:	00c0f809 	jalr	a2
108620:	02002825 	move	a1,s0
108624:	0c0423d5 	jal	0x108f54
108628:	00002025 	move	a0,zero
10862c:	24440001 	addiu	a0,v0,1
108630:	afa400d8 	sw	a0,216(sp)
108634:	3c040038 	lui	a0,0x38
108638:	8c84d854 	lw	a0,-10156(a0)
10863c:	0c041a04 	jal	0x106810
108640:	248516f0 	addiu	a1,a0,5872
108644:	afa200dc 	sw	v0,220(sp)
108648:	8e240000 	lw	a0,0(s1)
10864c:	02002825 	move	a1,s0
108650:	24840260 	addiu	a0,a0,608
108654:	84860000 	lh	a2,0(a0)
108658:	8c870004 	lw	a3,4(a0)
10865c:	00e0f809 	jalr	a3
108660:	02262021 	addu	a0,s1,a2
108664:	c7b40108 	lwc1	$f20,264(sp)
108668:	8fb0010c 	lw	s0,268(sp)
10866c:	8fb10110 	lw	s1,272(sp)
108670:	8fb20114 	lw	s2,276(sp)
108674:	8fbf0118 	lw	ra,280(sp)
108678:	03e00008 	jr	ra
10867c:	27bd0120 	addiu	sp,sp,288
```

## m2c Starting Point

```c
// Decompiled: gcNetGame::Update(cTimeValue) static
// Address: 0x00108420, Size: 608B
// Obj: gcAll_psp.obj

? __vec_new(? *, ?, ?, ?);                          /* extern */
? gcExpressionList__Evaluate_voidconst(void *, s32 *, ?); /* extern */
s32 gcGame__GetMapIndex_constcGUIDT_gcMap_refconst(s32, s32); /* extern */
s32 gcNetGame__GetNumActiveConnections_unsignedintstatic(?); /* extern */
? memset(? *, ?, ?);                                /* extern */
s32 nwNetwork__GetGameSocket_voidstatic(s32);       /* extern */
void **nwNetwork__GetLobby_voidstatic();            /* extern */
? nwNetwork__Update_cTimeValuestatic(s32);          /* extern */
void *nwSocket__GetSocket_nwSocketHandlestatic(s32); /* extern */

void gcNetGame__Update_cTimeValuestatic(s32 arg0) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s8 sp20;
    s32 sp24;
    f32 sp28;
    f32 sp2C;
    f32 sp30;
    f32 sp34;
    f32 sp38;
    f32 sp3C;
    f32 sp40;
    f32 sp44;
    f32 sp48;
    f32 sp4C;
    s32 sp60;
    s32 sp64;
    s32 sp68;
    s32 sp6C;
    s32 sp70;
    s32 sp74;
    s32 sp78;
    ? sp7C;
    ? sp8C;
    s32 spB4;
    ? spB8;
    s32 spC8;
    s32 spD8;
    s32 spDC;
    ? *spE4;
    s32 spE8;
    void *spEC;
    s32 *spF0;
    s32 *spF4;
    s32 sp100;
    s32 sp104;
    ? (*temp_a2_2)(void *, ? *, ?);
    ? (*temp_a3)(void *, ? *, s16, ?);
    s16 temp_a1;
    s16 temp_a2_3;
    s32 (*temp_a2)(void *, s16, ?);
    s32 temp_a0_2;
    s32 temp_a0_4;
    void **temp_v0;
    void *temp_a0;
    void *temp_a0_3;
    void *temp_a0_5;
    void *temp_s0;

    nwNetwork__Update_cTimeValuestatic(subroutine_arg0);
    temp_v0 = nwNetwork__GetLobby_voidstatic();
    if (temp_v0 != NULL) {
        temp_a0 = *temp_v0;
        temp_a1 = temp_a0->unk218;
        temp_a2 = (temp_a0 + 0x218)->unk4;
        if (temp_a2(temp_v0 + temp_a1, temp_a1, temp_a2) != 0) {
            sp18 = 0;
            sp1C = 0;
            sp20 = 1;
            sp8 = 0;
            spC = 0;
            sp10 = 0;
            sp14 = 0;
            sp24 = 0;
            sp28 = 0.0f;
            sp60 = 0;
            temp_s0 = *(s32 *)0x37D854 + 0x3E8;
            __vec_new(&sp7C, 2, 8, 0x2275F0);
            sp2C = 0.0f;
            sp30 = 0.0f;
            sp34 = 0.0f;
            sp38 = 0.0f;
            sp3C = 0.0f;
            sp40 = 0.0f;
            sp44 = 0.0f;
            sp48 = 0.0f;
            sp4C = 0.0f;
            sp64 = 0;
            sp68 = 0;
            sp6C = 0;
            sp70 = 0;
            sp74 = 0;
            sp78 = 0;
            if (!((temp_s0->unk8 == 0) & 0xFF) && ((*(s32 *)0x37D7B0 == 1) & 0xFF)) {
                spE8 = 0;
                spE4 = *(? **)0x37D7B4;
                spEC = temp_s0;
                spF0 = &sp8;
                spF4 = &sp24;
                *(? **)0x37D7B4 = &spE4;
                sp24 &= ~3;
                gcExpressionList__Evaluate_voidconst(temp_s0 + 8, &sp24, -4);
                *(? **)0x37D7B4 = spE4;
            }
        }
    }
    temp_a0_2 = *(s32 *)0x37D858 & 1;
    if (temp_a0_2 != 0) {
        sp100 = nwNetwork__GetGameSocket_voidstatic(temp_a0_2);
        if ((nwSocket__GetSocket_nwSocketHandlestatic(subroutine_arg1) != NULL) && (temp_v0 != NULL)) {
            sp104 = nwNetwork__GetGameSocket_voidstatic();
            if (nwSocket__GetSocket_nwSocketHandlestatic(subroutine_arg1)->unk20 != 0) {
                spB4 = 0;
                memset(&spB8, 0, 0x10);
                spC8 = 0;
                temp_a0_3 = *temp_v0;
                temp_a2_2 = (temp_a0_3 + 0x258)->unk4;
                temp_a2_2(temp_v0 + temp_a0_3->unk258, &sp8C, temp_a2_2);
                spD8 = gcNetGame__GetNumActiveConnections_unsignedintstatic(0) + 1;
                temp_a0_4 = *(void *)0x37D854;
                spDC = gcGame__GetMapIndex_constcGUIDT_gcMap_refconst(temp_a0_4, temp_a0_4 + 0x16F0);
                temp_a0_5 = *temp_v0;
                temp_a2_3 = temp_a0_5->unk260;
                temp_a3 = (temp_a0_5 + 0x260)->unk4;
                temp_a3(temp_v0 + temp_a2_3, &sp8C, temp_a2_3, temp_a3);
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
