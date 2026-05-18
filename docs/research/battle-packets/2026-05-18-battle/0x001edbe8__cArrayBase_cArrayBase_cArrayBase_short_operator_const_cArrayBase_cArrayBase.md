# Battle Packet: `cArrayBase<cArrayBase> & cArrayBase<short>::operator=(const cArrayBase<cArrayBase> &)`

## Target

- Address: `0x001edbe8`
- Size: `512` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `cArrayBase<cArrayBase> & cArrayBase<short>`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 12B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x001ee65c` `matched` 280B `eAnimation::AssignCopy(const cBase *)`
- `0x001f2528` `matched` 204B `eDynamicModelTemplate::AssignCopy(const cBase *)`
- `0x00271504` `matched` 52B `gcTableColumnShort::AssignCopy(const cBase *)`

## Placement

- Canonical source: `src/cArrayBasecArrayBasecArrayBaseshort.cpp`
- Header(s): `include/cArrayBasecArrayBasecArrayBaseshort.h`
- Allowed source(s): `src/cArrayBasecArrayBasecArrayBaseshort.cpp`
- Split-TU prefix: `src/cArrayBasecArrayBasecArrayBaseshort_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/cArrayBase<cArrayBase> & cArrayBase<short>.h`

Not found.

## Matched Method Exemplars

### Exemplar 1: `eCamera::operator=(const eCamera &)`

- Address: `0x0002dc54`
- Size: `492` bytes
- Source: `src/eCamera.cpp`

```cpp
eCamera &eCamera::operator=(const eCamera &other) {
    int word0 = *(const int *)((const char *)&other + 0x00);
    const int *srcWords = (const int *)((const char *)&other + 0x04);
    int *dstWords = (int *)((char *)this + 0x04);
    *(int *)((char *)this + 0x00) = word0;
    int word1 = srcWords[0];
    int word2 = srcWords[1];
    int word3 = srcWords[2];
    dstWords[0] = word1;
    int word4 = srcWords[3];
    dstWords[1] = word2;
    dstWords[2] = word3;
    dstWords[3] = word4;

    unsigned int i = 0;
    char *dst = (char *)this + 0x20;
    const char *src = (const char *)&other + 0x20;
    do {
        float w = ((const mPlane *)src)->w;
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        ((mPlane *)dst)->w = w;
        dst += 0x10;
        src += 0x10;
    } while (i < 6);

    i = 0;
    dst = (char *)this + 0x80;
    src = (const char *)&other + 0x80;
    do {
        float w = ((const mPlane *)src)->w;
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        ((mPlane *)dst)->w = w;
        dst += 0x10;
        src += 0x10;
    } while (i < 6);

    *(v4sf_t *)((char *)this + 0x110) =
        *(const v4sf_t *)((const char *)&other + 0x110);
    i = 0;
    *(v4sf_t *)((char *)this + 0x0E0) =
        *(const v4sf_t *)((const char *)&other + 0x0E0);
    dst = (char *)this + 0x120;
    *(v4sf_t *)((char *)this + 0x0F0) =
        *(const v4sf_t *)((const char *)&other + 0x0F0);
    src = (const char *)&other + 0x120;
    *(v4sf_t *)((char *)this + 0x100) =
        *(const v4sf_t *)((const char *)&other + 0x100);
    do {
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        dst += 0x10;
        src += 0x10;
    } while (i < 4);

    i = 0;
    dst = (char *)this + 0x160;
    src = (const char *)&other + 0x160;
    do {
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        dst += 0x10;
        src += 0x10;
    } while (i < 4);

    i = 0;
    dst = (char *)this + 0x1A0;
    src = (const char *)&other + 0x1A0;
    do {
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        __asm__ volatile("" ::: "memory");
        dst += 0x10;
        src += 0x10;
    } while (i < 4);

    const int *src1E8 = (const int *)((const char *)&other + 0x1E8);
    float value1E0 = *(const float *)((const char *)&other + 0x1E0);
    *(float *)((char *)this + 0x1E0) = value1E0;
    float value1E4 = *(const float *)((const char *)&other + 0x1E4);
    int *dst1E8 = (int *)((char *)this + 0x1E8);
    *(float *)((char *)this + 0x1E4) = value1E4;
    *dst1E8 = *src1E8;
    *(float *)((char *)this + 0x1EC) =
        *(const float *)((const char *)&other + 0x1EC);
    *(float *)((char *)this + 0x1F0) =
        *(const float *)((const char *)&other + 0x1F0);
    *(float *)((char *)this + 0x1F4) =
        *(const float *)((const char *)&other + 0x1F4);
    __asm__ volatile("" ::: "memory");
    *(v4sf_t *)((char *)this + 0x200) =
        *(const v4sf_t *)((const char *)&other + 0x200);
    *(v4sf_t *)((char *)this + 0x210) =
        *(const v4sf_t *)((const char *)&other + 0x210);
    *(v4sf_t *)((char *)this + 0x220) =
        *(const v4sf_t *)((const char *)&other + 0x220);
    float value230 = *(const float *)((const char *)&other + 0x230);
    *(float *)((char *)this + 0x230) = value230;
    const char *src250 = (const char *)&other;
    int value234 = *(const int *)((const char *)&other + 0x234);
    src250 += 0x250;
    *(int *)((char *)this + 0x234) = value234;
    *(v4sf_t *)((char *)this + 0x240) =
        *(const v4sf_t *)((const char *)&other + 0x240);
    int *dst250 = (int *)((char *)this + 0x250);
    int value250 = *(const int *)src250;
    *dst250 = value250;
    int value254 = *(const int *)((const char *)&other + 0x254);
    *(int *)((char *)this + 0x254) = value254;
    int value258 = *(const int *)((const char *)&other + 0x258);
    char *dstBase = (char *)this + 0x25C;
    const char *srcBase = (const char *)&other + 0x25C;
    *(int *)((char *)this + 0x258) = value258;

    int offset = 0;
    int count = 0x16;
    do {
        const int *srcPair = (const int *)((int)srcBase + offset);
        int pair0 = srcPair[0];
        int pair1 = srcPair[1];
        int *dstPair = (int *)(dstBase + offset);
        dstPair[0] = pair0;
        dstPair[1] = pair1;
        count--;
        offset += 8;
    } while (count != 0);

    return *this;
}
```

### Exemplar 2: `eSilhouetteModelMtl::operator=(const eSilhouetteModelMtl &)`

- Address: `0x00087220`
- Size: `148` bytes
- Source: `src/eSilhouetteModelMtl.cpp`

```cpp
eSilhouetteModelMtl::operator=(const eSilhouetteModelMtl &other) {
    unsigned char field5E;
    signed char field5F;
    int *dstWord;
    const int *srcWord;

    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    ((unsigned char *)this)[0x5C] = ((const unsigned char *)&other)[0x5C];
    ((unsigned char *)this)[0x5D] = ((const unsigned char *)&other)[0x5D];
    field5E = ((const unsigned char *)&other)[0x5E];
    ((unsigned char *)this)[0x5E] = field5E;
    field5F = ((const signed char *)&other)[0x5F];
    ((signed char *)this)[0x5F] = field5F;
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((const char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((const char *)&other + 0x64));
    dstWord = (int *)((char *)this + 0x68);
    srcWord = (const int *)((const char *)&other + 0x68);
    __asm__ volatile("" : "+r"(dstWord), "+r"(srcWord));
    *dstWord = *srcWord;
    dstWord = (int *)((char *)this + 0x6C);
    srcWord = (const int *)((const char *)&other + 0x6C);
    __asm__ volatile("" : "+r"(dstWord), "+r"(srcWord));
    *dstWord = *srcWord;
    *(int *)((char *)this + 0x70) = *(const int *)((const char *)&other + 0x70);
    return *this;
}
```

### Exemplar 3: `eTextureMap::operator=(const eTextureMap &)`

- Address: `0x0007b164`
- Size: `124` bytes
- Source: `src/eTextureMap.cpp`

```cpp
eTextureMap &eTextureMap::operator=(const eTextureMap &other) {
    ((cObject *)this)->operator=(*(const cObject *)&other);
    ((signed char *)this)[0x44] = ((const signed char *)&other)[0x44];
    ((signed char *)this)[0x45] = ((const signed char *)&other)[0x45];
    ((signed char *)this)[0x46] = ((const signed char *)&other)[0x46];
    ((unsigned char *)this)[0x47] = ((const unsigned char *)&other)[0x47];
    ((short *)this)[0x48 / 2] = ((const short *)&other)[0x48 / 2];
    ((short *)this)[0x4A / 2] = ((const short *)&other)[0x4A / 2];
    ((unsigned char *)this)[0x4C] = ((const unsigned char *)&other)[0x4C];
    ((unsigned char *)this)[0x4D] = ((const unsigned char *)&other)[0x4D];
    __asm__ volatile("" ::: "memory");
    CreateData();
    return *this;
}
```

### Exemplar 4: `cBaseArray::operator=(const cBaseArray &)`

- Address: `0x00006e7c`
- Size: `228` bytes
- Source: `src/cBaseArray.cpp`

```cpp
cBaseArray &cBaseArray::operator=(const cBaseArray &other) {
    RemoveAll();

    int count = 0;
    if (other.mData != 0) {
        count = other.mData[-1];
    }

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    mData = Allocate(count);

    int index = 0;
    if (index < count) {
        int offset = 0;
        do {
            void *copy = 0;
            void *src = *(void **)((char *)other.mData + offset);
            if (src != 0) {
                char *dispatch = *(char **)((char *)src + 4);
                cBaseArray_CloneEntry *clone =
                    (cBaseArray_CloneEntry *)(dispatch + 0x10);
                copy = clone->func((char *)src + clone->offset, pool, mOwner);
            }
            *(void **)((char *)mData + offset) = copy;
            index += 1;
            offset += 4;
        } while (index < count);
    }

    return *this;
}
```


## Prior Failure Notes

### Note 1

- session `26515966`

Implemented the short instantiation in the same template source with element size 2 and type id 0x36CF0C. It reaches the same best state as the int instantiation: exact 512-byte size and structurally matching delete, allocate, resize-copy, and same-size-copy control flow, but compare_func reports 12/512 bytes different. The remaining short diff is the same register-allocation swap in the same-size copy loop, with lh/sh in the right places but offset/count and destination registers opposite the target.

Tried the same manual variants as the int function: copy expression ordering, explicit base pointers, barriers, register hints, offset update ordering, and sched=1. As with int, the post-allocation liveness barrier fixed the earlier initializer schedule, but the same-size loop stayed at the a2/a3 register swap. This is not a bnel issue: there are no branch-likely vs normal-branch differences in the remaining mismatch, and the diff is register allocation/scheduling only.


## Disassembly

```asm
1edbe8:	27bdffe0 	addiu	sp,sp,-32
1edbec:	afb00000 	sw	s0,0(sp)
1edbf0:	afb10004 	sw	s1,4(sp)
1edbf4:	00a08825 	move	s1,a1
1edbf8:	00808025 	move	s0,a0
1edbfc:	afb20008 	sw	s2,8(sp)
1edc00:	8e240000 	lw	a0,0(s1)
1edc04:	3c124000 	lui	s2,0x4000
1edc08:	34050000 	li	a1,0x0
1edc0c:	2652ffff 	addiu	s2,s2,-1
1edc10:	afb3000c 	sw	s3,12(sp)
1edc14:	afb40010 	sw	s4,16(sp)
1edc18:	afbf0014 	sw	ra,20(sp)
1edc1c:	10800003 	beqz	a0,0x1edc2c
1edc20:	8e060000 	lw	a2,0(s0)
1edc24:	8c85fffc 	lw	a1,-4(a0)
1edc28:	00b22824 	and	a1,a1,s2
1edc2c:	10c00003 	beqz	a2,0x1edc3c
1edc30:	34070000 	li	a3,0x0
1edc34:	8cc7fffc 	lw	a3,-4(a2)
1edc38:	00f23824 	and	a3,a3,s2
1edc3c:	10a7004f 	beq	a1,a3,0x1edd7c
1edc40:	34050000 	li	a1,0x0
1edc44:	34050000 	li	a1,0x0
1edc48:	10c00003 	beqz	a2,0x1edc58
1edc4c:	8e130000 	lw	s3,0(s0)
1edc50:	8cc5fffc 	lw	a1,-4(a2)
1edc54:	00b22824 	and	a1,a1,s2
1edc58:	34060000 	li	a2,0x0
1edc5c:	00c5382a 	slt	a3,a2,a1
1edc60:	10e00005 	beqz	a3,0x1edc78
1edc64:	00000000 	nop
1edc68:	24c60001 	addiu	a2,a2,1
1edc6c:	00c5382a 	slt	a3,a2,a1
1edc70:	14e0fffe 	bnez	a3,0x1edc6c
1edc74:	24c60001 	addiu	a2,a2,1
1edc78:	1260000f 	beqz	s3,0x1edcb8
1edc7c:	2673fffc 	addiu	s3,s3,-4
1edc80:	5260000c 	beqzl	s3,0x1edcb4
1edc84:	ae000000 	sw	zero,0(s0)
1edc88:	0c0011e8 	jal	0x47a0
1edc8c:	02602025 	move	a0,s3
1edc90:	8c440024 	lw	a0,36(v0)
1edc94:	02602825 	move	a1,s3
1edc98:	8c86001c 	lw	a2,28(a0)
1edc9c:	24c60030 	addiu	a2,a2,48
1edca0:	84c70000 	lh	a3,0(a2)
1edca4:	8cc60004 	lw	a2,4(a2)
1edca8:	00c0f809 	jalr	a2
1edcac:	00872021 	addu	a0,a0,a3
1edcb0:	ae000000 	sw	zero,0(s0)
1edcb4:	8e240000 	lw	a0,0(s1)
1edcb8:	10800003 	beqz	a0,0x1edcc8
1edcbc:	34130000 	li	s3,0x0
1edcc0:	8c93fffc 	lw	s3,-4(a0)
1edcc4:	02729824 	and	s3,s3,s2
1edcc8:	0260a025 	move	s4,s3
1edccc:	1e800003 	bgtz	s4,0x1edcdc
1edcd0:	00000000 	nop
1edcd4:	10000012 	b	0x1edd20
1edcd8:	34130000 	li	s3,0x0
1edcdc:	0c0011e8 	jal	0x47a0
1edce0:	02002025 	move	a0,s0
1edce4:	8c440024 	lw	a0,36(v0)
1edce8:	02732821 	addu	a1,s3,s3
1edcec:	8c86001c 	lw	a2,28(a0)
1edcf0:	24a50004 	addiu	a1,a1,4
1edcf4:	24c60028 	addiu	a2,a2,40
1edcf8:	84c70000 	lh	a3,0(a2)
1edcfc:	8cc90004 	lw	t1,4(a2)
1edd00:	00872021 	addu	a0,a0,a3
1edd04:	3c070037 	lui	a3,0x37
1edd08:	34060002 	li	a2,0x2
1edd0c:	24e7cf0c 	addiu	a3,a3,-12532
1edd10:	0120f809 	jalr	t1
1edd14:	34080112 	li	t0,0x112
1edd18:	ac540000 	sw	s4,0(v0)
1edd1c:	24530004 	addiu	s3,v0,4
1edd20:	ae130000 	sw	s3,0(s0)
1edd24:	34040000 	li	a0,0x0
1edd28:	8e330000 	lw	s3,0(s1)
1edd2c:	34050000 	li	a1,0x0
1edd30:	12600003 	beqz	s3,0x1edd40
1edd34:	34060000 	li	a2,0x0
1edd38:	8e66fffc 	lw	a2,-4(s3)
1edd3c:	00d23024 	and	a2,a2,s2
1edd40:	0086302a 	slt	a2,a0,a2
1edd44:	10c0001f 	beqz	a2,0x1eddc4
1edd48:	00000000 	nop
1edd4c:	8e060000 	lw	a2,0(s0)
1edd50:	00c53021 	addu	a2,a2,a1
1edd54:	50c00007 	beqzl	a2,0x1edd74
1edd58:	24840001 	addiu	a0,a0,1
1edd5c:	8e270000 	lw	a3,0(s1)
1edd60:	00e53821 	addu	a3,a3,a1
1edd64:	84e70000 	lh	a3,0(a3)
1edd68:	a4c70000 	sh	a3,0(a2)
1edd6c:	8e330000 	lw	s3,0(s1)
1edd70:	24840001 	addiu	a0,a0,1
1edd74:	1000ffee 	b	0x1edd30
1edd78:	24a50002 	addiu	a1,a1,2
1edd7c:	34070000 	li	a3,0x0
1edd80:	10800003 	beqz	a0,0x1edd90
1edd84:	34060000 	li	a2,0x0
1edd88:	8c86fffc 	lw	a2,-4(a0)
1edd8c:	00d23024 	and	a2,a2,s2
1edd90:	00a6202a 	slt	a0,a1,a2
1edd94:	1080000b 	beqz	a0,0x1eddc4
1edd98:	00000000 	nop
1edd9c:	8e240000 	lw	a0,0(s1)
1edda0:	8e060000 	lw	a2,0(s0)
1edda4:	00872021 	addu	a0,a0,a3
1edda8:	84840000 	lh	a0,0(a0)
1eddac:	00c73021 	addu	a2,a2,a3
1eddb0:	a4c40000 	sh	a0,0(a2)
1eddb4:	24a50001 	addiu	a1,a1,1
1eddb8:	24e70002 	addiu	a3,a3,2
1eddbc:	1000fff0 	b	0x1edd80
1eddc0:	8e240000 	lw	a0,0(s1)
1eddc4:	02001025 	move	v0,s0
1eddc8:	8fb00000 	lw	s0,0(sp)
1eddcc:	8fb10004 	lw	s1,4(sp)
1eddd0:	8fb20008 	lw	s2,8(sp)
1eddd4:	8fb3000c 	lw	s3,12(sp)
1eddd8:	8fb40010 	lw	s4,16(sp)
1edddc:	8fbf0014 	lw	ra,20(sp)
1edde0:	03e00008 	jr	ra
1edde4:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cArrayBase<cArrayBase> & cArrayBase<short>::operator=(const cArrayBase<cArrayBase> &)
// Address: 0x001edbe8, Size: 512B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void **, s32, s32, s32); /* extern */

void **cArrayBase_cArrayBase_refcArrayBase_short___operator_eq_constcArrayBase_cArrayBase_ref(void **arg0, void **arg1) {
    ? (*temp_a2_3)(void *, void **, ?, s16);
    s16 *temp_a2_5;
    s16 temp_a3;
    s32 *temp_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a1_4;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_a3_3;
    s32 var_s3;
    void **temp_s3_2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a2;
    void *temp_a2_2;
    void *temp_a2_4;
    void *temp_s3;
    void *var_s3_2;

    temp_a0 = *arg1;
    var_a1 = 0;
    temp_a2 = *arg0;
    if (temp_a0 != NULL) {
        var_a1 = temp_a0->unk-4 & 0x3FFFFFFF;
    }
    var_a3 = 0;
    if (temp_a2 != NULL) {
        var_a3 = temp_a2->unk-4 & 0x3FFFFFFF;
    }
    var_a1_2 = 0;
    if (var_a1 != var_a3) {
        var_a1_3 = 0;
        temp_s3 = *arg0;
        if (temp_a2 != NULL) {
            var_a1_3 = temp_a2->unk-4 & 0x3FFFFFFF;
        }
        var_a2 = 0;
        var_a3_2 = var_a1_3 > 0;
        if (var_a3_2 != 0) {
            var_a2 = 1;
            do {
                var_a3_2 = var_a2 < var_a1_3;
                var_a2 += 1;
            } while (var_a3_2 != 0);
        }
        temp_s3_2 = temp_s3 - 4;
        if (temp_s3 != NULL) {
            if (temp_s3_2 != NULL) {
                temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s3_2, var_a1_3, var_a2, var_a3_2)->unk24;
                temp_a2_2 = temp_a0_2->unk1C;
                temp_a3 = temp_a2_2->unk30;
                temp_a2_3 = (temp_a2_2 + 0x30)->unk4;
                temp_a2_3(temp_a0_2 + temp_a3, temp_s3_2, temp_a2_3, temp_a3);
            }
            *arg0 = NULL;
        }
        var_s3 = 0;
        if (*arg1 != NULL) {
            var_s3 = (*arg1)->unk-4 & 0x3FFFFFFF;
        }
        if (var_s3 <= 0) {
            var_s3_2 = NULL;
        } else {
            temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a2_4 = temp_a0_3->unk1C;
            temp_v0 = (temp_a2_4 + 0x28)->unk4(temp_a0_3 + temp_a2_4->unk28, (var_s3 * 2) + 4, 2, 0x36CF0C);
            *temp_v0 = var_s3;
            var_s3_2 = temp_v0 + 4;
        }
        *arg0 = var_s3_2;
        var_a0 = 0;
        var_a1_4 = 0;
loop_20:
        var_a2_2 = 0;
        if (*arg1 != NULL) {
            var_a2_2 = (*arg1)->unk-4 & 0x3FFFFFFF;
        }
        if (var_a0 < var_a2_2) {
            temp_a2_5 = *arg0 + var_a1_4;
            if (temp_a2_5 != NULL) {
                *temp_a2_5 = *(*arg1 + var_a1_4);
            }
            var_a0 += 1;
            var_a1_4 += 2;
            goto loop_20;
        }
    } else {
        var_a3_3 = 0;
loop_27:
        var_a2_3 = 0;
        if (*arg1 != NULL) {
            var_a2_3 = (*arg1)->unk-4 & 0x3FFFFFFF;
        }
        if (var_a1_2 < var_a2_3) {
            *(*arg0 + var_a3_3) = *(*arg1 + var_a3_3);
            var_a1_2 += 1;
            var_a3_3 += 2;
            goto loop_27;
        }
    }
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
