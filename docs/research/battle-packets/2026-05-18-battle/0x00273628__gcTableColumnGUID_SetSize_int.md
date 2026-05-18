# Battle Packet: `gcTableColumnGUID::SetSize(int)`

## Target

- Address: `0x00273628`
- Size: `416` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcTableColumnGUID`
- Method family: `SetSize`
- Leaf: `False`

## Queue Metadata

- score: `138`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 25B; correct function size; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `25`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `25`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

## Placement

- Canonical source: `src/gcTableColumnGUID.cpp`
- Header(s): `include/gcTableColumnGUID.h`
- Allowed source(s): `src/gcTableColumnGUID.cpp`, `src/cXML_cNode.cpp`
- Split-TU prefix: `src/gcTableColumnGUID_*.cpp`

## Class Header: `include/gcTableColumnGUID.h`

Not found.

## Matched Same-Class Neighbors

- `0x0012b814` 200B `gcTableColumnGUID::Read(cFile &, cMemPool *)` — `src/gcTableColumnGUID.cpp`
- `0x00273490` 52B `gcTableColumnGUID::AssignCopy(const cBase *)` — `src/gcTableColumnGUID.cpp`
- `0x002734c4` 136B `gcTableColumnGUID::New(cMemPool *, cBase *) static` — `src/gcTableColumnGUID.cpp`
- `0x0027354c` 220B `gcTableColumnGUID::GetType(void) const` — `src/gcTableColumnGUID.cpp`
- `0x002737c8` 88B `gcTableColumnGUID::Set(int, const wchar_t *, bool)` — `src/gcTableColumnGUID.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcEntityAnimationConfigSet::SetSize(int)`

- Address: `0x0025aa80`
- Size: `28` bytes
- Source: `src/gcEntityAnimationConfigSet.cpp`

```cpp
//   0x0025aa80 gcEntityAnimationConfigSet::SetSize(int)
//   0x0025a8c4 gcEntityAnimationConfigSet::AssignCopy(const cBase *)
//   0x0010e270 gcEntityAnimationConfigSet::Write(cFile &) const
//   0x0025ad30 gcEntityAnimationConfigSet::~gcEntityAnimationConfigSet(void)
//   0x0025a908 gcEntityAnimationConfigSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```

### Exemplar 2: `gcEntityAttackSet::SetSize(int)`

- Address: `0x0025c864`
- Size: `28` bytes
- Source: `src/gcEntityAttackSet.cpp`

```cpp
//   0x0025c864 gcEntityAttackSet::SetSize(int)
//   0x0025c6a8 gcEntityAttackSet::AssignCopy(const cBase *)
//   0x0010fa78 gcEntityAttackSet::Write(cFile &) const
//   0x0025cb14 gcEntityAttackSet::~gcEntityAttackSet(void)
//   0x0025c6ec gcEntityAttackSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```


## Prior Failure Notes

### Note 1

- session `05dfc607`; src `src/cXML_cNode.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/05dfc607/00273628__agent_self_reported_failure__src_cXML_cNode.cpp`

Closest source compiles to the correct 416-byte size and matches the original control-flow shape for allocation failure, the copy loop, and the old-buffer delete path, but 25 bytes still differ. I tried if/else versus goto allocation joins, loop-carried destination pointers, ternary versus if source selection, explicit allocator temporaries to recover the a1/a2/a3/t1 call sequence, empty asm barriers, register-asm hints for the copy and cleanup temporaries, and an m2c-style old-count loop rewrite. A 120-second permuter run generated 4,536 candidates, compiled 1,676, and found no improvement from the 25-byte baseline.

The remaining diff appears to be register-allocation/context drift rather than source semantics: the allocator call now matches exactly, and the first copy-loop precheck matches except for an equivalent initial destination `move a2,s3` where original uses `addu a2,s3,a1` with a1 known zero. The cleanup block uses a0/a1/a2/a3 for oldData/oldCount/index/condition while the original reuses s1 for oldData and a0/a1/a2 for count/index/condition. Further local source reshaping either preserved the same 25-byte diff or regressed size/control flow.


## Disassembly

```asm
273628:	27bdffe0 	addiu	sp,sp,-32
27362c:	afa00000 	sw	zero,0(sp)
273630:	afb00008 	sw	s0,8(sp)
273634:	00808025 	move	s0,a0
273638:	afa00004 	sw	zero,4(sp)
27363c:	8e040008 	lw	a0,8(s0)
273640:	afb20010 	sw	s2,16(sp)
273644:	34120000 	li	s2,0x0
273648:	afb1000c 	sw	s1,12(sp)
27364c:	afb30014 	sw	s3,20(sp)
273650:	afbf0018 	sw	ra,24(sp)
273654:	10800005 	beqz	a0,0x27366c
273658:	00a08825 	move	s1,a1
27365c:	8c92fffc 	lw	s2,-4(a0)
273660:	3c044000 	lui	a0,0x4000
273664:	2484ffff 	addiu	a0,a0,-1
273668:	02449024 	and	s2,s2,a0
27366c:	1251004f 	beq	s2,s1,0x2737ac
273670:	00000000 	nop
273674:	1e200003 	bgtz	s1,0x273684
273678:	00000000 	nop
27367c:	10000012 	b	0x2736c8
273680:	34130000 	li	s3,0x0
273684:	0c0011e8 	jal	0x47a0
273688:	26040008 	addiu	a0,s0,8
27368c:	8c440024 	lw	a0,36(v0)
273690:	001128c0 	sll	a1,s1,0x3
273694:	8c86001c 	lw	a2,28(a0)
273698:	24a50004 	addiu	a1,a1,4
27369c:	24c60028 	addiu	a2,a2,40
2736a0:	84c70000 	lh	a3,0(a2)
2736a4:	8cc90004 	lw	t1,4(a2)
2736a8:	00872021 	addu	a0,a0,a3
2736ac:	3c070037 	lui	a3,0x37
2736b0:	34060004 	li	a2,0x4
2736b4:	24e7da98 	addiu	a3,a3,-9576
2736b8:	0120f809 	jalr	t1
2736bc:	34080112 	li	t0,0x112
2736c0:	ac510000 	sw	s1,0(v0)
2736c4:	24530004 	addiu	s3,v0,4
2736c8:	16600004 	bnez	s3,0x2736dc
2736cc:	34040000 	li	a0,0x0
2736d0:	1e200036 	bgtz	s1,0x2737ac
2736d4:	00000000 	nop
2736d8:	34040000 	li	a0,0x0
2736dc:	0091282a 	slt	a1,a0,s1
2736e0:	10a00013 	beqz	a1,0x273730
2736e4:	34050000 	li	a1,0x0
2736e8:	02653021 	addu	a2,s3,a1
2736ec:	00c03825 	move	a3,a2
2736f0:	10e0000a 	beqz	a3,0x27371c
2736f4:	0092402a 	slt	t0,a0,s2
2736f8:	11000004 	beqz	t0,0x27370c
2736fc:	03a04025 	move	t0,sp
273700:	8e080008 	lw	t0,8(s0)
273704:	10000001 	b	0x27370c
273708:	01054021 	addu	t0,t0,a1
27370c:	8d090000 	lw	t1,0(t0)
273710:	ace90000 	sw	t1,0(a3)
273714:	8d080004 	lw	t0,4(t0)
273718:	ace80004 	sw	t0,4(a3)
27371c:	24840001 	addiu	a0,a0,1
273720:	24a50008 	addiu	a1,a1,8
273724:	0091382a 	slt	a3,a0,s1
273728:	14e0fff0 	bnez	a3,0x2736ec
27372c:	24c60008 	addiu	a2,a2,8
273730:	8e110008 	lw	s1,8(s0)
273734:	12200005 	beqz	s1,0x27374c
273738:	34040000 	li	a0,0x0
27373c:	8e24fffc 	lw	a0,-4(s1)
273740:	3c054000 	lui	a1,0x4000
273744:	24a5ffff 	addiu	a1,a1,-1
273748:	00852024 	and	a0,a0,a1
27374c:	34050000 	li	a1,0x0
273750:	00a4302a 	slt	a2,a1,a0
273754:	10c00005 	beqz	a2,0x27376c
273758:	00000000 	nop
27375c:	24a50001 	addiu	a1,a1,1
273760:	00a4302a 	slt	a2,a1,a0
273764:	14c0fffe 	bnez	a2,0x273760
273768:	24a50001 	addiu	a1,a1,1
27376c:	1220000e 	beqz	s1,0x2737a8
273770:	2631fffc 	addiu	s1,s1,-4
273774:	5220000c 	beqzl	s1,0x2737a8
273778:	ae000008 	sw	zero,8(s0)
27377c:	0c0011e8 	jal	0x47a0
273780:	02202025 	move	a0,s1
273784:	8c440024 	lw	a0,36(v0)
273788:	02202825 	move	a1,s1
27378c:	8c86001c 	lw	a2,28(a0)
273790:	24c60030 	addiu	a2,a2,48
273794:	84c70000 	lh	a3,0(a2)
273798:	8cc60004 	lw	a2,4(a2)
27379c:	00c0f809 	jalr	a2
2737a0:	00872021 	addu	a0,a0,a3
2737a4:	ae000008 	sw	zero,8(s0)
2737a8:	ae130008 	sw	s3,8(s0)
2737ac:	8fb00008 	lw	s0,8(sp)
2737b0:	8fb1000c 	lw	s1,12(sp)
2737b4:	8fb20010 	lw	s2,16(sp)
2737b8:	8fb30014 	lw	s3,20(sp)
2737bc:	8fbf0018 	lw	ra,24(sp)
2737c0:	03e00008 	jr	ra
2737c4:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcTableColumnGUID::SetSize(int)
// Address: 0x00273628, Size: 416B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */

void gcTableColumnGUID__SetSize_int(void *arg0, s32 arg1) {
    ? (*temp_a2_3)(void *, void *, ?, s16);
    s16 temp_a3;
    s32 *temp_v0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2_2;
    s32 var_s2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a2;
    void *temp_a2_2;
    void *temp_s1;
    void *temp_s1_2;
    void *var_a2;
    void *var_s3;
    void *var_t0;

    temp_a0 = arg0->unk8;
    var_s2 = 0;
    if (temp_a0 != NULL) {
        var_s2 = temp_a0->unk-4 & 0x3FFFFFFF;
    }
    if (var_s2 != arg1) {
        if (arg1 <= 0) {
            var_s3 = NULL;
        } else {
            temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0 + 8)->unk24;
            temp_a2 = temp_a0_2->unk1C;
            temp_v0 = (temp_a2 + 0x28)->unk4(temp_a0_2 + temp_a2->unk28, (arg1 * 8) + 4, 4, 0x36DA98);
            *temp_v0 = arg1;
            var_s3 = temp_v0 + 4;
        }
        var_a0 = 0;
        if (var_s3 == NULL) {
            if (arg1 <= 0) {
                var_a0 = 0;
                goto block_9;
            }
        } else {
block_9:
            var_a1 = 0;
            if (arg1 > 0) {
                var_a2 = var_s3;
                do {
                    if (var_a2 != NULL) {
                        var_t0 = sp;
                        if (var_a0 < var_s2) {
                            var_t0 = arg0->unk8 + var_a1;
                        }
                        var_a2->unk0 = (s32) var_t0->unk0;
                        var_a2->unk4 = (s32) var_t0->unk4;
                    }
                    var_a0 += 1;
                    var_a1 += 8;
                    var_a2 += 8;
                } while (var_a0 < arg1);
            }
            temp_s1 = arg0->unk8;
            var_a0_2 = 0;
            if (temp_s1 != NULL) {
                var_a0_2 = temp_s1->unk-4 & 0x3FFFFFFF;
            }
            var_a1_2 = 0;
            var_a2_2 = var_a0_2 > 0;
            if (var_a2_2 != 0) {
                var_a1_2 = 1;
                do {
                    var_a2_2 = var_a1_2 < var_a0_2;
                    var_a1_2 += 1;
                } while (var_a2_2 != 0);
            }
            temp_s1_2 = temp_s1 - 4;
            if (temp_s1 != NULL) {
                if (temp_s1_2 != NULL) {
                    temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s1_2, var_a1_2, var_a2_2)->unk24;
                    temp_a2_2 = temp_a0_3->unk1C;
                    temp_a3 = temp_a2_2->unk30;
                    temp_a2_3 = (temp_a2_2 + 0x30)->unk4;
                    temp_a2_3(temp_a0_3 + temp_a3, temp_s1_2, temp_a2_3, temp_a3);
                }
                arg0->unk8 = NULL;
            }
            arg0->unk8 = var_s3;
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
