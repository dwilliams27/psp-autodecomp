# Battle Packet: `gcDoEntityMoveToPoint::Read(cFile &, cMemPool *)`

## Target

- Address: `0x002b4b38`
- Size: `712` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDoEntityMoveToPoint`
- Method family: `Read`
- Leaf: `False`

## Queue Metadata

- score: `130`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 6B; Read hard family`
- failure_action: `prep`
- near_miss_bytes: `6`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `6`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00004ff4` `untried` 128B `cFileSystem::Read(cFileHandle *, void *, unsigned int) static`
- `0x000053c8` `matched` 28B `cFile::SetCurrentPos(unsigned int)`
- `0x00005d00` `failed` 188B `cReadBlock::cReadBlock(cFile &, unsigned int, bool)`
- `0x00005dbc` `failed` 212B `cReadBlock::~cReadBlock(void)`
- `0x00008c78` `matched` 124B `cName::Read(cReadBlock &)`
- `0x000e292c` `matched` 200B `gcDesiredValue::Read(cReadBlock &)`
- `0x0012f3c4` `failed` 260B `gcAction::Read(cFile &, cMemPool *)`

## Placement

- Canonical source: `src/gcDoEntityMoveToPoint.cpp`
- Header(s): `include/gcDoEntityMoveToPoint.h`
- Allowed source(s): `src/gcDoEntityMoveToPoint.cpp`, `src/gcValUITextParam.cpp`
- Split-TU prefix: `src/gcDoEntityMoveToPoint_*.cpp`

## Compiler Guidance

Read warning: many `Read(cFile &, cMemPool *)` methods are blocked by the known cReadBlock prologue scheduler divergence. If the body matches and only the 20-byte prologue differs, record that blocker instead of spending the session on retries.

## Class Header: `include/gcDoEntityMoveToPoint.h`

Not found.

## Matched Same-Class Neighbors

- `0x002b44d4` 420B `gcDoEntityMoveToPoint::AssignCopy(const cBase *)` — `src/gcDoEntityMoveToPoint.cpp`
- `0x002b4678` 404B `gcDoEntityMoveToPoint::gcDoEntityMoveToPoint(cBase *)` — `src/gcDoEntityMoveToPoint.cpp`
- `0x002b480c` 124B `gcDoEntityMoveToPoint::New(cMemPool *, cBase *) static` — `src/gcDoEntityCastRay.cpp`
- `0x002b4888` 280B `gcDoEntityMoveToPoint::GetType(void) const` — `src/gcDoEntityMoveToPoint.cpp`
- `0x002b49a0` 408B `gcDoEntityMoveToPoint::Write(cFile &) const` — `src/gcDoEntityMoveToPoint.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDoEntityPlaySound::Read(cFile &, cMemPool *)`

- Address: `0x002bdf30`
- Size: `648` bytes
- Source: `src/gcDoEntityPlaySound.cpp`

```cpp
int gcDoEntityPlaySound::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char flag0;
    char flag1;

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] >= 6 || (unsigned int)rb[3] < 3 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    {
        char *typeInfo = *(char **)((char *)this + 0x14);
        char *base = (char *)this + 0x10;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    int version = rb[3];
    if ((unsigned int)version < 4) {
        ((cBaseArray *)((char *)this + 0x3C))->SetSize(1);

        void *block = ((void **)pool)[9];
        register char *allocTable __asm__("$6") = ((PoolBlock *)block)->allocTable;
        gcDesiredObject *obj = 0;
        AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
        register int off __asm__("$7") = entry->offset;
        void *base = (char *)block + off;
        gcDesiredObject *created =
            (gcDesiredObject *)entry->fn(base, 0x14, 4, 0, 0);
        if (created != 0) {
            gcDesiredObject_gcDesiredObject(created, (cBase *)this);
            *(void **)((char *)created + 0x04) = D_00000658;
            *(int *)((char *)created + 0x0C) = 7;
            *(int *)((char *)created + 0x10) = 0;
            *(void **)((char *)created + 0x04) = (void *)0x389800;
            obj = created;
        }

        **(gcDesiredObject ***)((char *)this + 0x3C) = obj;
        {
            char *desired = **(char ***)((char *)this + 0x3C);
            char *typeInfo = (char *)((void **)desired)[1];
            ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
            short roff = rec->offset;
            cFile *f = *(cFile **)&rb[0];
            rec->fn(desired + roff, f, cMemPool::GetPoolFromPtr(desired));
        }
        version = rb[3];
    }

    if ((unsigned int)version >= 4) {
        ((cBaseArray *)((char *)this + 0x3C))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x44))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x48))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x4C))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x50))->Read(*(cReadBlock *)rb);
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], &flag0, 1);
    int version2 = rb[3];
    *(unsigned char *)((char *)this + 0x54) = flag0 != 0;
    if ((unsigned int)version2 >= 5) {
        cFileSystem::Read(*(cFileHandle **)rb[0], &flag1, 1);
        *(unsigned char *)((char *)this + 0x55) = flag1 != 0;
        ((gcDesiredValue *)((char *)this + 0x58))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 2: `gcDoLobbyScoreboardOp::Read(cFile &, cMemPool *)`

- Address: `0x002e3d28`
- Size: `548` bytes
- Source: `src/gcDoCameraLookFromEntityNode.cpp`

```cpp
int gcDoLobbyScoreboardOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);
    ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);

    int sp14;
    int value0 = *(int *)((char *)this + 0x1C);
    int tag0 = value0 & 1;
    int flag0 = 0;
    if (tag0 != 0) {
        flag0 = 1;
    }

    int out0;
    if (flag0 != 0) {
        out0 = 0;
        goto out_done0;
    }
    out0 = value0;
out_done0:
    sp14 = out0;

    int flag0b = 0;
    if (tag0 != 0) {
        flag0b = 1;
    }

    int base0;
    if (flag0b != 0) {
        base0 = value0 & ~1;
    } else {
        base0 = *(int *)value0;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x1C),
                                 (cBase *)base0, *(cBase **)&sp14);

    int sp18;
    int value1 = *(int *)((char *)this + 0x20);
    int tag1 = value1 & 1;
    int new0;
    if (sp14 == 0) {
        new0 = base0 | 1;
    } else {
        new0 = sp14;
    }
    *(int *)((char *)this + 0x1C) = new0;

    int flag1 = 0;
    if (tag1 != 0) {
        flag1 = 1;
    }

    int out1;
    if (flag1 != 0) {
        out1 = 0;
        goto out_done1;
    }
    out1 = value1;
out_done1:
    sp18 = out1;

    int flag1b = 0;
    if (tag1 != 0) {
        flag1b = 1;
    }

    int base1;
    if (flag1b != 0) {
        base1 = value1 & ~1;
    } else {
        base1 = *(int *)value1;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x20),
                                 (cBase *)base1, *(cBase **)&sp18);

    int new1;
    if (sp18 != 0) {
        new1 = sp18;
    } else {
        new1 = base1 | 1;
    }
    int version = rb[3];
    *(int *)((char *)this + 0x20) = new1;

    if ((unsigned int)version >= 2) {
        ((gcDesiredValue *)((char *)this + 0x24))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x28))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 3: `gcDoCameraLookFromEntityNode::Read(cFile &, cMemPool *)`

- Address: `0x00296510`
- Size: `544` bytes
- Source: `src/gcDoCameraLookFromEntityNode.cpp`

```cpp
int gcDoCameraLookFromEntityNode::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char outerFlag;
    int inner[5];
    char innerFlag;
    int ok;

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] >= 4 || (unsigned int)rb[3] < 1) {
        goto fail_outer;
    }

    __0oKcReadBlockctR6FcFileUib(inner, file, 1, true);
    if (inner[3] != 1) {
        goto fail_inner;
    }
    if (((gcAction *)this)->Read(file, pool) != 0) {
        goto read_inner;
    }

fail_inner:
    cFile_SetCurrentPos(*(void **)&inner[0], inner[1]);
    __0oKcReadBlockdtv(inner, 2);
    ok = 0;
    goto after_inner;

read_inner:
    {
        char *typeInfo0 = *(char **)((char *)this + 0x10);
        char *base0 = (char *)this + 0x0C;
        ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
        short off0 = rec0->offset;
        cFile *f0 = *(cFile **)&inner[0];
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    }

    ((gcDesiredValue *)((char *)this + 0x44))->Read(*(cReadBlock *)inner);

    cFileSystem::Read(*(cFileHandle **)inner[0], &innerFlag, 1);
    *(unsigned char *)((char *)this + 0x48) = innerFlag != 0;

    __0oKcReadBlockdtv(inner, 2);
    ok = 1;

after_inner:
    if (ok != 0) {
        goto read_outer;
    }

fail_outer:
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;

read_outer:
    {
        char *typeInfo1 = *(char **)((char *)this + 0x50);
        char *base1 = (char *)this + 0x4C;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    ((cName *)((char *)this + 0x78))->Read(*(cReadBlock *)rb);
    int version = rb[3];
    if ((unsigned int)version >= 2) {
        cFileSystem::Read(*(cFileHandle **)rb[0], &outerFlag, 1);
        int outerTmp = outerFlag;
        __asm__ volatile("" ::: "memory");
        version = rb[3];
        *(unsigned char *)((char *)this + 0x94) = outerTmp != 0;
    }
    if ((unsigned int)version >= 3) {
        ((gcDesiredValue *)((char *)this + 0x90))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 4: `gcDoQueryNavMesh::Read(cFile &, cMemPool *)`

- Address: `0x002f6aa4`
- Size: `512` bytes
- Source: `src/gcDoQueryNavMesh.cpp`

```cpp
int gcDoQueryNavMesh::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value0 = *(int *)((char *)this + 0x0C);
    int tag0 = value0 & 1;
    int flag0 = 0;
    if (tag0 != 0) {
        flag0 = 1;
    }

    int out0;
    if (flag0 != 0) {
        out0 = 0;
        goto out_done0;
    }
    out0 = value0;
out_done0:
    sp14 = out0;

    int flag0b = 0;
    if (tag0 != 0) {
        flag0b = 1;
    }

    int base0;
    if (flag0b != 0) {
        base0 = value0 & ~1;
    } else {
        base0 = *(int *)value0;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x0C),
                                 (cBase *)base0, *(cBase **)&sp14);

    int sp18;
    int value1 = *(int *)((char *)this + 0x10);
    int tag1 = value1 & 1;
    int new0;
    if (sp14 == 0) {
        new0 = base0 | 1;
    } else {
        new0 = sp14;
    }
    *(int *)((char *)this + 0x0C) = new0;

    int flag1 = 0;
    if (tag1 != 0) {
        flag1 = 1;
    }

    int out1;
    if (flag1 != 0) {
        out1 = 0;
        goto out_done1;
    }
    out1 = value1;
out_done1:
    sp18 = out1;

    int flag1b = 0;
    if (tag1 != 0) {
        flag1b = 1;
    }

    int base1;
    if (flag1b != 0) {
        base1 = value1 & ~1;
    } else {
        base1 = *(int *)value1;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x10),
                                 (cBase *)base1, *(cBase **)&sp18);

    int new1;
    if (sp18 == 0) {
        new1 = base1 | 1;
    } else {
        new1 = sp18;
    }
    *(int *)((char *)this + 0x10) = new1;

    ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    ((cName *)((char *)this + 0x1C))->Read(*(cReadBlock *)rb);
    ((cName *)((char *)this + 0x34))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x4C, 4);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```


## Prior Failure Notes

### Note 1

- session `5901eb0b`; src `src/gcValUITextParam.cpp`; snapshot `logs/failure_snapshots/match_20260508_185952/5901eb0b/002b4b38__agent_self_reported_failure__src_gcValUITextParam.cpp`

Implemented gcDoEntityMoveToPoint::Read as a canonical C++ method with explicit ReadRec dispatches for offsets 0x0C, 0x38, 0x4C, the six initial gcDesiredValue reads, scalar reads at 0xD8/0xE4, cName read at 0xE8, bool read at 0x104, version>=6 extra reads, old-version 0xE4 flag compatibility, and version>=7 final ReadRec dispatch. Restructuring the failure label to sit between the version>=6 block and old-version block reduced the mismatch to 6/712 bytes; all calls and overall size are correct.

The remaining mismatch is local to the one-byte bool read/version-load sequence around offsets 0x194-0x19c: expected loads the signed byte from sp+0x14, then loads rb[3], then performs `sltu` and stores the bool; the compiler prefers loading rb[3] before the byte load in the closest stable source. Variants with a separate bool temporary or register bindings made the layout much worse. A targeted permuter run for 120s generated 3744 candidates, compiled 1919, and found no improvement over the 6-byte baseline.


## Disassembly

```asm
2b4b38:	27bdffd0 	addiu	sp,sp,-48
2b4b3c:	afb00018 	sw	s0,24(sp)
2b4b40:	afb20020 	sw	s2,32(sp)
2b4b44:	00c09025 	move	s2,a2
2b4b48:	00808025 	move	s0,a0
2b4b4c:	afb1001c 	sw	s1,28(sp)
2b4b50:	afb30024 	sw	s3,36(sp)
2b4b54:	00a09825 	move	s3,a1
2b4b58:	34110001 	li	s1,0x1
2b4b5c:	03a02025 	move	a0,sp
2b4b60:	34060007 	li	a2,0x7
2b4b64:	afb40028 	sw	s4,40(sp)
2b4b68:	afbf002c 	sw	ra,44(sp)
2b4b6c:	0c001740 	jal	0x5d00
2b4b70:	34070001 	li	a3,0x1
2b4b74:	8fa4000c 	lw	a0,12(sp)
2b4b78:	2c850008 	sltiu	a1,a0,8
2b4b7c:	10a00073 	beqz	a1,0x2b4d4c
2b4b80:	2c840005 	sltiu	a0,a0,5
2b4b84:	14800071 	bnez	a0,0x2b4d4c
2b4b88:	02002025 	move	a0,s0
2b4b8c:	02602825 	move	a1,s3
2b4b90:	0c04bcf1 	jal	0x12f3c4
2b4b94:	02403025 	move	a2,s2
2b4b98:	5040006d 	beqzl	v0,0x2b4d50
2b4b9c:	8fa40000 	lw	a0,0(sp)
2b4ba0:	8e050010 	lw	a1,16(s0)
2b4ba4:	2604000c 	addiu	a0,s0,12
2b4ba8:	24b20030 	addiu	s2,a1,48
2b4bac:	86450000 	lh	a1,0(s2)
2b4bb0:	8fb30000 	lw	s3,0(sp)
2b4bb4:	0c0011e8 	jal	0x47a0
2b4bb8:	0085a021 	addu	s4,a0,a1
2b4bbc:	8e470004 	lw	a3,4(s2)
2b4bc0:	02802025 	move	a0,s4
2b4bc4:	02602825 	move	a1,s3
2b4bc8:	00e0f809 	jalr	a3
2b4bcc:	00403025 	move	a2,v0
2b4bd0:	8e05003c 	lw	a1,60(s0)
2b4bd4:	26040038 	addiu	a0,s0,56
2b4bd8:	24b20030 	addiu	s2,a1,48
2b4bdc:	86450000 	lh	a1,0(s2)
2b4be0:	8fb30000 	lw	s3,0(sp)
2b4be4:	0c0011e8 	jal	0x47a0
2b4be8:	0085a021 	addu	s4,a0,a1
2b4bec:	8e470004 	lw	a3,4(s2)
2b4bf0:	02802025 	move	a0,s4
2b4bf4:	02602825 	move	a1,s3
2b4bf8:	00e0f809 	jalr	a3
2b4bfc:	00403025 	move	a2,v0
2b4c00:	8e050050 	lw	a1,80(s0)
2b4c04:	2604004c 	addiu	a0,s0,76
2b4c08:	24b20030 	addiu	s2,a1,48
2b4c0c:	86450000 	lh	a1,0(s2)
2b4c10:	8fb30000 	lw	s3,0(sp)
2b4c14:	0c0011e8 	jal	0x47a0
2b4c18:	0085a021 	addu	s4,a0,a1
2b4c1c:	8e470004 	lw	a3,4(s2)
2b4c20:	02802025 	move	a0,s4
2b4c24:	02602825 	move	a1,s3
2b4c28:	00e0f809 	jalr	a3
2b4c2c:	00403025 	move	a2,v0
2b4c30:	260400b0 	addiu	a0,s0,176
2b4c34:	0c038a4b 	jal	0xe292c
2b4c38:	03a02825 	move	a1,sp
2b4c3c:	260400b4 	addiu	a0,s0,180
2b4c40:	0c038a4b 	jal	0xe292c
2b4c44:	03a02825 	move	a1,sp
2b4c48:	260400b8 	addiu	a0,s0,184
2b4c4c:	0c038a4b 	jal	0xe292c
2b4c50:	03a02825 	move	a1,sp
2b4c54:	260400c0 	addiu	a0,s0,192
2b4c58:	0c038a4b 	jal	0xe292c
2b4c5c:	03a02825 	move	a1,sp
2b4c60:	260400c4 	addiu	a0,s0,196
2b4c64:	0c038a4b 	jal	0xe292c
2b4c68:	03a02825 	move	a1,sp
2b4c6c:	260400c8 	addiu	a0,s0,200
2b4c70:	0c038a4b 	jal	0xe292c
2b4c74:	03a02825 	move	a1,sp
2b4c78:	8fa40000 	lw	a0,0(sp)
2b4c7c:	260500d8 	addiu	a1,s0,216
2b4c80:	8c840000 	lw	a0,0(a0)
2b4c84:	0c0013fd 	jal	0x4ff4
2b4c88:	34060004 	li	a2,0x4
2b4c8c:	8fa40000 	lw	a0,0(sp)
2b4c90:	260500e4 	addiu	a1,s0,228
2b4c94:	8c840000 	lw	a0,0(a0)
2b4c98:	0c0013fd 	jal	0x4ff4
2b4c9c:	34060004 	li	a2,0x4
2b4ca0:	260400e8 	addiu	a0,s0,232
2b4ca4:	0c00231e 	jal	0x8c78
2b4ca8:	03a02825 	move	a1,sp
2b4cac:	26040100 	addiu	a0,s0,256
2b4cb0:	0c038a4b 	jal	0xe292c
2b4cb4:	03a02825 	move	a1,sp
2b4cb8:	8fa40000 	lw	a0,0(sp)
2b4cbc:	27a50014 	addiu	a1,sp,20
2b4cc0:	8c840000 	lw	a0,0(a0)
2b4cc4:	0c0013fd 	jal	0x4ff4
2b4cc8:	34060001 	li	a2,0x1
2b4ccc:	83a50014 	lb	a1,20(sp)
2b4cd0:	8fa4000c 	lw	a0,12(sp)
2b4cd4:	0005282b 	sltu	a1,zero,a1
2b4cd8:	a2050104 	sb	a1,260(s0)
2b4cdc:	2c850006 	sltiu	a1,a0,6
2b4ce0:	54a00029 	bnezl	a1,0x2b4d88
2b4ce4:	8e0500e4 	lw	a1,228(s0)
2b4ce8:	8fa40000 	lw	a0,0(sp)
2b4cec:	260500dc 	addiu	a1,s0,220
2b4cf0:	8c840000 	lw	a0,0(a0)
2b4cf4:	0c0013fd 	jal	0x4ff4
2b4cf8:	34060004 	li	a2,0x4
2b4cfc:	8fa40000 	lw	a0,0(sp)
2b4d00:	260500e0 	addiu	a1,s0,224
2b4d04:	8c840000 	lw	a0,0(a0)
2b4d08:	0c0013fd 	jal	0x4ff4
2b4d0c:	34060004 	li	a2,0x4
2b4d10:	260400bc 	addiu	a0,s0,188
2b4d14:	0c038a4b 	jal	0xe292c
2b4d18:	03a02825 	move	a1,sp
2b4d1c:	260400cc 	addiu	a0,s0,204
2b4d20:	0c038a4b 	jal	0xe292c
2b4d24:	03a02825 	move	a1,sp
2b4d28:	260400d0 	addiu	a0,s0,208
2b4d2c:	0c038a4b 	jal	0xe292c
2b4d30:	03a02825 	move	a1,sp
2b4d34:	260400d4 	addiu	a0,s0,212
2b4d38:	0c038a4b 	jal	0xe292c
2b4d3c:	03a02825 	move	a1,sp
2b4d40:	8fa4000c 	lw	a0,12(sp)
2b4d44:	10000014 	b	0x2b4d98
2b4d48:	2c840007 	sltiu	a0,a0,7
2b4d4c:	8fa40000 	lw	a0,0(sp)
2b4d50:	0c0014f2 	jal	0x53c8
2b4d54:	8fa50004 	lw	a1,4(sp)
2b4d58:	03a02025 	move	a0,sp
2b4d5c:	0c00176f 	jal	0x5dbc
2b4d60:	34050002 	li	a1,0x2
2b4d64:	00001025 	move	v0,zero
2b4d68:	8fb00018 	lw	s0,24(sp)
2b4d6c:	8fb1001c 	lw	s1,28(sp)
2b4d70:	8fb20020 	lw	s2,32(sp)
2b4d74:	8fb30024 	lw	s3,36(sp)
2b4d78:	8fb40028 	lw	s4,40(sp)
2b4d7c:	8fbf002c 	lw	ra,44(sp)
2b4d80:	03e00008 	jr	ra
2b4d84:	27bd0030 	addiu	sp,sp,48
2b4d88:	30a50004 	andi	a1,a1,0x4
2b4d8c:	10a00002 	beqz	a1,0x2b4d98
2b4d90:	2c840007 	sltiu	a0,a0,7
2b4d94:	ae0000dc 	sw	zero,220(s0)
2b4d98:	1480000e 	bnez	a0,0x2b4dd4
2b4d9c:	03a02025 	move	a0,sp
2b4da0:	8e05007c 	lw	a1,124(s0)
2b4da4:	26040078 	addiu	a0,s0,120
2b4da8:	24b00030 	addiu	s0,a1,48
2b4dac:	86050000 	lh	a1,0(s0)
2b4db0:	8fb20000 	lw	s2,0(sp)
2b4db4:	0c0011e8 	jal	0x47a0
2b4db8:	00859821 	addu	s3,a0,a1
2b4dbc:	8e070004 	lw	a3,4(s0)
2b4dc0:	02602025 	move	a0,s3
2b4dc4:	02402825 	move	a1,s2
2b4dc8:	00e0f809 	jalr	a3
2b4dcc:	00403025 	move	a2,v0
2b4dd0:	03a02025 	move	a0,sp
2b4dd4:	0c00176f 	jal	0x5dbc
2b4dd8:	34050002 	li	a1,0x2
2b4ddc:	02201025 	move	v0,s1
2b4de0:	8fb00018 	lw	s0,24(sp)
2b4de4:	8fb1001c 	lw	s1,28(sp)
2b4de8:	8fb20020 	lw	s2,32(sp)
2b4dec:	8fb30024 	lw	s3,36(sp)
2b4df0:	8fb40028 	lw	s4,40(sp)
2b4df4:	8fbf002c 	lw	ra,44(sp)
2b4df8:	03e00008 	jr	ra
2b4dfc:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: gcDoEntityMoveToPoint::Read(cFile &, cMemPool *)
// Address: 0x002b4b38, Size: 712B
// Obj: gcAll_psp.obj

? cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(s32, s8 *, ?); /* extern */
? cFile__SetCurrentPos_unsignedint(s32, s32);       /* extern */
s32 cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s16); /* extern */
? cName__Read_cReadBlockref(void *, void *);        /* extern */
? cReadBlock___dtor_cReadBlock_void(void *, ?);     /* extern */
? cReadBlock__cReadBlock_cFileref_unsignedint_bool(void *, ?, ?); /* extern */
s32 gcAction__Read_cFileref_cMemPoolptr(void *, ?, ?); /* extern */
? gcDesiredValue__Read_cReadBlockref(void *, void *); /* extern */

s32 gcDoEntityMoveToPoint__Read_cFileref_cMemPoolptr(void *arg0, ? arg1, ? arg2) {
    s8 sp14;
    ? (*temp_a3)(void *, s32, s32, ?);
    ? (*temp_a3_2)(void *, s32, s32, ?);
    ? (*temp_a3_3)(void *, s32, s32, ?);
    ? (*temp_a3_4)(void *, s32, s32, ?);
    s16 temp_a1_2;
    s16 temp_a1_4;
    s16 temp_a1_6;
    s16 temp_a1_8;
    s32 var_a0;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a0_4;
    void *temp_a1;
    void *temp_a1_3;
    void *temp_a1_5;
    void *temp_a1_7;

    cReadBlock__cReadBlock_cFileref_unsignedint_bool(sp, 7, 1);
    if (((u32) subroutine_arg3 < 8U) && ((u32) subroutine_arg3 >= 5U) && (gcAction__Read_cFileref_cMemPoolptr(arg0, arg1, arg2) != 0)) {
        temp_a1 = arg0->unk10;
        temp_a0 = arg0 + 0xC;
        temp_a1_2 = temp_a1->unk30;
        temp_a3 = (temp_a1 + 0x30)->unk4;
        temp_a3(temp_a0 + temp_a1_2, subroutine_arg0, cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_a0, temp_a1_2), temp_a3);
        temp_a1_3 = arg0->unk3C;
        temp_a0_2 = arg0 + 0x38;
        temp_a1_4 = temp_a1_3->unk30;
        temp_a3_2 = (temp_a1_3 + 0x30)->unk4;
        temp_a3_2(temp_a0_2 + temp_a1_4, subroutine_arg0, cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_a0_2, temp_a1_4), temp_a3_2);
        temp_a1_5 = arg0->unk50;
        temp_a0_3 = arg0 + 0x4C;
        temp_a1_6 = temp_a1_5->unk30;
        temp_a3_3 = (temp_a1_5 + 0x30)->unk4;
        temp_a3_3(temp_a0_3 + temp_a1_6, subroutine_arg0, cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_a0_3, temp_a1_6), temp_a3_3);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0xB0, sp);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0xB4, sp);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0xB8, sp);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0xC0, sp);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0xC4, sp);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0xC8, sp);
        cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, arg0 + 0xD8, 4);
        cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, arg0 + 0xE4, 4);
        cName__Read_cReadBlockref(arg0 + 0xE8, sp);
        gcDesiredValue__Read_cReadBlockref(arg0 + 0x100, sp);
        cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, &sp14, 1);
        arg0->unk104 = (s8) (sp14 != 0);
        if ((u32) subroutine_arg3 < 6U) {
            var_a0 = (u32) subroutine_arg3 < 7U;
            if (arg0->unkE4 & 4) {
                arg0->unkDC = 0;
            }
        } else {
            cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, arg0 + 0xDC, 4);
            cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, arg0 + 0xE0, 4);
            gcDesiredValue__Read_cReadBlockref(arg0 + 0xBC, sp);
            gcDesiredValue__Read_cReadBlockref(arg0 + 0xCC, sp);
            gcDesiredValue__Read_cReadBlockref(arg0 + 0xD0, sp);
            gcDesiredValue__Read_cReadBlockref(arg0 + 0xD4, sp);
            var_a0 = (u32) subroutine_arg3 < 7U;
        }
        if (var_a0 == 0) {
            temp_a1_7 = arg0->unk7C;
            temp_a0_4 = arg0 + 0x78;
            temp_a1_8 = temp_a1_7->unk30;
            temp_a3_4 = (temp_a1_7 + 0x30)->unk4;
            temp_a3_4(temp_a0_4 + temp_a1_8, subroutine_arg0, cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_a0_4, temp_a1_8), temp_a3_4);
        }
        cReadBlock___dtor_cReadBlock_void(sp, 2);
        return 1;
    }
    cFile__SetCurrentPos_unsignedint(subroutine_arg0, subroutine_arg1);
    cReadBlock___dtor_cReadBlock_void(sp, 2);
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
