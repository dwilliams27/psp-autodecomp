# Battle Packet: `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcEntityCustomAnimation> >::operator=(const cArrayBase<cArrayBase> &)`

## Target

- Address: `0x00241684`
- Size: `512` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcEntityCustomAnimation> >`
- Method family: `operator=`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 24B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `24`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `24`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x00241a70` `failed` 720B `gcCinematicEvent::AssignCopy(const cBase *)`

## Placement

- Canonical source: `src/cArrayBasecArrayBasecArrayBasecHandleTgcEntityCustomAnimation.cpp`
- Header(s): `include/cArrayBasecArrayBasecArrayBasecHandleTgcEntityCustomAnimation.h`
- Allowed source(s): `src/cArrayBasecArrayBasecArrayBasecHandleTgcEntityCustomAnimation.cpp`
- Split-TU prefix: `src/cArrayBasecArrayBasecArrayBasecHandleTgcEntityCustomAnimation_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcEntityCustomAnimation> >.h`

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

- session `bc49800f`; src `src/cArrayBasecArrayBasecArrayBasecHandleTgcEntityCustomAnimation.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/bc49800f/00241684__agent_self_reported_failure__src_cArrayBasecArrayBasecArrayBasecHandleTgcEntityCustomAnimation.cpp`

Started from the tracked cArrayBase<cHandleT<gcEnumeration> > sibling body and added the gcEntityCustomAnimation explicit instantiation. A single-instantiation TU compiled to 492/512 bytes; adding the paired gcEnumeration instantiation did not improve it. I then tested the newer gcTableTemplate failure-snapshot shape with oldData/allocCap temporaries and a goto-based allocation diamond; that recovered the target bgtz+nop / b+li allocation branch but perturbed the prologue and compiled to 500/512 bytes. The closest retained source combines the cleaner sibling prologue/loops with only the allocation diamond and compiles to 496/512 bytes.

The remaining gap is the known template-array register/source-shape drift: the initial this-data load is kept in s3 instead of target a2, and the post-resize copy loop emits the compact null-destination branch instead of the target beqzl with duplicated loop increment and a fresh other.mData load. I tried an explicit continue/duplicated-increment form; it made the symbol size exact but worsened to 162/512 differing bytes. A required permuter pass was attempted with --time 300 --save-best, but tools/permuter.py aborts before search because it cannot extract .text from the temporary template object (.gnu.linkonce section). Not a bnel/beql divergence; retryable structural/regalloc drift.

### Note 2

- session `57aa5012`

Added a real C++ cArrayBase<cHandleT<gcEntityCustomAnimation> >::operator= template instantiation using the existing cArrayBase<cHandleT<...>> sibling pattern, with masked array counts, cMemPool free through the 0x30 delete record, allocation through the 0x28 alloc record, allocation tag 0x36DA98, and element copy loops. The best build emits the correct mangled operator symbol and exact 512-byte size, but compare_func reports 24/512 bytes different. The remaining diff is localized: in the resize-copy setup SNC emits `li a1` before `lw s3` where the target has the load first, and in the same-size copy loop SNC keeps the byte offset in a2 and count in a3 while the target uses offset a3 and count/destination scratch a2.


## Disassembly

```asm
241684:	27bdffe0 	addiu	sp,sp,-32
241688:	afb00000 	sw	s0,0(sp)
24168c:	afb10004 	sw	s1,4(sp)
241690:	00a08825 	move	s1,a1
241694:	00808025 	move	s0,a0
241698:	afb20008 	sw	s2,8(sp)
24169c:	8e240000 	lw	a0,0(s1)
2416a0:	3c124000 	lui	s2,0x4000
2416a4:	34050000 	li	a1,0x0
2416a8:	2652ffff 	addiu	s2,s2,-1
2416ac:	afb3000c 	sw	s3,12(sp)
2416b0:	afb40010 	sw	s4,16(sp)
2416b4:	afbf0014 	sw	ra,20(sp)
2416b8:	10800003 	beqz	a0,0x2416c8
2416bc:	8e060000 	lw	a2,0(s0)
2416c0:	8c85fffc 	lw	a1,-4(a0)
2416c4:	00b22824 	and	a1,a1,s2
2416c8:	10c00003 	beqz	a2,0x2416d8
2416cc:	34070000 	li	a3,0x0
2416d0:	8cc7fffc 	lw	a3,-4(a2)
2416d4:	00f23824 	and	a3,a3,s2
2416d8:	10a7004f 	beq	a1,a3,0x241818
2416dc:	34050000 	li	a1,0x0
2416e0:	34050000 	li	a1,0x0
2416e4:	10c00003 	beqz	a2,0x2416f4
2416e8:	8e130000 	lw	s3,0(s0)
2416ec:	8cc5fffc 	lw	a1,-4(a2)
2416f0:	00b22824 	and	a1,a1,s2
2416f4:	34060000 	li	a2,0x0
2416f8:	00c5382a 	slt	a3,a2,a1
2416fc:	10e00005 	beqz	a3,0x241714
241700:	00000000 	nop
241704:	24c60001 	addiu	a2,a2,1
241708:	00c5382a 	slt	a3,a2,a1
24170c:	14e0fffe 	bnez	a3,0x241708
241710:	24c60001 	addiu	a2,a2,1
241714:	1260000f 	beqz	s3,0x241754
241718:	2673fffc 	addiu	s3,s3,-4
24171c:	5260000c 	beqzl	s3,0x241750
241720:	ae000000 	sw	zero,0(s0)
241724:	0c0011e8 	jal	0x47a0
241728:	02602025 	move	a0,s3
24172c:	8c440024 	lw	a0,36(v0)
241730:	02602825 	move	a1,s3
241734:	8c86001c 	lw	a2,28(a0)
241738:	24c60030 	addiu	a2,a2,48
24173c:	84c70000 	lh	a3,0(a2)
241740:	8cc60004 	lw	a2,4(a2)
241744:	00c0f809 	jalr	a2
241748:	00872021 	addu	a0,a0,a3
24174c:	ae000000 	sw	zero,0(s0)
241750:	8e240000 	lw	a0,0(s1)
241754:	10800003 	beqz	a0,0x241764
241758:	34130000 	li	s3,0x0
24175c:	8c93fffc 	lw	s3,-4(a0)
241760:	02729824 	and	s3,s3,s2
241764:	0260a025 	move	s4,s3
241768:	1e800003 	bgtz	s4,0x241778
24176c:	00000000 	nop
241770:	10000012 	b	0x2417bc
241774:	34130000 	li	s3,0x0
241778:	0c0011e8 	jal	0x47a0
24177c:	02002025 	move	a0,s0
241780:	8c440024 	lw	a0,36(v0)
241784:	00132880 	sll	a1,s3,0x2
241788:	8c86001c 	lw	a2,28(a0)
24178c:	24a50004 	addiu	a1,a1,4
241790:	24c60028 	addiu	a2,a2,40
241794:	84c70000 	lh	a3,0(a2)
241798:	8cc90004 	lw	t1,4(a2)
24179c:	00872021 	addu	a0,a0,a3
2417a0:	3c070037 	lui	a3,0x37
2417a4:	34060004 	li	a2,0x4
2417a8:	24e7da98 	addiu	a3,a3,-9576
2417ac:	0120f809 	jalr	t1
2417b0:	34080112 	li	t0,0x112
2417b4:	ac540000 	sw	s4,0(v0)
2417b8:	24530004 	addiu	s3,v0,4
2417bc:	ae130000 	sw	s3,0(s0)
2417c0:	34040000 	li	a0,0x0
2417c4:	8e330000 	lw	s3,0(s1)
2417c8:	34050000 	li	a1,0x0
2417cc:	12600003 	beqz	s3,0x2417dc
2417d0:	34060000 	li	a2,0x0
2417d4:	8e66fffc 	lw	a2,-4(s3)
2417d8:	00d23024 	and	a2,a2,s2
2417dc:	0086302a 	slt	a2,a0,a2
2417e0:	10c0001f 	beqz	a2,0x241860
2417e4:	00000000 	nop
2417e8:	8e060000 	lw	a2,0(s0)
2417ec:	00c53021 	addu	a2,a2,a1
2417f0:	50c00007 	beqzl	a2,0x241810
2417f4:	24840001 	addiu	a0,a0,1
2417f8:	8e270000 	lw	a3,0(s1)
2417fc:	00e53821 	addu	a3,a3,a1
241800:	8ce70000 	lw	a3,0(a3)
241804:	acc70000 	sw	a3,0(a2)
241808:	8e330000 	lw	s3,0(s1)
24180c:	24840001 	addiu	a0,a0,1
241810:	1000ffee 	b	0x2417cc
241814:	24a50004 	addiu	a1,a1,4
241818:	34070000 	li	a3,0x0
24181c:	10800003 	beqz	a0,0x24182c
241820:	34060000 	li	a2,0x0
241824:	8c86fffc 	lw	a2,-4(a0)
241828:	00d23024 	and	a2,a2,s2
24182c:	00a6202a 	slt	a0,a1,a2
241830:	1080000b 	beqz	a0,0x241860
241834:	00000000 	nop
241838:	8e240000 	lw	a0,0(s1)
24183c:	8e060000 	lw	a2,0(s0)
241840:	00872021 	addu	a0,a0,a3
241844:	8c840000 	lw	a0,0(a0)
241848:	00c73021 	addu	a2,a2,a3
24184c:	acc40000 	sw	a0,0(a2)
241850:	24a50001 	addiu	a1,a1,1
241854:	24e70004 	addiu	a3,a3,4
241858:	1000fff0 	b	0x24181c
24185c:	8e240000 	lw	a0,0(s1)
241860:	02001025 	move	v0,s0
241864:	8fb00000 	lw	s0,0(sp)
241868:	8fb10004 	lw	s1,4(sp)
24186c:	8fb20008 	lw	s2,8(sp)
241870:	8fb3000c 	lw	s3,12(sp)
241874:	8fb40010 	lw	s4,16(sp)
241878:	8fbf0014 	lw	ra,20(sp)
24187c:	03e00008 	jr	ra
241880:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cArrayBase<cArrayBase> & cArrayBase<cHandleT<gcEntityCustomAnimation> >::operator=(const cArrayBase<cArrayBase> &)
// Address: 0x00241684, Size: 512B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void **, s32, s32, s32); /* extern */

void **cArrayBase_cArrayBase_refcArrayBase_cHandleT_gcEntityCustomAnimation____operator_eq_constcArrayBase_cArrayBase_ref(void **arg0, void **arg1) {
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
