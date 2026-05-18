# Battle Packet: `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcTableTemplate> >::operator=(const cArrayBase<cArrayBase> &)`

## Target

- Address: `0x0024bc50`
- Size: `512` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcTableTemplate> >`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 16B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `16`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x0024be50` `failed` 624B `gcGameSettings::AssignCopy(const cBase *)`

## Placement

- Canonical source: `src/cArrayBasecArrayBasecArrayBasecHandleTgcTableTemplate.cpp`
- Header(s): `include/cArrayBasecArrayBasecArrayBasecHandleTgcTableTemplate.h`
- Allowed source(s): `src/cArrayBasecArrayBasecArrayBasecHandleTgcTableTemplate.cpp`
- Split-TU prefix: `src/cArrayBasecArrayBasecArrayBasecHandleTgcTableTemplate_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcTableTemplate> >.h`

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

- session `bc1fbdd3`; src `src/cArrayBasecArrayBasecArrayBasecHandleTgcTableTemplate.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/bc1fbdd3/0024bc50__agent_self_reported_failure__src_cArrayBasecArrayBasecArrayBasecHandleTgcTableTemplate.cpp`

Created the canonical template-method source using the existing cArrayBase<cHandleT<gcEnumeration> > sibling as the base pattern, with the gcTableTemplate handle allocation tag 0x36DA98. The best stable source emits the correct mangled symbol and matches the allocation/free structure much more closely than the initial 476-byte clone: distinct oldData/allocCap temporaries recover s4, goto allocation shape recovers the target bgtz+nop / b+li allocation diamond, and a memory barrier in the equal-size loop forces the expected nop after the loop-exit branch. Current closest output is still a 500-byte symbol versus the 512-byte target.

Remaining gap is identical to the unsigned-int instantiation. The post-resize loop still emits a compact common-increment null check instead of the target beqzl-with-increment-in-delay shape; attempts using duplicated increments, gotos, and continue made the function size exact but perturbed loop-carried registers badly, producing 180-197 differing bytes. The equal-size loop remains the same a2/a3 offset/count register allocation swap described in the prior notes; register bindings for a2/a3 and srcData/dstData temporaries did not move SNC's choice. sched=1 was tested and did not change the size. tools/permuter.py was attempted for 300s with --save-best but failed before search with 'Failed to extract .text section from /tmp/...cpp.o'.

### Note 2

- session `bbf16b5f`

Implemented a real C++ template instantiation for cArrayBase<cHandleT<gcTableTemplate> >::operator= using the existing 4-byte cArrayBase assignment pattern: size comparison, old allocation deletion through cMemPool::GetPoolFromPtr, allocation via record +0x28 with source tag 0x36DA98 and line 0x112, then element copy. The best source compiles to the correct 512-byte function shape and all allocator/free code matches; the remaining mismatch is 16/512 bytes. The exact differing offsets are 0x140/0x144 (the post-resize copy loop emits li a1,0 before lw s3,0(s1), while expected has those swapped) and 0x194-0x1d0 in the equal-size copy loop, where SNC allocates byte offset in a2 and count/destination base in a3 while the original uses offset in a3 and count/destination pointer in a2.


## Disassembly

```asm
24bc50:	27bdffe0 	addiu	sp,sp,-32
24bc54:	afb00000 	sw	s0,0(sp)
24bc58:	afb10004 	sw	s1,4(sp)
24bc5c:	00a08825 	move	s1,a1
24bc60:	00808025 	move	s0,a0
24bc64:	afb20008 	sw	s2,8(sp)
24bc68:	8e240000 	lw	a0,0(s1)
24bc6c:	3c124000 	lui	s2,0x4000
24bc70:	34050000 	li	a1,0x0
24bc74:	2652ffff 	addiu	s2,s2,-1
24bc78:	afb3000c 	sw	s3,12(sp)
24bc7c:	afb40010 	sw	s4,16(sp)
24bc80:	afbf0014 	sw	ra,20(sp)
24bc84:	10800003 	beqz	a0,0x24bc94
24bc88:	8e060000 	lw	a2,0(s0)
24bc8c:	8c85fffc 	lw	a1,-4(a0)
24bc90:	00b22824 	and	a1,a1,s2
24bc94:	10c00003 	beqz	a2,0x24bca4
24bc98:	34070000 	li	a3,0x0
24bc9c:	8cc7fffc 	lw	a3,-4(a2)
24bca0:	00f23824 	and	a3,a3,s2
24bca4:	10a7004f 	beq	a1,a3,0x24bde4
24bca8:	34050000 	li	a1,0x0
24bcac:	34050000 	li	a1,0x0
24bcb0:	10c00003 	beqz	a2,0x24bcc0
24bcb4:	8e130000 	lw	s3,0(s0)
24bcb8:	8cc5fffc 	lw	a1,-4(a2)
24bcbc:	00b22824 	and	a1,a1,s2
24bcc0:	34060000 	li	a2,0x0
24bcc4:	00c5382a 	slt	a3,a2,a1
24bcc8:	10e00005 	beqz	a3,0x24bce0
24bccc:	00000000 	nop
24bcd0:	24c60001 	addiu	a2,a2,1
24bcd4:	00c5382a 	slt	a3,a2,a1
24bcd8:	14e0fffe 	bnez	a3,0x24bcd4
24bcdc:	24c60001 	addiu	a2,a2,1
24bce0:	1260000f 	beqz	s3,0x24bd20
24bce4:	2673fffc 	addiu	s3,s3,-4
24bce8:	5260000c 	beqzl	s3,0x24bd1c
24bcec:	ae000000 	sw	zero,0(s0)
24bcf0:	0c0011e8 	jal	0x47a0
24bcf4:	02602025 	move	a0,s3
24bcf8:	8c440024 	lw	a0,36(v0)
24bcfc:	02602825 	move	a1,s3
24bd00:	8c86001c 	lw	a2,28(a0)
24bd04:	24c60030 	addiu	a2,a2,48
24bd08:	84c70000 	lh	a3,0(a2)
24bd0c:	8cc60004 	lw	a2,4(a2)
24bd10:	00c0f809 	jalr	a2
24bd14:	00872021 	addu	a0,a0,a3
24bd18:	ae000000 	sw	zero,0(s0)
24bd1c:	8e240000 	lw	a0,0(s1)
24bd20:	10800003 	beqz	a0,0x24bd30
24bd24:	34130000 	li	s3,0x0
24bd28:	8c93fffc 	lw	s3,-4(a0)
24bd2c:	02729824 	and	s3,s3,s2
24bd30:	0260a025 	move	s4,s3
24bd34:	1e800003 	bgtz	s4,0x24bd44
24bd38:	00000000 	nop
24bd3c:	10000012 	b	0x24bd88
24bd40:	34130000 	li	s3,0x0
24bd44:	0c0011e8 	jal	0x47a0
24bd48:	02002025 	move	a0,s0
24bd4c:	8c440024 	lw	a0,36(v0)
24bd50:	00132880 	sll	a1,s3,0x2
24bd54:	8c86001c 	lw	a2,28(a0)
24bd58:	24a50004 	addiu	a1,a1,4
24bd5c:	24c60028 	addiu	a2,a2,40
24bd60:	84c70000 	lh	a3,0(a2)
24bd64:	8cc90004 	lw	t1,4(a2)
24bd68:	00872021 	addu	a0,a0,a3
24bd6c:	3c070037 	lui	a3,0x37
24bd70:	34060004 	li	a2,0x4
24bd74:	24e7da98 	addiu	a3,a3,-9576
24bd78:	0120f809 	jalr	t1
24bd7c:	34080112 	li	t0,0x112
24bd80:	ac540000 	sw	s4,0(v0)
24bd84:	24530004 	addiu	s3,v0,4
24bd88:	ae130000 	sw	s3,0(s0)
24bd8c:	34040000 	li	a0,0x0
24bd90:	8e330000 	lw	s3,0(s1)
24bd94:	34050000 	li	a1,0x0
24bd98:	12600003 	beqz	s3,0x24bda8
24bd9c:	34060000 	li	a2,0x0
24bda0:	8e66fffc 	lw	a2,-4(s3)
24bda4:	00d23024 	and	a2,a2,s2
24bda8:	0086302a 	slt	a2,a0,a2
24bdac:	10c0001f 	beqz	a2,0x24be2c
24bdb0:	00000000 	nop
24bdb4:	8e060000 	lw	a2,0(s0)
24bdb8:	00c53021 	addu	a2,a2,a1
24bdbc:	50c00007 	beqzl	a2,0x24bddc
24bdc0:	24840001 	addiu	a0,a0,1
24bdc4:	8e270000 	lw	a3,0(s1)
24bdc8:	00e53821 	addu	a3,a3,a1
24bdcc:	8ce70000 	lw	a3,0(a3)
24bdd0:	acc70000 	sw	a3,0(a2)
24bdd4:	8e330000 	lw	s3,0(s1)
24bdd8:	24840001 	addiu	a0,a0,1
24bddc:	1000ffee 	b	0x24bd98
24bde0:	24a50004 	addiu	a1,a1,4
24bde4:	34070000 	li	a3,0x0
24bde8:	10800003 	beqz	a0,0x24bdf8
24bdec:	34060000 	li	a2,0x0
24bdf0:	8c86fffc 	lw	a2,-4(a0)
24bdf4:	00d23024 	and	a2,a2,s2
24bdf8:	00a6202a 	slt	a0,a1,a2
24bdfc:	1080000b 	beqz	a0,0x24be2c
24be00:	00000000 	nop
24be04:	8e240000 	lw	a0,0(s1)
24be08:	8e060000 	lw	a2,0(s0)
24be0c:	00872021 	addu	a0,a0,a3
24be10:	8c840000 	lw	a0,0(a0)
24be14:	00c73021 	addu	a2,a2,a3
24be18:	acc40000 	sw	a0,0(a2)
24be1c:	24a50001 	addiu	a1,a1,1
24be20:	24e70004 	addiu	a3,a3,4
24be24:	1000fff0 	b	0x24bde8
24be28:	8e240000 	lw	a0,0(s1)
24be2c:	02001025 	move	v0,s0
24be30:	8fb00000 	lw	s0,0(sp)
24be34:	8fb10004 	lw	s1,4(sp)
24be38:	8fb20008 	lw	s2,8(sp)
24be3c:	8fb3000c 	lw	s3,12(sp)
24be40:	8fb40010 	lw	s4,16(sp)
24be44:	8fbf0014 	lw	ra,20(sp)
24be48:	03e00008 	jr	ra
24be4c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcTableTemplate> >::operator=(const cArrayBase<cArrayBase> &)
// Address: 0x0024bc50, Size: 512B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void **, s32, s32, s32); /* extern */

void **cArrayBase_cArrayBase_refcArrayBase_cHandleT_gcTableTemplate____operator_eq_constcArrayBase_cArrayBase_ref(void **arg0, void **arg1) {
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
