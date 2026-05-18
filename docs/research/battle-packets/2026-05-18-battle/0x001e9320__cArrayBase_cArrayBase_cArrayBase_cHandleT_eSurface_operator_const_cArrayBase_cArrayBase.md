# Battle Packet: `cArrayBase<cArrayBase> & cArrayBase<cHandleT<eSurface> >::operator=(const cArrayBase<cArrayBase> &)`

## Target

- Address: `0x001e9320`
- Size: `512` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `cArrayBase<cArrayBase> & cArrayBase<cHandleT<eSurface> >`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 16B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `16`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x001e9920` `failed` 720B `eRoom::AssignCopy(const cBase *)`
- `0x001f65e8` `matched` 76B `eSurfaceSet::AssignCopy(const cBase *)`

## Placement

- Canonical source: `src/cArrayBasecArrayBasecArrayBasecHandleTeSurface.cpp`
- Header(s): `include/cArrayBasecArrayBasecArrayBasecHandleTeSurface.h`
- Allowed source(s): `src/cArrayBasecArrayBasecArrayBasecHandleTeSurface.cpp`
- Split-TU prefix: `src/cArrayBasecArrayBasecArrayBasecHandleTeSurface_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/cArrayBase<cArrayBase> & cArrayBase<cHandleT<eSurface> >.h`

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

- session `9fd4de0d`

Added a real C++ cArrayBase<cHandleT<eSurface> >::operator= template instantiation in the requested source file and iterated against compare_func. The best source shape is the structured eAll cArrayBase template form with the same 512-byte size; compare_func reports a 16/512 byte mismatch. The remaining diff is localized to the same-size copy loop: the compiled code keeps the byte offset in a2 and the reloaded count in a3, while the target keeps the offset in a3 and the count in a2, with a small load-order/register allocation difference in the copy body. I tried typed helper vs void** template forms, register hints for offset/count/index, empty asm liveness barriers, removing the srcArray barrier, direct data[index] copy, and manual inlined count calculation. The direct/manual variants made the function 508B or increased the diff substantially; removing the srcArray barrier was the best improvement (24B -> 16B).

I also attempted the project permuter for 300s with --save-best, but tools/permuter.py failed immediately while extracting .text from its temporary object, before doing a search. This looks like a tooling failure, and I did not modify tools per repo rules. The likely root cause is SNC register allocation around the same-size loop rather than control-flow or flags; sched=1 worsened this function.


## Disassembly

```asm
1e9320:	27bdffe0 	addiu	sp,sp,-32
1e9324:	afb00000 	sw	s0,0(sp)
1e9328:	afb10004 	sw	s1,4(sp)
1e932c:	00a08825 	move	s1,a1
1e9330:	00808025 	move	s0,a0
1e9334:	afb20008 	sw	s2,8(sp)
1e9338:	8e240000 	lw	a0,0(s1)
1e933c:	3c124000 	lui	s2,0x4000
1e9340:	34050000 	li	a1,0x0
1e9344:	2652ffff 	addiu	s2,s2,-1
1e9348:	afb3000c 	sw	s3,12(sp)
1e934c:	afb40010 	sw	s4,16(sp)
1e9350:	afbf0014 	sw	ra,20(sp)
1e9354:	10800003 	beqz	a0,0x1e9364
1e9358:	8e060000 	lw	a2,0(s0)
1e935c:	8c85fffc 	lw	a1,-4(a0)
1e9360:	00b22824 	and	a1,a1,s2
1e9364:	10c00003 	beqz	a2,0x1e9374
1e9368:	34070000 	li	a3,0x0
1e936c:	8cc7fffc 	lw	a3,-4(a2)
1e9370:	00f23824 	and	a3,a3,s2
1e9374:	10a7004f 	beq	a1,a3,0x1e94b4
1e9378:	34050000 	li	a1,0x0
1e937c:	34050000 	li	a1,0x0
1e9380:	10c00003 	beqz	a2,0x1e9390
1e9384:	8e130000 	lw	s3,0(s0)
1e9388:	8cc5fffc 	lw	a1,-4(a2)
1e938c:	00b22824 	and	a1,a1,s2
1e9390:	34060000 	li	a2,0x0
1e9394:	00c5382a 	slt	a3,a2,a1
1e9398:	10e00005 	beqz	a3,0x1e93b0
1e939c:	00000000 	nop
1e93a0:	24c60001 	addiu	a2,a2,1
1e93a4:	00c5382a 	slt	a3,a2,a1
1e93a8:	14e0fffe 	bnez	a3,0x1e93a4
1e93ac:	24c60001 	addiu	a2,a2,1
1e93b0:	1260000f 	beqz	s3,0x1e93f0
1e93b4:	2673fffc 	addiu	s3,s3,-4
1e93b8:	5260000c 	beqzl	s3,0x1e93ec
1e93bc:	ae000000 	sw	zero,0(s0)
1e93c0:	0c0011e8 	jal	0x47a0
1e93c4:	02602025 	move	a0,s3
1e93c8:	8c440024 	lw	a0,36(v0)
1e93cc:	02602825 	move	a1,s3
1e93d0:	8c86001c 	lw	a2,28(a0)
1e93d4:	24c60030 	addiu	a2,a2,48
1e93d8:	84c70000 	lh	a3,0(a2)
1e93dc:	8cc60004 	lw	a2,4(a2)
1e93e0:	00c0f809 	jalr	a2
1e93e4:	00872021 	addu	a0,a0,a3
1e93e8:	ae000000 	sw	zero,0(s0)
1e93ec:	8e240000 	lw	a0,0(s1)
1e93f0:	10800003 	beqz	a0,0x1e9400
1e93f4:	34130000 	li	s3,0x0
1e93f8:	8c93fffc 	lw	s3,-4(a0)
1e93fc:	02729824 	and	s3,s3,s2
1e9400:	0260a025 	move	s4,s3
1e9404:	1e800003 	bgtz	s4,0x1e9414
1e9408:	00000000 	nop
1e940c:	10000012 	b	0x1e9458
1e9410:	34130000 	li	s3,0x0
1e9414:	0c0011e8 	jal	0x47a0
1e9418:	02002025 	move	a0,s0
1e941c:	8c440024 	lw	a0,36(v0)
1e9420:	00132880 	sll	a1,s3,0x2
1e9424:	8c86001c 	lw	a2,28(a0)
1e9428:	24a50004 	addiu	a1,a1,4
1e942c:	24c60028 	addiu	a2,a2,40
1e9430:	84c70000 	lh	a3,0(a2)
1e9434:	8cc90004 	lw	t1,4(a2)
1e9438:	00872021 	addu	a0,a0,a3
1e943c:	3c070037 	lui	a3,0x37
1e9440:	34060004 	li	a2,0x4
1e9444:	24e7cf0c 	addiu	a3,a3,-12532
1e9448:	0120f809 	jalr	t1
1e944c:	34080112 	li	t0,0x112
1e9450:	ac540000 	sw	s4,0(v0)
1e9454:	24530004 	addiu	s3,v0,4
1e9458:	ae130000 	sw	s3,0(s0)
1e945c:	34040000 	li	a0,0x0
1e9460:	8e330000 	lw	s3,0(s1)
1e9464:	34050000 	li	a1,0x0
1e9468:	12600003 	beqz	s3,0x1e9478
1e946c:	34060000 	li	a2,0x0
1e9470:	8e66fffc 	lw	a2,-4(s3)
1e9474:	00d23024 	and	a2,a2,s2
1e9478:	0086302a 	slt	a2,a0,a2
1e947c:	10c0001f 	beqz	a2,0x1e94fc
1e9480:	00000000 	nop
1e9484:	8e060000 	lw	a2,0(s0)
1e9488:	00c53021 	addu	a2,a2,a1
1e948c:	50c00007 	beqzl	a2,0x1e94ac
1e9490:	24840001 	addiu	a0,a0,1
1e9494:	8e270000 	lw	a3,0(s1)
1e9498:	00e53821 	addu	a3,a3,a1
1e949c:	8ce70000 	lw	a3,0(a3)
1e94a0:	acc70000 	sw	a3,0(a2)
1e94a4:	8e330000 	lw	s3,0(s1)
1e94a8:	24840001 	addiu	a0,a0,1
1e94ac:	1000ffee 	b	0x1e9468
1e94b0:	24a50004 	addiu	a1,a1,4
1e94b4:	34070000 	li	a3,0x0
1e94b8:	10800003 	beqz	a0,0x1e94c8
1e94bc:	34060000 	li	a2,0x0
1e94c0:	8c86fffc 	lw	a2,-4(a0)
1e94c4:	00d23024 	and	a2,a2,s2
1e94c8:	00a6202a 	slt	a0,a1,a2
1e94cc:	1080000b 	beqz	a0,0x1e94fc
1e94d0:	00000000 	nop
1e94d4:	8e240000 	lw	a0,0(s1)
1e94d8:	8e060000 	lw	a2,0(s0)
1e94dc:	00872021 	addu	a0,a0,a3
1e94e0:	8c840000 	lw	a0,0(a0)
1e94e4:	00c73021 	addu	a2,a2,a3
1e94e8:	acc40000 	sw	a0,0(a2)
1e94ec:	24a50001 	addiu	a1,a1,1
1e94f0:	24e70004 	addiu	a3,a3,4
1e94f4:	1000fff0 	b	0x1e94b8
1e94f8:	8e240000 	lw	a0,0(s1)
1e94fc:	02001025 	move	v0,s0
1e9500:	8fb00000 	lw	s0,0(sp)
1e9504:	8fb10004 	lw	s1,4(sp)
1e9508:	8fb20008 	lw	s2,8(sp)
1e950c:	8fb3000c 	lw	s3,12(sp)
1e9510:	8fb40010 	lw	s4,16(sp)
1e9514:	8fbf0014 	lw	ra,20(sp)
1e9518:	03e00008 	jr	ra
1e951c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cArrayBase<cArrayBase> & cArrayBase<cHandleT<eSurface> >::operator=(const cArrayBase<cArrayBase> &)
// Address: 0x001e9320, Size: 512B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void **, s32, s32, s32); /* extern */

void **cArrayBase_cArrayBase_refcArrayBase_cHandleT_eSurface____operator_eq_constcArrayBase_cArrayBase_ref(void **arg0, void **arg1) {
    ? (*temp_a2_3)(void *, void **, ?, s16);
    s16 temp_a3;
    s32 *temp_a2_5;
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
            temp_v0 = (temp_a2_4 + 0x28)->unk4(temp_a0_3 + temp_a2_4->unk28, (var_s3 * 4) + 4, 4, 0x36CF0C);
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
            var_a1_4 += 4;
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
            var_a3_3 += 4;
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
