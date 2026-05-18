# Battle Packet: `cArrayBase<cArrayBase> & cArrayBase<float>::operator=(const cArrayBase<cArrayBase> &)`

## Target

- Address: `0x001e812c`
- Size: `512` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `cArrayBase<cArrayBase> & cArrayBase<float>`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `146`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 6B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `6`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `6`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x00046e3c` `untried` 564B `eDynamicModel::operator=(const eDynamicModel &)`
- `0x00100c2c` `untried` 312B `gcProfile::ResetVariables(cArray<cArray<float> > &, const cBaseArrayT<gcFloatSet *> &) static`
- `0x00171e80` `untried` 4952B `gcEntity::Reset(cMemPool *, bool)`
- `0x001ebec0` `matched` 132B `eStaticModelTemplate::AssignCopy(const cBase *)`
- `0x001f4b70` `matched` 208B `eHeightmapShape::AssignCopy(const cBase *)`
- `0x0024932c` `matched` 68B `gcFloatSet::AssignCopy(const cBase *)`
- `0x0024a358` `untried` 676B `cArrayBase<cArrayBase> & cArrayBase<cArray<float> >::operator=(const cArrayBase<cArrayBase> &)`
- `0x0024a9a8` `untried` 688B `cArrayBase<cArray<float> >::SetSize(int, const cArrayBase &)`
- `0x00263934` `matched` 368B `gcUITextControl::AssignCopy(const cBase *)`
- `0x00271b10` `matched` 52B `gcTableColumnFloat::AssignCopy(const cBase *)`
- `0x0027e6f0` `matched` 108B `gcGenericSubscriber::AssignCopy(const cBase *)`
- `0x0028e1b8` `untried` 928B `gcUIButton::AssignCopy(const cBase *)`
- `0x0028eac4` `untried` 928B `gcUICheckBox::AssignCopy(const cBase *)`
- `0x002901ec` `failed` 964B `gcUIEditBox::AssignCopy(const cBase *)`
- `0x00291378` `failed` 932B `gcUIMarqueeControl::AssignCopy(const cBase *)`

## Placement

- Canonical source: `src/cArrayBasecArrayBasecArrayBasefloat.cpp`
- Header(s): `include/cArrayBasecArrayBasecArrayBasefloat.h`
- Allowed source(s): `src/cArrayBasecArrayBasecArrayBasefloat.cpp`
- Split-TU prefix: `src/cArrayBasecArrayBasecArrayBasefloat_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/cArrayBase<cArrayBase> & cArrayBase<float>.h`

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

- session `1ccbfaa8`

Implemented the matching cArrayBase<float>::operator= instantiation in the same source file. The function is also close: compare_func reports 6/512 bytes different after relocation masking. It has the same post-resize setup swap at offsets 0x140/0x144 as the eStaticModelNodeCullId version, plus a small same-size float store register-allocation difference: expected computes the destination address back into a0 before swc1, while the current source keeps the destination in a2. The control flow, allocation/free calls, size masks, element size, allocation tag, and most copy-loop scheduling match.

Manual attempts included pointer-vs-indexed copies, value temporaries for the float load/store, register-variable hints, memory barriers, explicit-specialization and free-function symbol experiments, and sched=1 compilation for comparison. Most alternatives made the function worse; the current generic template form is the best result found. The permuter could not run because the tool failed on linkonce template sections with 'Failed to extract .text section'.


## Disassembly

```asm
1e812c:	27bdffe0 	addiu	sp,sp,-32
1e8130:	afb00000 	sw	s0,0(sp)
1e8134:	afb10004 	sw	s1,4(sp)
1e8138:	00a08825 	move	s1,a1
1e813c:	00808025 	move	s0,a0
1e8140:	afb20008 	sw	s2,8(sp)
1e8144:	8e240000 	lw	a0,0(s1)
1e8148:	3c124000 	lui	s2,0x4000
1e814c:	34050000 	li	a1,0x0
1e8150:	2652ffff 	addiu	s2,s2,-1
1e8154:	afb3000c 	sw	s3,12(sp)
1e8158:	afb40010 	sw	s4,16(sp)
1e815c:	afbf0014 	sw	ra,20(sp)
1e8160:	10800003 	beqz	a0,0x1e8170
1e8164:	8e060000 	lw	a2,0(s0)
1e8168:	8c85fffc 	lw	a1,-4(a0)
1e816c:	00b22824 	and	a1,a1,s2
1e8170:	10c00003 	beqz	a2,0x1e8180
1e8174:	34070000 	li	a3,0x0
1e8178:	8cc7fffc 	lw	a3,-4(a2)
1e817c:	00f23824 	and	a3,a3,s2
1e8180:	10a7004f 	beq	a1,a3,0x1e82c0
1e8184:	34050000 	li	a1,0x0
1e8188:	34050000 	li	a1,0x0
1e818c:	10c00003 	beqz	a2,0x1e819c
1e8190:	8e130000 	lw	s3,0(s0)
1e8194:	8cc5fffc 	lw	a1,-4(a2)
1e8198:	00b22824 	and	a1,a1,s2
1e819c:	34060000 	li	a2,0x0
1e81a0:	00c5382a 	slt	a3,a2,a1
1e81a4:	10e00005 	beqz	a3,0x1e81bc
1e81a8:	00000000 	nop
1e81ac:	24c60001 	addiu	a2,a2,1
1e81b0:	00c5382a 	slt	a3,a2,a1
1e81b4:	14e0fffe 	bnez	a3,0x1e81b0
1e81b8:	24c60001 	addiu	a2,a2,1
1e81bc:	1260000f 	beqz	s3,0x1e81fc
1e81c0:	2673fffc 	addiu	s3,s3,-4
1e81c4:	5260000c 	beqzl	s3,0x1e81f8
1e81c8:	ae000000 	sw	zero,0(s0)
1e81cc:	0c0011e8 	jal	0x47a0
1e81d0:	02602025 	move	a0,s3
1e81d4:	8c440024 	lw	a0,36(v0)
1e81d8:	02602825 	move	a1,s3
1e81dc:	8c86001c 	lw	a2,28(a0)
1e81e0:	24c60030 	addiu	a2,a2,48
1e81e4:	84c70000 	lh	a3,0(a2)
1e81e8:	8cc60004 	lw	a2,4(a2)
1e81ec:	00c0f809 	jalr	a2
1e81f0:	00872021 	addu	a0,a0,a3
1e81f4:	ae000000 	sw	zero,0(s0)
1e81f8:	8e240000 	lw	a0,0(s1)
1e81fc:	10800003 	beqz	a0,0x1e820c
1e8200:	34130000 	li	s3,0x0
1e8204:	8c93fffc 	lw	s3,-4(a0)
1e8208:	02729824 	and	s3,s3,s2
1e820c:	0260a025 	move	s4,s3
1e8210:	1e800003 	bgtz	s4,0x1e8220
1e8214:	00000000 	nop
1e8218:	10000012 	b	0x1e8264
1e821c:	34130000 	li	s3,0x0
1e8220:	0c0011e8 	jal	0x47a0
1e8224:	02002025 	move	a0,s0
1e8228:	8c440024 	lw	a0,36(v0)
1e822c:	00132880 	sll	a1,s3,0x2
1e8230:	8c86001c 	lw	a2,28(a0)
1e8234:	24a50004 	addiu	a1,a1,4
1e8238:	24c60028 	addiu	a2,a2,40
1e823c:	84c70000 	lh	a3,0(a2)
1e8240:	8cc90004 	lw	t1,4(a2)
1e8244:	00872021 	addu	a0,a0,a3
1e8248:	3c070037 	lui	a3,0x37
1e824c:	34060004 	li	a2,0x4
1e8250:	24e7cf0c 	addiu	a3,a3,-12532
1e8254:	0120f809 	jalr	t1
1e8258:	34080112 	li	t0,0x112
1e825c:	ac540000 	sw	s4,0(v0)
1e8260:	24530004 	addiu	s3,v0,4
1e8264:	ae130000 	sw	s3,0(s0)
1e8268:	34040000 	li	a0,0x0
1e826c:	8e330000 	lw	s3,0(s1)
1e8270:	34050000 	li	a1,0x0
1e8274:	12600003 	beqz	s3,0x1e8284
1e8278:	34060000 	li	a2,0x0
1e827c:	8e66fffc 	lw	a2,-4(s3)
1e8280:	00d23024 	and	a2,a2,s2
1e8284:	0086302a 	slt	a2,a0,a2
1e8288:	10c0001f 	beqz	a2,0x1e8308
1e828c:	00000000 	nop
1e8290:	8e060000 	lw	a2,0(s0)
1e8294:	00c53021 	addu	a2,a2,a1
1e8298:	50c00007 	beqzl	a2,0x1e82b8
1e829c:	24840001 	addiu	a0,a0,1
1e82a0:	8e270000 	lw	a3,0(s1)
1e82a4:	00e53821 	addu	a3,a3,a1
1e82a8:	c4ec0000 	lwc1	$f12,0(a3)
1e82ac:	e4cc0000 	swc1	$f12,0(a2)
1e82b0:	8e330000 	lw	s3,0(s1)
1e82b4:	24840001 	addiu	a0,a0,1
1e82b8:	1000ffee 	b	0x1e8274
1e82bc:	24a50004 	addiu	a1,a1,4
1e82c0:	34070000 	li	a3,0x0
1e82c4:	10800003 	beqz	a0,0x1e82d4
1e82c8:	34060000 	li	a2,0x0
1e82cc:	8c86fffc 	lw	a2,-4(a0)
1e82d0:	00d23024 	and	a2,a2,s2
1e82d4:	00a6202a 	slt	a0,a1,a2
1e82d8:	1080000b 	beqz	a0,0x1e8308
1e82dc:	00000000 	nop
1e82e0:	8e240000 	lw	a0,0(s1)
1e82e4:	8e060000 	lw	a2,0(s0)
1e82e8:	00872021 	addu	a0,a0,a3
1e82ec:	c48c0000 	lwc1	$f12,0(a0)
1e82f0:	00c72021 	addu	a0,a2,a3
1e82f4:	e48c0000 	swc1	$f12,0(a0)
1e82f8:	24a50001 	addiu	a1,a1,1
1e82fc:	24e70004 	addiu	a3,a3,4
1e8300:	1000fff0 	b	0x1e82c4
1e8304:	8e240000 	lw	a0,0(s1)
1e8308:	02001025 	move	v0,s0
1e830c:	8fb00000 	lw	s0,0(sp)
1e8310:	8fb10004 	lw	s1,4(sp)
1e8314:	8fb20008 	lw	s2,8(sp)
1e8318:	8fb3000c 	lw	s3,12(sp)
1e831c:	8fb40010 	lw	s4,16(sp)
1e8320:	8fbf0014 	lw	ra,20(sp)
1e8324:	03e00008 	jr	ra
1e8328:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cArrayBase<cArrayBase> & cArrayBase<float>::operator=(const cArrayBase<cArrayBase> &)
// Address: 0x001e812c, Size: 512B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void **, s32, s32, s32); /* extern */

void **cArrayBase_cArrayBase_refcArrayBase_float___operator_eq_constcArrayBase_cArrayBase_ref(void **arg0, void **arg1) {
    ? (*temp_a2_3)(void *, void **, ?, s16);
    f32 *temp_a2_5;
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
