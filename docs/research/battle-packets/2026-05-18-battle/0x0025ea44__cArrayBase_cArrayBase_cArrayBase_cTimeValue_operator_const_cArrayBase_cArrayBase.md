# Battle Packet: `cArrayBase<cArrayBase> & cArrayBase<cTimeValue>::operator=(const cArrayBase<cArrayBase> &)`

## Target

- Address: `0x0025ea44`
- Size: `512` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `cArrayBase<cArrayBase> & cArrayBase<cTimeValue>`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `131`
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
- `0x0025ec44` `untried` 688B `cArrayBase<cArray<cTimeValue> >::SetSize(int, const cArrayBase &)`

## Placement

- Canonical source: `src/cArrayBasecArrayBasecArrayBasecTimeValue.cpp`
- Header(s): `include/cArrayBasecArrayBasecArrayBasecTimeValue.h`
- Allowed source(s): `src/cArrayBasecArrayBasecArrayBasecTimeValue.cpp`
- Split-TU prefix: `src/cArrayBasecArrayBasecArrayBasecTimeValue_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/cArrayBase<cArrayBase> & cArrayBase<cTimeValue>.h`

Not found.

## Matched Method Exemplars

### Exemplar 1: `eCamera::operator=(const eCamera &)`

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

### Exemplar 2: `gcExpressionList::operator=(const gcExpressionList &)`

- Address: `0x000d515c`
- Size: `356` bytes
- Source: `src/gcExpressionList.cpp`

```cpp
gcExpressionList &gcExpressionList::operator=(const gcExpressionList &other) {
    DeleteExpressions();

    if (*(ExprNode **)(char *)&other != NULL) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        ExprNode *src = *(ExprNode **)(char *)&other;
        CloneEntry *clone = (CloneEntry *)(src->dispatch + 0x10);
        *(ExprNode **)(char *)this =
            clone->func((char *)src + clone->offset, pool,
                        *(cBase **)((char *)this + 4));

        src = *(ExprNode **)(char *)&other;
        DispatchEntry *headNext = (DispatchEntry *)(src->dispatch + 0xA8);
        src = (ExprNode *)headNext->func((char *)src + headNext->offset);
        ExprNode *dst = *(ExprNode **)(char *)this;
        if (dst != NULL) {
            do {
                if (src != NULL) {
                    LinkEntry *link = (LinkEntry *)(dst->dispatch + 0xB0);
                    char *target = (char *)dst + link->offset;
                    CloneEntry *srcClone = (CloneEntry *)(src->dispatch + 0x10);
                    link->func(target,
                               srcClone->func((char *)src + srcClone->offset,
                                              pool,
                                              *(cBase **)((char *)this + 4)));

                    DispatchEntry *srcNext =
                        (DispatchEntry *)(src->dispatch + 0xA8);
                    src =
                        (ExprNode *)srcNext->func((char *)src + srcNext->offset);
                } else {
                    LinkEntry *link = (LinkEntry *)(dst->dispatch + 0xB0);
                    link->func((char *)dst + link->offset, NULL);
                }

                DispatchEntry *dstNext = (DispatchEntry *)(dst->dispatch + 0xA8);
                dst = (ExprNode *)dstNext->func((char *)dst + dstNext->offset);
            } while (dst != NULL);
        }
    }

    return *this;
}
```

### Exemplar 3: `gcStateMachine::operator=(const gcStateMachine &)`

- Address: `0x0010cc8c`
- Size: `332` bytes
- Source: `src/gcStateMachine.cpp`

```cpp
// ── gcStateMachine::operator=(const gcStateMachine &) @ 0x0010cc8c ──
gcStateMachine &gcStateMachine::operator=(const gcStateMachine &other) {
    ((cObject *)this)->operator=(*(const cObject *)&other);

    const int *srcField44 = (const int *)((const char *)&other + 0x44);
    int *dstField44 = (int *)((char *)this + 0x44);
    const int *srcSelfHandle = (const int *)((const char *)&other + 0x48);
    int *dstSelfHandle = (int *)((char *)this + 0x48);
    *dstField44 = *srcField44;
    int copiedSelfHandle = *srcSelfHandle;
    const int *srcField4C = (const int *)((const char *)&other + 0x4C);
    int *dstField4C = (int *)((char *)this + 0x4C);
    *dstSelfHandle = copiedSelfHandle;
    int copiedField4C = *srcField4C;
    *dstField4C = copiedField4C;
    ((cBaseArray *)((char *)this + 0x50))->operator=(
        *(const cBaseArray *)((const char *)&other + 0x50));
    ((cBaseArray *)((char *)this + 0x58))->operator=(
        *(const cBaseArray *)((const char *)&other + 0x58));
    ((cBaseArray *)((char *)this + 0x60))->operator=(
        *(const cBaseArray *)((const char *)&other + 0x60));
    ((cArrayBase<gcStateVTableEntry> *)((char *)this + 0x68))->operator=(
        *(const cArrayBase<gcStateVTableEntry> *)((const char *)&other + 0x68));

    int handle = *(const int *)((const char *)&other + 0x48);
    HandleEntry *entry;
    if (handle == 0) {
        entry = 0;
    } else {
        HandleEntry *lookup = (HandleEntry *)D_00038890[handle & 0xFFFF];
        entry = 0;
        if (lookup != 0) {
            if (lookup->mId == handle) {
                entry = lookup;
            }
        }
    }

    if (entry != 0) {
        int self;
        if (handle == 0) {
            self = (&other == 0);
        } else {
            HandleEntry *lookup = (HandleEntry *)D_00038890[handle & 0xFFFF];
            HandleEntry *entry2 = 0;
            if (lookup != 0) {
                if (lookup->mId == handle) {
                    entry2 = lookup;
                }
            }
            self = (((int)entry2 ^ (int)&other) == 0);
        }
        self = (unsigned char)self;
        if (self != 0) {
            int id = 0;
            if (this != 0) {
                id = *(int *)((char *)this + 0x30);
            }
            volatile int temp = id;
            *dstSelfHandle = temp;
        }
    }
    return *this;
}
```

### Exemplar 4: `gcEvent::operator=(const gcEvent &)`

- Address: `0x000d61d0`
- Size: `68` bytes
- Source: `src/gcEvent.cpp`

```cpp
// gcEvent::operator=(const gcEvent &) — 0x000d61d0

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }
```


## Prior Failure Notes

### Note 1

- session `bbf16b5f`

Implemented the cArrayBase<cTimeValue>::operator= instantiation in the same file because cTimeValue is also a 4-byte element and its disassembly is byte-for-byte the same template body as the gcTableTemplate handle variant. It reaches the same 16/512-byte near miss with identical differing offsets after relocation masking: 0x140/0x144 instruction ordering in the resized copy loop, and a2/a3 register allocation in the equal-size copy loop. Tried helper vs manual size loads, explicit source/destination pointer splits, declaration reordering, register annotations, memory barriers, alternate conditions, int vs pointer offset variables, -Xsched=1, -Xxopt=0, -Xmopt=0, and a throwaway /tmp variant sweep over several hundred loop forms. The repo permuter was also attempted but failed before searching because it could not extract .text from the temporary object for this weak template-symbol file.


## Disassembly

```asm
25ea44:	27bdffe0 	addiu	sp,sp,-32
25ea48:	afb00000 	sw	s0,0(sp)
25ea4c:	afb10004 	sw	s1,4(sp)
25ea50:	00a08825 	move	s1,a1
25ea54:	00808025 	move	s0,a0
25ea58:	afb20008 	sw	s2,8(sp)
25ea5c:	8e240000 	lw	a0,0(s1)
25ea60:	3c124000 	lui	s2,0x4000
25ea64:	34050000 	li	a1,0x0
25ea68:	2652ffff 	addiu	s2,s2,-1
25ea6c:	afb3000c 	sw	s3,12(sp)
25ea70:	afb40010 	sw	s4,16(sp)
25ea74:	afbf0014 	sw	ra,20(sp)
25ea78:	10800003 	beqz	a0,0x25ea88
25ea7c:	8e060000 	lw	a2,0(s0)
25ea80:	8c85fffc 	lw	a1,-4(a0)
25ea84:	00b22824 	and	a1,a1,s2
25ea88:	10c00003 	beqz	a2,0x25ea98
25ea8c:	34070000 	li	a3,0x0
25ea90:	8cc7fffc 	lw	a3,-4(a2)
25ea94:	00f23824 	and	a3,a3,s2
25ea98:	10a7004f 	beq	a1,a3,0x25ebd8
25ea9c:	34050000 	li	a1,0x0
25eaa0:	34050000 	li	a1,0x0
25eaa4:	10c00003 	beqz	a2,0x25eab4
25eaa8:	8e130000 	lw	s3,0(s0)
25eaac:	8cc5fffc 	lw	a1,-4(a2)
25eab0:	00b22824 	and	a1,a1,s2
25eab4:	34060000 	li	a2,0x0
25eab8:	00c5382a 	slt	a3,a2,a1
25eabc:	10e00005 	beqz	a3,0x25ead4
25eac0:	00000000 	nop
25eac4:	24c60001 	addiu	a2,a2,1
25eac8:	00c5382a 	slt	a3,a2,a1
25eacc:	14e0fffe 	bnez	a3,0x25eac8
25ead0:	24c60001 	addiu	a2,a2,1
25ead4:	1260000f 	beqz	s3,0x25eb14
25ead8:	2673fffc 	addiu	s3,s3,-4
25eadc:	5260000c 	beqzl	s3,0x25eb10
25eae0:	ae000000 	sw	zero,0(s0)
25eae4:	0c0011e8 	jal	0x47a0
25eae8:	02602025 	move	a0,s3
25eaec:	8c440024 	lw	a0,36(v0)
25eaf0:	02602825 	move	a1,s3
25eaf4:	8c86001c 	lw	a2,28(a0)
25eaf8:	24c60030 	addiu	a2,a2,48
25eafc:	84c70000 	lh	a3,0(a2)
25eb00:	8cc60004 	lw	a2,4(a2)
25eb04:	00c0f809 	jalr	a2
25eb08:	00872021 	addu	a0,a0,a3
25eb0c:	ae000000 	sw	zero,0(s0)
25eb10:	8e240000 	lw	a0,0(s1)
25eb14:	10800003 	beqz	a0,0x25eb24
25eb18:	34130000 	li	s3,0x0
25eb1c:	8c93fffc 	lw	s3,-4(a0)
25eb20:	02729824 	and	s3,s3,s2
25eb24:	0260a025 	move	s4,s3
25eb28:	1e800003 	bgtz	s4,0x25eb38
25eb2c:	00000000 	nop
25eb30:	10000012 	b	0x25eb7c
25eb34:	34130000 	li	s3,0x0
25eb38:	0c0011e8 	jal	0x47a0
25eb3c:	02002025 	move	a0,s0
25eb40:	8c440024 	lw	a0,36(v0)
25eb44:	00132880 	sll	a1,s3,0x2
25eb48:	8c86001c 	lw	a2,28(a0)
25eb4c:	24a50004 	addiu	a1,a1,4
25eb50:	24c60028 	addiu	a2,a2,40
25eb54:	84c70000 	lh	a3,0(a2)
25eb58:	8cc90004 	lw	t1,4(a2)
25eb5c:	00872021 	addu	a0,a0,a3
25eb60:	3c070037 	lui	a3,0x37
25eb64:	34060004 	li	a2,0x4
25eb68:	24e7da98 	addiu	a3,a3,-9576
25eb6c:	0120f809 	jalr	t1
25eb70:	34080112 	li	t0,0x112
25eb74:	ac540000 	sw	s4,0(v0)
25eb78:	24530004 	addiu	s3,v0,4
25eb7c:	ae130000 	sw	s3,0(s0)
25eb80:	34040000 	li	a0,0x0
25eb84:	8e330000 	lw	s3,0(s1)
25eb88:	34050000 	li	a1,0x0
25eb8c:	12600003 	beqz	s3,0x25eb9c
25eb90:	34060000 	li	a2,0x0
25eb94:	8e66fffc 	lw	a2,-4(s3)
25eb98:	00d23024 	and	a2,a2,s2
25eb9c:	0086302a 	slt	a2,a0,a2
25eba0:	10c0001f 	beqz	a2,0x25ec20
25eba4:	00000000 	nop
25eba8:	8e060000 	lw	a2,0(s0)
25ebac:	00c53021 	addu	a2,a2,a1
25ebb0:	50c00007 	beqzl	a2,0x25ebd0
25ebb4:	24840001 	addiu	a0,a0,1
25ebb8:	8e270000 	lw	a3,0(s1)
25ebbc:	00e53821 	addu	a3,a3,a1
25ebc0:	8ce70000 	lw	a3,0(a3)
25ebc4:	acc70000 	sw	a3,0(a2)
25ebc8:	8e330000 	lw	s3,0(s1)
25ebcc:	24840001 	addiu	a0,a0,1
25ebd0:	1000ffee 	b	0x25eb8c
25ebd4:	24a50004 	addiu	a1,a1,4
25ebd8:	34070000 	li	a3,0x0
25ebdc:	10800003 	beqz	a0,0x25ebec
25ebe0:	34060000 	li	a2,0x0
25ebe4:	8c86fffc 	lw	a2,-4(a0)
25ebe8:	00d23024 	and	a2,a2,s2
25ebec:	00a6202a 	slt	a0,a1,a2
25ebf0:	1080000b 	beqz	a0,0x25ec20
25ebf4:	00000000 	nop
25ebf8:	8e240000 	lw	a0,0(s1)
25ebfc:	8e060000 	lw	a2,0(s0)
25ec00:	00872021 	addu	a0,a0,a3
25ec04:	8c840000 	lw	a0,0(a0)
25ec08:	00c73021 	addu	a2,a2,a3
25ec0c:	acc40000 	sw	a0,0(a2)
25ec10:	24a50001 	addiu	a1,a1,1
25ec14:	24e70004 	addiu	a3,a3,4
25ec18:	1000fff0 	b	0x25ebdc
25ec1c:	8e240000 	lw	a0,0(s1)
25ec20:	02001025 	move	v0,s0
25ec24:	8fb00000 	lw	s0,0(sp)
25ec28:	8fb10004 	lw	s1,4(sp)
25ec2c:	8fb20008 	lw	s2,8(sp)
25ec30:	8fb3000c 	lw	s3,12(sp)
25ec34:	8fb40010 	lw	s4,16(sp)
25ec38:	8fbf0014 	lw	ra,20(sp)
25ec3c:	03e00008 	jr	ra
25ec40:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cArrayBase<cArrayBase> & cArrayBase<cTimeValue>::operator=(const cArrayBase<cArrayBase> &)
// Address: 0x0025ea44, Size: 512B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void **, s32, s32, s32); /* extern */

void **cArrayBase_cArrayBase_refcArrayBase_cTimeValue___operator_eq_constcArrayBase_cArrayBase_ref(void **arg0, void **arg1) {
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
            temp_v0 = (temp_a2_4 + 0x28)->unk4(temp_a0_3 + temp_a2_4->unk28, (var_s3 * 4) + 4, 4, 0x36DA98);
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
