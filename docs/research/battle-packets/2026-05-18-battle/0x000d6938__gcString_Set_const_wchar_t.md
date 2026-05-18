# Battle Packet: `gcString::Set(const wchar_t *)`

## Target

- Address: `0x000d6938`
- Size: `608` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcString`
- Method family: `Set`
- Leaf: `False`

## Queue Metadata

- score: `175`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 5B; codegen-specific diagnosis`
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

## Graph Context

Callees:
- `0x00001314` `untried` 84B `cStrLength(const wchar_t *)`
- `0x000014d8` `untried` 80B `cStrCopy(wchar_t *, const wchar_t *)`
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x000d6dec` `matched` 348B `gcStringTable::Set(int, const wchar_t *)`

## Placement

- Canonical source: `src/gcString.cpp`
- Header(s): `include/gcString.h`
- Allowed source(s): `src/gcString.cpp`
- Split-TU prefix: `src/gcString_*.cpp`

## Class Header: `include/gcString.h`

Not found.

## Matched Same-Class Neighbors

- `0x000d6214` 264B `gcString::Write(cFile &) const` — `src/gcString.cpp`
- `0x000d675c` 52B `gcString::gcString(cBase *)` — `src/eRenderSurface.cpp`
- `0x0023a92c` 124B `gcString::New(cMemPool *, cBase *) static` — `src/gcString.cpp`
- `0x0023a9a8` 160B `gcString::GetType(void) const` — `src/gcString.cpp`
- `0x0023aa48` 56B `gcString::GetName(char *) const` — `src/gcMsgCheckSynchronization.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcStringTable::Set(int, const wchar_t *)`

- Address: `0x000d6dec`
- Size: `348` bytes
- Source: `src/gcStringTable.cpp`

```cpp
// ── gcStringTable::Set(int, const wchar_t *) @ 0x000d6dec ──
void gcStringTable::Set(int index, const wchar_t *text) {
    int len = cStrLength(text);
    int offset = index * 4;
    gcString *old = *(gcString **)((char *)mArray.mData + offset);

    if (len == 0) {
        if (old != 0) {
            char *type = *(char **)((char *)old + 4);
            gcStringTable_SetEntry *entry =
                (gcStringTable_SetEntry *)(type + 0x50);
            entry->fn((char *)old + entry->offset, 3, (void *)entry->fn,
                      entry->offset);
            *(gcString **)((char *)mArray.mData + offset) = 0;
        }
    } else {
        if (old != 0) {
            old->Set(text);
            return;
        }

        gcString *string = 0;
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        void *block = ((void **)pool)[9];
        AllocEntry *entry =
            (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
        gcString *allocated =
            (gcString *)entry->fn((char *)block + entry->offset,
                                  0x24, 4, 0, 0);
        if (allocated != 0) {
            new (allocated) gcString((cBase *)this);
            string = allocated;
        }
        string->Set(text);
        string->mSubHandle = (((cIRand() & 0x7fff) | 1) << 16) | index;
        *(gcString **)((char *)mArray.mData + offset) = string;
    }
}
```

### Exemplar 2: `gcEntityTemplateRelationship::Set(cObject *) const`

- Address: `0x00153b20`
- Size: `336` bytes
- Source: `src/gcEntityTemplateRelationship.cpp`

```cpp
void gcEntityTemplateRelationship::Set(cObject *) const {
    gcDesiredObjectHelper::gcPrimary primary =
        (gcDesiredObjectHelper::gcPrimary)*(int *)((const char *)this + 0x20);
    int handle = *(int *)((const char *)this + 0x24);

    if (D_0009F448 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F448 = cType::InitializeType(
            0, 0, 0x8E, D_000385E4, &gcEntityTemplate::New,
            (const char *)0x36D9B8, (const char *)0x36D9C8, 5);
    }
    const cType *type = D_0009F448;
    register int flag __asm__("$7");
    __asm__ volatile("ori %0, $0, 1" : "=r"(flag));
    gcDesiredObjectHelper_GetDesiredObject_Type(primary, handle, type, flag);
}
```

### Exemplar 3: `gcLobbyGameStrings::Set(const wchar_t *) const`

- Address: `0x002818b8`
- Size: `228` bytes
- Source: `src/gcLobbyGameStrings.cpp`

```cpp
// ── gcLobbyGameStrings::Set(const wchar_t *) const @ 0x002818b8, 228B ──
void gcLobbyGameStrings::Set(const wchar_t *src) const {
    LobbyGameInfo info;
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0 && mField08 == 2) {
        info.field28 = 0;
        memset(info.field2C, 0, 0x10);
        info.field3C = 0;

        LobbyDispatchEntry *get =
            (LobbyDispatchEntry *)(*(char **)lobby + 0x258);
        get->fn((char *)lobby + get->offset, &info, get->offset);

        int index = mField10;
        switch (index) {
        case 0:
            cStrCopy(info.text0, src, 0x14);
            break;
        case 1:
            cStrCopy(info.text1, src, 8);
            break;
        }

        LobbyDispatchEntry *set =
            (LobbyDispatchEntry *)(*(char **)lobby + 0x260);
        set->fn((char *)lobby + set->offset, &info, set->offset);
    }
}
```

### Exemplar 4: `gcLobbyConfigStrings::Set(const wchar_t *) const`

- Address: `0x00280728`
- Size: `128` bytes
- Source: `src/gcLobbyConfigStrings.cpp`

```cpp
// ── gcLobbyConfigStrings::Set(const wchar_t *) const  @ 0x00280728, 128B ──
void gcLobbyConfigStrings::Set(const wchar_t *src) const {
    char buf[256];
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0) {
        buf[0] = 0;
        cStrCopy(buf, src, 0xff);
        if (this->mText == 0) {
            DispatchEntry *e = (DispatchEntry *)(*(char **)lobby + 0x20);
            e->fn((char *)lobby + e->offset, buf, e->offset);
        }
    }
}
```


## Prior Failure Notes

### Note 1

- session `d5b36e6a`; src `src/gcString.cpp`; snapshot `logs/failure_snapshots/match_20260517_210450/d5b36e6a/000d6938__agent_self_reported_failure__src_gcString.cpp`

Retained the prior best exact-size gcString::Set(const wchar_t *) attempt in canonical C++ in src/gcString.cpp. Final compare remains 244/608 bytes different after relocation masking; matched siblings Write/New/GetType still compare cleanly and the pre-existing AssignCopy mismatch remains 5 bytes. The retained source implements the null/empty clear path, cStrLength+1 sizing, alloc record +0x28 with tag 0x36DA98 and line 0x112, zero-fill/copy loop, old-buffer delete via alloc record +0x30, and final cStrCopy.

This session read the battle packet plus branch-likely, delay-slot, loop/switch, and register-allocation docs. I tried a cArray-style source-shape change that made the empty branch carry an explicit zero pointer, reversed the oldCount/newCount equality source order, and rewrote the resize loop to use a moving dstIter like nearby cArray examples. SNC kept the broad register/scheduling shape and compare worsened to 384/608, so I reverted to the prior best. The remaining mismatch is too broad for last-mile permuter; likely still needs a different high-level cArray<wchar_t> resize seed rather than local scheduling tweaks.

### Note 2

- session `994a6735`; src `src/gcString.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/994a6735/000d6938__agent_self_reported_failure__src_gcString.cpp`

Added gcString::Set(const wchar_t *) in canonical C++ in src/gcString.cpp. Closest source is exact-size 608B and implements the null/empty clear path, cStrLength+1 sizing, cArray-style allocation through alloc record +0x28 with tag 0x36DA98 and line 0x112, copy/zero-fill loop, old-buffer pool delete, and final cStrCopy. Matched siblings Write/New/GetType still compare cleanly; existing AssignCopy remains its prior 5-byte mismatch. Remaining Set mismatch is 244/608 bytes after relocation masking: control flow and semantics are close, but register scheduling differs across the empty clear path, allocation record load/call setup, copy loop pointer/check temporaries, and final delete path. Tried helper vs direct delete blocks, explicit bgtz allocation diamond using gotos (recovered exact size), recomputing dst+byteOff inside the copy loop, explicit alloc temporaries for offset/fn/base/size, zero-store variants for the empty path, and sched=1 (worse at 267/608). Diff is too broad for last-mile permuter per repo guidance; likely needs a different source-shape seed for cArrayBase<wchar_t> reallocation rather than local permutation.


## Disassembly

```asm
d6938:	27bdffe0 	addiu	sp,sp,-32
d693c:	afb00000 	sw	s0,0(sp)
d6940:	00808025 	move	s0,a0
d6944:	afb10004 	sw	s1,4(sp)
d6948:	afb20008 	sw	s2,8(sp)
d694c:	afb3000c 	sw	s3,12(sp)
d6950:	afb40010 	sw	s4,16(sp)
d6954:	afbf0014 	sw	ra,20(sp)
d6958:	10a00004 	beqz	a1,0xd696c
d695c:	00a08825 	move	s1,a1
d6960:	96240000 	lhu	a0,0(s1)
d6964:	14800029 	bnez	a0,0xd6a0c
d6968:	00000000 	nop
d696c:	8e120018 	lw	s2,24(s0)
d6970:	12400005 	beqz	s2,0xd6988
d6974:	34040000 	li	a0,0x0
d6978:	8e44fffc 	lw	a0,-4(s2)
d697c:	3c054000 	lui	a1,0x4000
d6980:	24a5ffff 	addiu	a1,a1,-1
d6984:	00852024 	and	a0,a0,a1
d6988:	1080007b 	beqz	a0,0xd6b78
d698c:	34110000 	li	s1,0x0
d6990:	12400005 	beqz	s2,0xd69a8
d6994:	34040000 	li	a0,0x0
d6998:	8e44fffc 	lw	a0,-4(s2)
d699c:	3c054000 	lui	a1,0x4000
d69a0:	24a5ffff 	addiu	a1,a1,-1
d69a4:	00852024 	and	a0,a0,a1
d69a8:	34050000 	li	a1,0x0
d69ac:	00a4302a 	slt	a2,a1,a0
d69b0:	10c00005 	beqz	a2,0xd69c8
d69b4:	00000000 	nop
d69b8:	24a50001 	addiu	a1,a1,1
d69bc:	00a4302a 	slt	a2,a1,a0
d69c0:	14c0fffe 	bnez	a2,0xd69bc
d69c4:	24a50001 	addiu	a1,a1,1
d69c8:	1240000e 	beqz	s2,0xd6a04
d69cc:	2652fffc 	addiu	s2,s2,-4
d69d0:	5240000c 	beqzl	s2,0xd6a04
d69d4:	ae000018 	sw	zero,24(s0)
d69d8:	0c0011e8 	jal	0x47a0
d69dc:	02402025 	move	a0,s2
d69e0:	8c440024 	lw	a0,36(v0)
d69e4:	02402825 	move	a1,s2
d69e8:	8c86001c 	lw	a2,28(a0)
d69ec:	24c60030 	addiu	a2,a2,48
d69f0:	84c70000 	lh	a3,0(a2)
d69f4:	8cc60004 	lw	a2,4(a2)
d69f8:	00c0f809 	jalr	a2
d69fc:	00872021 	addu	a0,a0,a3
d6a00:	ae000018 	sw	zero,24(s0)
d6a04:	1000005c 	b	0xd6b78
d6a08:	ae110018 	sw	s1,24(s0)
d6a0c:	0c0004c5 	jal	0x1314
d6a10:	02202025 	move	a0,s1
d6a14:	8e140018 	lw	s4,24(s0)
d6a18:	24520001 	addiu	s2,v0,1
d6a1c:	12800005 	beqz	s4,0xd6a34
d6a20:	34130000 	li	s3,0x0
d6a24:	8e93fffc 	lw	s3,-4(s4)
d6a28:	3c044000 	lui	a0,0x4000
d6a2c:	2484ffff 	addiu	a0,a0,-1
d6a30:	02649824 	and	s3,s3,a0
d6a34:	1272004e 	beq	s3,s2,0xd6b70
d6a38:	02802025 	move	a0,s4
d6a3c:	1e400003 	bgtz	s2,0xd6a4c
d6a40:	00000000 	nop
d6a44:	10000012 	b	0xd6a90
d6a48:	34140000 	li	s4,0x0
d6a4c:	0c0011e8 	jal	0x47a0
d6a50:	26040018 	addiu	a0,s0,24
d6a54:	8c440024 	lw	a0,36(v0)
d6a58:	02522821 	addu	a1,s2,s2
d6a5c:	8c86001c 	lw	a2,28(a0)
d6a60:	24a50004 	addiu	a1,a1,4
d6a64:	24c60028 	addiu	a2,a2,40
d6a68:	84c70000 	lh	a3,0(a2)
d6a6c:	8cc90004 	lw	t1,4(a2)
d6a70:	00872021 	addu	a0,a0,a3
d6a74:	3c070037 	lui	a3,0x37
d6a78:	34060002 	li	a2,0x2
d6a7c:	24e7da98 	addiu	a3,a3,-9576
d6a80:	0120f809 	jalr	t1
d6a84:	34080112 	li	t0,0x112
d6a88:	ac520000 	sw	s2,0(v0)
d6a8c:	24540004 	addiu	s4,v0,4
d6a90:	16800005 	bnez	s4,0xd6aa8
d6a94:	34040000 	li	a0,0x0
d6a98:	1a400003 	blez	s2,0xd6aa8
d6a9c:	34040000 	li	a0,0x0
d6aa0:	10000032 	b	0xd6b6c
d6aa4:	8e140018 	lw	s4,24(s0)
d6aa8:	0092282a 	slt	a1,a0,s2
d6aac:	10a00010 	beqz	a1,0xd6af0
d6ab0:	34050000 	li	a1,0x0
d6ab4:	02853021 	addu	a2,s4,a1
d6ab8:	00c03825 	move	a3,a2
d6abc:	10e00007 	beqz	a3,0xd6adc
d6ac0:	0093482a 	slt	t1,a0,s3
d6ac4:	11200004 	beqz	t1,0xd6ad8
d6ac8:	34080000 	li	t0,0x0
d6acc:	8e080018 	lw	t0,24(s0)
d6ad0:	01054021 	addu	t0,t0,a1
d6ad4:	95080000 	lhu	t0,0(t0)
d6ad8:	a4e80000 	sh	t0,0(a3)
d6adc:	24840001 	addiu	a0,a0,1
d6ae0:	24a50002 	addiu	a1,a1,2
d6ae4:	0092382a 	slt	a3,a0,s2
d6ae8:	14e0fff3 	bnez	a3,0xd6ab8
d6aec:	24c60002 	addiu	a2,a2,2
d6af0:	8e120018 	lw	s2,24(s0)
d6af4:	12400005 	beqz	s2,0xd6b0c
d6af8:	34040000 	li	a0,0x0
d6afc:	8e44fffc 	lw	a0,-4(s2)
d6b00:	3c054000 	lui	a1,0x4000
d6b04:	24a5ffff 	addiu	a1,a1,-1
d6b08:	00852024 	and	a0,a0,a1
d6b0c:	34050000 	li	a1,0x0
d6b10:	00a4302a 	slt	a2,a1,a0
d6b14:	10c00005 	beqz	a2,0xd6b2c
d6b18:	00000000 	nop
d6b1c:	24a50001 	addiu	a1,a1,1
d6b20:	00a4302a 	slt	a2,a1,a0
d6b24:	14c0fffe 	bnez	a2,0xd6b20
d6b28:	24a50001 	addiu	a1,a1,1
d6b2c:	1240000e 	beqz	s2,0xd6b68
d6b30:	2652fffc 	addiu	s2,s2,-4
d6b34:	5240000c 	beqzl	s2,0xd6b68
d6b38:	ae000018 	sw	zero,24(s0)
d6b3c:	0c0011e8 	jal	0x47a0
d6b40:	02402025 	move	a0,s2
d6b44:	8c440024 	lw	a0,36(v0)
d6b48:	02402825 	move	a1,s2
d6b4c:	8c86001c 	lw	a2,28(a0)
d6b50:	24c60030 	addiu	a2,a2,48
d6b54:	84c70000 	lh	a3,0(a2)
d6b58:	8cc60004 	lw	a2,4(a2)
d6b5c:	00c0f809 	jalr	a2
d6b60:	00872021 	addu	a0,a0,a3
d6b64:	ae000018 	sw	zero,24(s0)
d6b68:	ae140018 	sw	s4,24(s0)
d6b6c:	02802025 	move	a0,s4
d6b70:	0c000536 	jal	0x14d8
d6b74:	02202825 	move	a1,s1
d6b78:	8fb00000 	lw	s0,0(sp)
d6b7c:	8fb10004 	lw	s1,4(sp)
d6b80:	8fb20008 	lw	s2,8(sp)
d6b84:	8fb3000c 	lw	s3,12(sp)
d6b88:	8fb40010 	lw	s4,16(sp)
d6b8c:	8fbf0014 	lw	ra,20(sp)
d6b90:	03e00008 	jr	ra
d6b94:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcString::Set(const wchar_t *)
// Address: 0x000d6938, Size: 608B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(u16 *, s32, s32); /* extern */
? cStrCopy_wchar_tptr_constwchar_tptr(u16 *, u16 *); /* extern */
s32 cStrLength_constwchar_tptr(u16 *);              /* extern */

void gcString__Set_constwchar_tptr(void *arg0, u16 *arg1) {
    ? (*temp_a2_2)(void *, u16 *, ?, s16);
    ? (*temp_a2_5)(void *, u16 *, ?, s16);
    s16 temp_a3;
    s16 temp_a3_2;
    s32 *temp_v0;
    s32 temp_s2_3;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a2;
    s32 var_a2_3;
    s32 var_s3;
    u16 *temp_s2;
    u16 *temp_s2_2;
    u16 *temp_s2_4;
    u16 *temp_s2_5;
    u16 *temp_s4;
    u16 *var_a0_3;
    u16 *var_a2_2;
    u16 *var_s4;
    u16 var_t0;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a2;
    void *temp_a2_3;
    void *temp_a2_4;

    if ((arg1 == NULL) || (*arg1 == 0)) {
        temp_s2 = arg0->unk18;
        var_a0 = 0;
        if (temp_s2 != NULL) {
            var_a0 = temp_s2->unk-4 & 0x3FFFFFFF;
        }
        if (var_a0 != 0) {
            var_a0_2 = 0;
            if (temp_s2 != NULL) {
                var_a0_2 = temp_s2->unk-4 & 0x3FFFFFFF;
            }
            var_a1 = 0;
            var_a2 = var_a0_2 > 0;
            if (var_a2 != 0) {
                var_a1 = 1;
                do {
                    var_a2 = var_a1 < var_a0_2;
                    var_a1 += 1;
                } while (var_a2 != 0);
            }
            temp_s2_2 = temp_s2 - 4;
            if (temp_s2 != NULL) {
                if (temp_s2_2 != NULL) {
                    temp_a0 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_2, var_a1, var_a2)->unk24;
                    temp_a2 = temp_a0->unk1C;
                    temp_a3 = temp_a2->unk30;
                    temp_a2_2 = (temp_a2 + 0x30)->unk4;
                    temp_a2_2(temp_a0 + temp_a3, temp_s2_2, temp_a2_2, temp_a3);
                }
                arg0->unk18 = NULL;
            }
            arg0->unk18 = NULL;
        }
    } else {
        temp_s4 = arg0->unk18;
        temp_s2_3 = cStrLength_constwchar_tptr(arg1) + 1;
        var_s3 = 0;
        if (temp_s4 != NULL) {
            var_s3 = temp_s4->unk-4 & 0x3FFFFFFF;
        }
        var_a0_3 = temp_s4;
        if (var_s3 != temp_s2_3) {
            if (temp_s2_3 <= 0) {
                var_s4 = NULL;
            } else {
                temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0 + 0x18)->unk24;
                temp_a2_3 = temp_a0_2->unk1C;
                temp_v0 = (temp_a2_3 + 0x28)->unk4(temp_a0_2 + temp_a2_3->unk28, (temp_s2_3 * 2) + 4, 2, 0x36DA98);
                *temp_v0 = temp_s2_3;
                var_s4 = temp_v0 + 4;
            }
            var_a0_4 = 0;
            if ((var_s4 == NULL) && (var_a0_4 = 0, (temp_s2_3 > 0))) {
                var_s4 = arg0->unk18;
            } else {
                var_a1_2 = 0;
                if (temp_s2_3 > 0) {
                    var_a2_2 = var_s4;
                    do {
                        if (var_a2_2 != NULL) {
                            var_t0 = 0;
                            if (var_a0_4 < var_s3) {
                                var_t0 = *(arg0->unk18 + var_a1_2);
                            }
                            *var_a2_2 = var_t0;
                        }
                        var_a0_4 += 1;
                        var_a1_2 += 2;
                        var_a2_2 += 2;
                    } while (var_a0_4 < temp_s2_3);
                }
                temp_s2_4 = arg0->unk18;
                var_a0_5 = 0;
                if (temp_s2_4 != NULL) {
                    var_a0_5 = temp_s2_4->unk-4 & 0x3FFFFFFF;
                }
                var_a1_3 = 0;
                var_a2_3 = var_a0_5 > 0;
                if (var_a2_3 != 0) {
                    var_a1_3 = 1;
                    do {
                        var_a2_3 = var_a1_3 < var_a0_5;
                        var_a1_3 += 1;
                    } while (var_a2_3 != 0);
                }
                temp_s2_5 = temp_s2_4 - 4;
                if (temp_s2_4 != NULL) {
                    if (temp_s2_5 != NULL) {
                        temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_5, var_a1_3, var_a2_3)->unk24;
                        temp_a2_4 = temp_a0_3->unk1C;
                        temp_a3_2 = temp_a2_4->unk30;
                        temp_a2_5 = (temp_a2_4 + 0x30)->unk4;
                        temp_a2_5(temp_a0_3 + temp_a3_2, temp_s2_5, temp_a2_5, temp_a3_2);
                    }
                    arg0->unk18 = NULL;
                }
                arg0->unk18 = var_s4;
            }
            var_a0_3 = var_s4;
        }
        cStrCopy_wchar_tptr_constwchar_tptr(var_a0_3, arg1);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
