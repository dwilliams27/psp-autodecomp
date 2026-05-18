# Battle Packet: `gcEventStackData::Read(cInStream &)`

## Target

- Address: `0x000d5dc8`
- Size: `468` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEventStackData`
- Method family: `Read`
- Leaf: `False`

## Queue Metadata

- score: `126`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 2B; codegen-specific diagnosis; Read hard family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `2`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00006af8` `matched` 200B `cInStream::Read(unsigned int &, int, bool)`
- `0x00006bc0` `untried` 196B `cInStream::Read(float &, bool)`
- `0x00009370` `matched` 124B `cHandle::Read(cInStream &)`
- `0x001b9c64` `untried` 68B `__vec_new`

Callers:
- `0x00118f88` `untried` 780B `gcEntity::ProcessQueuedMessages(void) static`
- `0x00134238` `untried` 124B `gcMsgEntitySendMessage::ReadEventData(cInStream &, cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &, gcEventParams &, gcEventStackData &) static`

## Placement

- Canonical source: `src/gcEventStackData.cpp`
- Header(s): `include/gcEventStackData.h`
- Allowed source(s): `src/gcEventStackData.cpp`
- Split-TU prefix: `src/gcEventStackData_*.cpp`

## Compiler Guidance

Read template: preserve read-block/stream construction, old-position restore paths, and result initialization location. Compare against same stream type exemplars when available.

## Class Header: `include/gcEventStackData.h`

Not found.

## Matched Same-Class Neighbors

- `0x000d5d30` 152B `gcEventStackData::Write(cOutStream &) const` — `src/gcEventStackData.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcConfig::Read(cFile &, cMemPool *)`

- Address: `0x000efddc`
- Size: `460` bytes
- Source: `src/gcConfig.cpp`

```cpp
// ── gcConfig::Read(cFile &, cMemPool *) @ 0x000efddc ──
int gcConfig::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x18, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x1C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x20, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x28, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x3C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x50, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x64, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x68, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x6C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x70, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x74, 4);

    ConfigReadEntry *slot =
        (ConfigReadEntry *)(*(char **)((char *)this + 4) + 0x38);
    slot->fn((char *)this + slot->offset, pool, false);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 2: `gcEntityAttack::Read(cFile &, cMemPool *)`

- Address: `0x0010efc0`
- Size: `456` bytes
- Source: `src/gcEntityAttack.cpp`

```cpp
int gcEntityAttack::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 6, true);
    if ((unsigned int)rb[3] >= 7 || (unsigned int)rb[3] < 5) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((gcEntityAnimationConfig *)((char *)this + 0x08))->Read(file, pool);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x2C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x30, 4);
    ((cBaseArray *)((char *)this + 0x70))->Read(*(cReadBlock *)rb);

    {
        char *typeInfo = *(char **)((char *)this + 0x3C);
        char *base = (char *)this + 0x38;
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        short off = rec->offset;
        char *target = base + off;
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x34, 4);

    if ((unsigned int)rb[3] >= 6) {
        char sp14;
        cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
        cFile *f1 = *(cFile **)&rb[0];
        *(unsigned char *)((char *)this + 0x54) = sp14 != 0;

        char sp15;
        cFileSystem::Read(*(cFileHandle **)f1, &sp15, 1);
        *(unsigned char *)((char *)this + 0x55) = sp15 != 0;

        char *typeInfo = *(char **)((char *)this + 0x5C);
        char *base = (char *)this + 0x58;
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        short off = rec->offset;
        char *target = base + off;
        __asm__ volatile("" ::: "memory");
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 3: `gcTimer::Read(cFile &, cMemPool *)`

- Address: `0x0012c294`
- Size: `432` bytes
- Source: `src/gcTimer.cpp`

```cpp
int gcTimer::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || ((cObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }

    cFileSystem_Read(*(void **)rb._data[0], (char *)this + 0xB4, 4);
    cFileSystem_Read(*(void **)rb._data[0], (char *)this + 0xBC, 4);

    char *base0 = (char *)this + 0x44;
    TypeMethod *slot0 = (TypeMethod *)(*(char **)((char *)this + 0x48) + 0x30);
    ((ReadFn)slot0->fn)(base0 + slot0->offset,
                        (cFile *)rb._data[0],
                        cMemPool::GetPoolFromPtr(base0));

    char *base1 = (char *)this + 0x60;
    TypeMethod *slot1 = (TypeMethod *)(*(char **)((char *)this + 0x64) + 0x30);
    ((ReadFn)slot1->fn)(base1 + slot1->offset,
                        (cFile *)rb._data[0],
                        cMemPool::GetPoolFromPtr(base1));

    char *base2 = (char *)this + 0x7C;
    TypeMethod *slot2 = (TypeMethod *)(*(char **)((char *)this + 0x80) + 0x30);
    ((ReadFn)slot2->fn)(base2 + slot2->offset,
                        (cFile *)rb._data[0],
                        cMemPool::GetPoolFromPtr(base2));

    char *base3 = (char *)this + 0x98;
    TypeMethod *slot3 = (TypeMethod *)(*(char **)((char *)this + 0x9C) + 0x30);
    ((ReadFn)slot3->fn)(base3 + slot3->offset,
                        (cFile *)rb._data[0],
                        cMemPool::GetPoolFromPtr(base3));
    return result;
}
```

### Exemplar 4: `gcTrigger::Read(cFile &, cMemPool *)`

- Address: `0x0012d1ac`
- Size: `540` bytes
- Source: `src/gcTrigger.cpp`

```cpp
int gcTrigger::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);
    if ((unsigned int)rb[3] >= 5 || (unsigned int)rb[3] < 3 ||
        cObject::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char sp14;
    cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
    *(unsigned char *)((char *)this + 0xA0) = sp14 != 0;

    {
        char *typeInfo = *(char **)((char *)this + 0x48);
        TypeReadEntry *rec = (TypeReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x44;
        short off = rec->offset;
        char *target = base + off;
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x64);
        TypeReadEntry *rec = (TypeReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x60;
        short off = rec->offset;
        char *target = base + off;
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x80);
        TypeReadEntry *rec = (TypeReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0x7C;
        short off = rec->offset;
        char *target = base + off;
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0xB4);
        TypeReadEntry *rec = (TypeReadEntry *)(typeInfo + 0x30);
        char *base = (char *)this + 0xB0;
        short off = rec->offset;
        char *target = base + off;
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0xA4, 4);

    cHandle *handle = (cHandle *)((char *)this + 0xA8);
    *(int *)((char *)this + 0xA8) = 0;
    handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));

    ((cArray<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x98))
        ->Read(*(cReadBlock *)rb);

    if ((unsigned int)rb[3] >= 4) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x9C, 4);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```


## Prior Failure Notes

### Note 1

- session `b5d4c115`

Improved the prior 8-byte mismatch to a 2-byte mismatch by scoping the float-read loop locals, which makes SNC allocate the first read loop as target: counter in s2 and float pointer in s3. The remaining diff is isolated to the temporary handle-copy loop at offsets 0x134/0x13c in the target: expected computes dst+4 in t1 and stores through t1 (`addiu t1,a1,4`; `sw a3,0(t1)`), while the best C build computes dst+4 in t0 and stores through t0. All surrounding bytes, size, calls, stack frame, and later loops match.

Tried word-pointer, char-pointer, aggregate struct copy, pointer-walk, dst+4 declaration reordering, volatile source/destination variants, register-local binding to t1/$9, empty asm liveness inputs/clobbers, and source loop scoping/redeclaration changes. A 300-second permuter run from the 2-byte baseline generated 9,264 candidates and compiled 5,426 with zero improvement. This appears to be a narrow SNC register-allocation divergence for the dst+4 temporary, not a control-flow or semantic reconstruction issue.

### Note 2

- session `dcee29b5`

Reconstructed the full 0x68-byte layout and got the function down to an exact-size 8-byte mismatch. Important quirk: the target really does zero only `temp.mFloats18[0..4]` before copying nine words from `0x18..0x38`; leaving the upper four words uninitialized is required to match the target initialization block. The current source also matches the overall loop structure, stack frame, and later stream-read loops.

The remaining diff is isolated to the two-word copy loop that copies the temporary handle array at `0x58` into `this`. The current best build uses `t0` for both the source `+4` pointer and the destination `+4` pointer, while the original uses `t0` for `a2+4` and `t1` for `a1+4`; that is the final 8-byte mismatch. Tried aggregate assignment, indexed member copies, pointer-walk copies, raw `unsigned int` word copies, expression-order changes, an empty memory barrier, and a 120-second permuter run (`4176` candidates generated, `2872` compiled) with zero improvement. Root cause appears to be SNC register allocation in that narrow copy loop, not the larger function structure.


## Disassembly

```asm
d5dc8:	27bdff80 	addiu	sp,sp,-128
d5dcc:	e7b40068 	swc1	$f20,104(sp)
d5dd0:	4480a000 	mtc1	zero,$f20
d5dd4:	afa00000 	sw	zero,0(sp)
d5dd8:	afb0006c 	sw	s0,108(sp)
d5ddc:	afb10070 	sw	s1,112(sp)
d5de0:	00a08825 	move	s1,a1
d5de4:	00808025 	move	s0,a0
d5de8:	e7b40004 	swc1	$f20,4(sp)
d5dec:	3c070022 	lui	a3,0x22
d5df0:	afa0003c 	sw	zero,60(sp)
d5df4:	27a40058 	addiu	a0,sp,88
d5df8:	34050002 	li	a1,0x2
d5dfc:	34060008 	li	a2,0x8
d5e00:	afb20074 	sw	s2,116(sp)
d5e04:	afb30078 	sw	s3,120(sp)
d5e08:	afbf007c 	sw	ra,124(sp)
d5e0c:	0c06e719 	jal	0x1b9c64
d5e10:	24e775f0 	addiu	a3,a3,30192
d5e14:	e7b40008 	swc1	$f20,8(sp)
d5e18:	e7b4000c 	swc1	$f20,12(sp)
d5e1c:	e7b40010 	swc1	$f20,16(sp)
d5e20:	e7b40014 	swc1	$f20,20(sp)
d5e24:	e7b40018 	swc1	$f20,24(sp)
d5e28:	e7b4001c 	swc1	$f20,28(sp)
d5e2c:	e7b40020 	swc1	$f20,32(sp)
d5e30:	e7b40024 	swc1	$f20,36(sp)
d5e34:	e7b40028 	swc1	$f20,40(sp)
d5e38:	afa00040 	sw	zero,64(sp)
d5e3c:	afa00044 	sw	zero,68(sp)
d5e40:	afa00048 	sw	zero,72(sp)
d5e44:	afa0004c 	sw	zero,76(sp)
d5e48:	8fa40000 	lw	a0,0(sp)
d5e4c:	afa00050 	sw	zero,80(sp)
d5e50:	afa00054 	sw	zero,84(sp)
d5e54:	c7ac0004 	lwc1	$f12,4(sp)
d5e58:	ae040000 	sw	a0,0(s0)
d5e5c:	e60c0004 	swc1	$f12,4(s0)
d5e60:	34040000 	li	a0,0x0
d5e64:	03a02825 	move	a1,sp
d5e68:	02003025 	move	a2,s0
d5e6c:	c4ac0008 	lwc1	$f12,8(a1)
d5e70:	24840001 	addiu	a0,a0,1
d5e74:	e4cc0008 	swc1	$f12,8(a2)
d5e78:	24a50004 	addiu	a1,a1,4
d5e7c:	28870004 	slti	a3,a0,4
d5e80:	14e0fffa 	bnez	a3,0xd5e6c
d5e84:	24c60004 	addiu	a2,a2,4
d5e88:	34040000 	li	a0,0x0
d5e8c:	03a02825 	move	a1,sp
d5e90:	02003025 	move	a2,s0
d5e94:	c4ac0018 	lwc1	$f12,24(a1)
d5e98:	24840001 	addiu	a0,a0,1
d5e9c:	e4cc0018 	swc1	$f12,24(a2)
d5ea0:	24a50004 	addiu	a1,a1,4
d5ea4:	28870009 	slti	a3,a0,9
d5ea8:	14e0fffa 	bnez	a3,0xd5e94
d5eac:	24c60004 	addiu	a2,a2,4
d5eb0:	8fa5003c 	lw	a1,60(sp)
d5eb4:	34040000 	li	a0,0x0
d5eb8:	ae05003c 	sw	a1,60(s0)
d5ebc:	03a02825 	move	a1,sp
d5ec0:	02003025 	move	a2,s0
d5ec4:	8ca70040 	lw	a3,64(a1)
d5ec8:	24840001 	addiu	a0,a0,1
d5ecc:	acc70040 	sw	a3,64(a2)
d5ed0:	24a50004 	addiu	a1,a1,4
d5ed4:	28870006 	slti	a3,a0,6
d5ed8:	14e0fffa 	bnez	a3,0xd5ec4
d5edc:	24c60004 	addiu	a2,a2,4
d5ee0:	34040000 	li	a0,0x0
d5ee4:	26050058 	addiu	a1,s0,88
d5ee8:	27a60058 	addiu	a2,sp,88
d5eec:	8cc70000 	lw	a3,0(a2)
d5ef0:	24c80004 	addiu	t0,a2,4
d5ef4:	aca70000 	sw	a3,0(a1)
d5ef8:	8d070000 	lw	a3,0(t0)
d5efc:	24a90004 	addiu	t1,a1,4
d5f00:	24840001 	addiu	a0,a0,1
d5f04:	ad270000 	sw	a3,0(t1)
d5f08:	24a50008 	addiu	a1,a1,8
d5f0c:	2c870002 	sltiu	a3,a0,2
d5f10:	14e0fff6 	bnez	a3,0xd5eec
d5f14:	24c60008 	addiu	a2,a2,8
d5f18:	34120000 	li	s2,0x0
d5f1c:	26130008 	addiu	s3,s0,8
d5f20:	02202025 	move	a0,s1
d5f24:	02602825 	move	a1,s3
d5f28:	0c001af0 	jal	0x6bc0
d5f2c:	34060001 	li	a2,0x1
d5f30:	26520001 	addiu	s2,s2,1
d5f34:	2a440004 	slti	a0,s2,4
d5f38:	1480fff9 	bnez	a0,0xd5f20
d5f3c:	26730004 	addiu	s3,s3,4
d5f40:	02009025 	move	s2,s0
d5f44:	34100000 	li	s0,0x0
d5f48:	26520058 	addiu	s2,s2,88
d5f4c:	02402025 	move	a0,s2
d5f50:	0c0024dc 	jal	0x9370
d5f54:	02202825 	move	a1,s1
d5f58:	26450004 	addiu	a1,s2,4
d5f5c:	02202025 	move	a0,s1
d5f60:	34060020 	li	a2,0x20
d5f64:	0c001abe 	jal	0x6af8
d5f68:	34070001 	li	a3,0x1
d5f6c:	26100001 	addiu	s0,s0,1
d5f70:	2a040002 	slti	a0,s0,2
d5f74:	1480fff5 	bnez	a0,0xd5f4c
d5f78:	26520008 	addiu	s2,s2,8
d5f7c:	c7b40068 	lwc1	$f20,104(sp)
d5f80:	8fb0006c 	lw	s0,108(sp)
d5f84:	8fb10070 	lw	s1,112(sp)
d5f88:	8fb20074 	lw	s2,116(sp)
d5f8c:	8fb30078 	lw	s3,120(sp)
d5f90:	8fbf007c 	lw	ra,124(sp)
d5f94:	03e00008 	jr	ra
d5f98:	27bd0080 	addiu	sp,sp,128
```

## m2c Starting Point

```c
// Decompiled: gcEventStackData::Read(cInStream &)
// Address: 0x000d5dc8, Size: 468B
// Obj: gcAll_psp.obj

? __vec_new(? *, ?, ?, ?);                          /* extern */
? cHandle__Read_cInStreamref(void *, ?);            /* extern */
? cInStream__Read_floatref_bool(?, void *, ?);      /* extern */
? cInStream__Read_unsignedintref_int_bool(?, void *, ?, ?); /* extern */

void gcEventStackData__Read_cInStreamref(void *arg0, ? arg1) {
    s32 sp3C;
    s32 sp40;
    s32 sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    ? sp58;
    ? *var_a2_4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_s0;
    s32 var_s2;
    u32 var_a0_4;
    void *var_a1;
    void *var_a1_2;
    void *var_a1_3;
    void *var_a1_4;
    void *var_a2;
    void *var_a2_2;
    void *var_a2_3;
    void *var_s2_2;
    void *var_s3;

    sp3C = 0;
    __vec_new(&sp58, 2, 8, 0x2275F0);
    sp40 = 0;
    sp44 = 0;
    sp48 = 0;
    sp4C = 0;
    sp50 = 0;
    sp54 = 0;
    arg0->unk0 = (s32) subroutine_arg0;
    arg0->unk4 = (f32) subroutine_arg1;
    var_a0 = 0;
    var_a1 = sp;
    var_a2 = arg0;
    do {
        var_a0 += 1;
        var_a2->unk8 = (f32) var_a1->unk8;
        var_a1 += 4;
        var_a2 += 4;
    } while (var_a0 < 4);
    var_a0_2 = 0;
    var_a1_2 = sp;
    var_a2_2 = arg0;
    do {
        var_a0_2 += 1;
        var_a2_2->unk18 = (f32) var_a1_2->unk18;
        var_a1_2 += 4;
        var_a2_2 += 4;
    } while (var_a0_2 < 9);
    var_a0_3 = 0;
    arg0->unk3C = sp3C;
    var_a1_3 = sp;
    var_a2_3 = arg0;
    do {
        var_a0_3 += 1;
        var_a2_3->unk40 = (s32) var_a1_3->unk40;
        var_a1_3 += 4;
        var_a2_3 += 4;
    } while (var_a0_3 < 6);
    var_a0_4 = 0;
    var_a1_4 = arg0 + 0x58;
    var_a2_4 = &sp58;
    do {
        var_a1_4->unk0 = (s32) var_a2_4->unk0;
        var_a0_4 += 1;
        var_a1_4->unk4 = (s32) var_a2_4->unk4;
        var_a1_4 += 8;
        var_a2_4 += 8;
    } while (var_a0_4 < 2U);
    var_s2 = 0;
    var_s3 = arg0 + 8;
    do {
        cInStream__Read_floatref_bool(arg1, var_s3, 1);
        var_s2 += 1;
        var_s3 += 4;
    } while (var_s2 < 4);
    var_s0 = 0;
    var_s2_2 = arg0 + 0x58;
    do {
        cHandle__Read_cInStreamref(var_s2_2, arg1);
        cInStream__Read_unsignedintref_int_bool(arg1, var_s2_2 + 4, 0x20, 1);
        var_s0 += 1;
        var_s2_2 += 8;
    } while (var_s0 < 2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
