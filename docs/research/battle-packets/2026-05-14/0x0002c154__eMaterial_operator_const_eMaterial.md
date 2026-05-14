# Battle Packet: `eMaterial::operator=(const eMaterial &)`

## Target

- Address: `0x0002c154`
- Size: `128` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMaterial`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `182`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 2B; codegen-specific diagnosis; manageable size`
- failure_action: `research`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `2`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00009d70` `matched` 84B `cObject::operator=(const cObject &)`
- `0x0002c304` `matched` 8B `eMaterial::CopyMaterialData(eMaterialData *)`

Callers:
- `0x00082154` `failed` 388B `eOnePassModelMtl::operator=(const eOnePassModelMtl &)`
- `0x000857a8` `untried` 428B `eRefractionModelMtl::operator=(const eRefractionModelMtl &)`
- `0x00087220` `matched` 148B `eSilhouetteModelMtl::operator=(const eSilhouetteModelMtl &)`
- `0x0008b554` `untried` 540B `eTwoPassModelMtl::operator=(const eTwoPassModelMtl &)`
- `0x001e4434` `matched` 144B `eStandardSpriteMtl::AssignCopy(const cBase *)`
- `0x0021581c` `matched` 160B `eProjectedModelMtl::AssignCopy(const cBase *)`
- `0x00216a1c` `matched` 160B `eShadowFillModelMtl::AssignCopy(const cBase *)`
- `0x00217948` `matched` 136B `eDynamicLightHeightmapMtl::AssignCopy(const cBase *)`
- `0x00217c20` `matched` 160B `eDynamicLightModelMtl::AssignCopy(const cBase *)`
- `0x00219210` `matched` 136B `eProjectedDynamicMtl::AssignCopy(const cBase *)`
- `0x002194ac` `matched` 136B `eProjectedHeightmapMtl::AssignCopy(const cBase *)`
- `0x00219784` `matched` 312B `eReflectionModelMtl::AssignCopy(const cBase *)`
- `0x0021a814` `matched` 176B `eShadowVolumeModelMtl::AssignCopy(const cBase *)`
- `0x0021ab14` `matched` 176B `eExtrudedShadowVolumeModelMtl::AssignCopy(const cBase *)`
- `0x0021b590` `matched` 152B `eStandardGeomTrailMtl::AssignCopy(const cBase *)`
- `0x0021ba7c` `matched` 136B `eStandardHeightmapMtl::AssignCopy(const cBase *)`
- `0x0021bd54` `matched` 136B `eStandardParticleSystemMtl::AssignCopy(const cBase *)`
- `0x0021c02c` `matched` 136B `eStandardWeatherEffectMtl::AssignCopy(const cBase *)`

## Placement

- Canonical source: `src/eMaterial.cpp`
- Header(s): `include/eMaterial.h`
- Allowed source(s): `src/eMaterial.cpp`, `src/eMaterial_operator_eq.cpp`
- Split-TU prefix: `src/eMaterial_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/eMaterial.h`

Not found.

## Matched Same-Class Neighbors

- `0x0002bf4c` 88B `eMaterial::Write(cFile &) const` — `src/eMaterial.cpp`
- `0x0002bfa4` 208B `eMaterial::Read(cFile &, cMemPool *)` — `src/eMaterial.cpp`
- `0x0002c074` 92B `eMaterial::eMaterial(cBase *)` — `src/eMaterial.cpp`
- `0x0002c0d0` 132B `eMaterial::~eMaterial(void)` — `src/eMaterial.cpp`
- `0x0002c1d4` 72B `eMaterial::Flip(void) static` — `src/gcTableColumn.cpp`

## Matched Method Exemplars

### Exemplar 1: `eTextureMap::operator=(const eTextureMap &)`

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

### Exemplar 3: `eCamera::operator=(const eCamera &)`

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

### Exemplar 4: `eCamera::operator=(const eCamera &)`

- Address: `0x000f8410`
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


## Prior Failure Notes

### Note 1

- session `0aa429db`; src `src/eMaterial_operator_eq.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/0aa429db/0002c154__agent_self_reported_failure__src_eMaterial_operator_eq.cpp`

Down to 2/128 bytes diff matching prior agent's best. Diff is exactly the rt nibble in the addiu pair at offsets 0x28 and 0x2c: expected `addiu a0,s0,72` then `addiu a1,s1,72` (src first), got `addiu a1,s1,72` then `addiu a0,s0,72` (dst first). All other bytes match including base cObject assignment, mField44 copy, loop body (lw/sw/slti/bnez), pointer increments in correct slots (dst++ before slti, src++ in delay), tail copies (mField50/54), CopyMaterialData call with mField58 in delay slot, and epilogue.

Approaches tried (extending prior agent's set): (1) Plain int* declarations both src-first and dst-first - SNC picks weird allocation without binding (10-byte diff). (2) `register int *src asm("$a0")` and `register int *dst asm("$a1")` with src declared first - SNC IGNORES the binding and emits src->$a1, dst->$a0 (6-byte diff). (3) Same bindings with dst declared first - SNC respects bindings (src->$a0, dst->$a1) but emits addiu a1 BEFORE addiu a0 (2-byte diff, our best). (4) Inverted bindings src->$a1, dst->$a0 - same 2-byte output (SNC ignores bindings here too). (5) Inline-asm explicit `addiu` instructions - addiu order correct but SNC then re-allocates loop registers wrongly (12+ byte diff). (6) Tried every combination of -Xsched=0/1/2 -Xxopt=0/1/2 -Xmopt=0/1/2; sched=2 is best at 2 bytes, others much worse. (7) #pragma sched=1 at function scope - same 2-byte baseline. (8) Asm barriers (`__asm__ volatile("" : "+r"(src))`) between declarations - flips back to 6-byte. (9) Permuter ran 240s (3869 candidates compiled) from the 2-byte baseline with zero improvement.

Used split-TU pattern in src/eMaterial_operator_eq.cpp to avoid perturbing the matched siblings in src/eMaterial.cpp (verified all 5 prior matches stay intact).

Root cause hypothesis: SNC's instruction scheduler has a fixed ordering preference for two adjacent independent addiu instructions in this context: it consistently emits the higher-numbered destination register first ($a1 before $a0). The original game source likely used a structure that forced SNC to materialize the source pointer first via a dependency we haven't identified - possibly a different parent class layout, a non-trivial cObject::operator= invocation, or an inlined helper. The 2-byte rt-field swap is the only remaining diff; the bnel/beql unmatchable category does not apply (this is addiu, not branch-likely).

### Note 2

- session `4f600325`

Best C reconstruction compiles to a 2-byte mismatch in the two pointer setup instructions for the 0x48 two-word copy loop: expected `addiu a0,s0,72` followed by `addiu a1,s1,72`, while SNC emits the same two instructions in the opposite order for the best source form. The rest of the function, including base cObject assignment, loop body, signed `slti`, tail field copies, CopyMaterialData call, and epilogue, matches after relocation masking.

Manual variants tried included signed vs unsigned loop counters, source-first and destination-first pointer declarations, split pointer assignments, base-pointer temporaries, explicit register variable attempts, empty asm register barriers, and different pointer increment/source ordering. The permuter was run for 300 seconds with `--save-best`; it generated 10248 candidates, compiled 4994, and found zero improvement over the 2-byte baseline. Root cause appears to be an SNC scheduling/register setup ordering choice for the adjacent `addiu` pointer initializers, not a semantic source issue.


## Disassembly

```asm
2c154:	27bdfff0 	addiu	sp,sp,-16
2c158:	afb00000 	sw	s0,0(sp)
2c15c:	00a08025 	move	s0,a1
2c160:	afb10004 	sw	s1,4(sp)
2c164:	afbf0008 	sw	ra,8(sp)
2c168:	0c00275c 	jal	0x9d70
2c16c:	00808825 	move	s1,a0
2c170:	8e040044 	lw	a0,68(s0)
2c174:	ae240044 	sw	a0,68(s1)
2c178:	34060000 	li	a2,0x0
2c17c:	26040048 	addiu	a0,s0,72
2c180:	26250048 	addiu	a1,s1,72
2c184:	8c870000 	lw	a3,0(a0)
2c188:	24c60001 	addiu	a2,a2,1
2c18c:	aca70000 	sw	a3,0(a1)
2c190:	24a50004 	addiu	a1,a1,4
2c194:	28c70002 	slti	a3,a2,2
2c198:	14e0fffa 	bnez	a3,0x2c184
2c19c:	24840004 	addiu	a0,a0,4
2c1a0:	8e040050 	lw	a0,80(s0)
2c1a4:	ae240050 	sw	a0,80(s1)
2c1a8:	8e050054 	lw	a1,84(s0)
2c1ac:	02202025 	move	a0,s1
2c1b0:	ae250054 	sw	a1,84(s1)
2c1b4:	0c00b0c1 	jal	0x2c304
2c1b8:	8e050058 	lw	a1,88(s0)
2c1bc:	02201025 	move	v0,s1
2c1c0:	8fb00000 	lw	s0,0(sp)
2c1c4:	8fb10004 	lw	s1,4(sp)
2c1c8:	8fbf0008 	lw	ra,8(sp)
2c1cc:	03e00008 	jr	ra
2c1d0:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eMaterial::operator=(const eMaterial &)
// Address: 0x0002c154, Size: 128B
// Obj: eAll_psp.obj

? cObject__operator_eq_constcObjectref();           /* extern */
? eMaterial__CopyMaterialData_eMaterialDataptr(void *, s32, s32, s32); /* extern */

void *eMaterial__operator_eq_consteMaterialref(void *arg0, void *arg1) {
    s32 *var_a0;
    s32 *var_a1;
    s32 temp_a3;
    s32 var_a2;

    cObject__operator_eq_constcObjectref();
    arg0->unk44 = (s32) arg1->unk44;
    var_a2 = 0;
    var_a0 = arg1 + 0x48;
    var_a1 = arg0 + 0x48;
    do {
        var_a2 += 1;
        *var_a1 = *var_a0;
        var_a1 += 4;
        temp_a3 = var_a2 < 2;
        var_a0 += 4;
    } while (temp_a3 != 0);
    arg0->unk50 = (s32) arg1->unk50;
    arg0->unk54 = (s32) arg1->unk54;
    eMaterial__CopyMaterialData_eMaterialDataptr(arg0, arg1->unk58, var_a2, temp_a3);
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
