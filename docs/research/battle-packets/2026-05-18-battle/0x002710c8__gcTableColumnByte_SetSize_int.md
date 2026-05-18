# Battle Packet: `gcTableColumnByte::SetSize(int)`

## Target

- Address: `0x002710c8`
- Size: `384` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcTableColumnByte`
- Method family: `SetSize`
- Leaf: `False`

## Queue Metadata

- score: `129`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 59B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `59`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `59`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

## Placement

- Canonical source: `src/gcTableColumnByte.cpp`
- Header(s): `include/gcTableColumnByte.h`
- Allowed source(s): `src/gcTableColumnByte.cpp`
- Split-TU prefix: `src/gcTableColumnByte_*.cpp`

## Class Header: `include/gcTableColumnByte.h`

Not found.

## Matched Same-Class Neighbors

- `0x0012ac3c` 152B `gcTableColumnByte::Write(cFile &) const` — `src/gcTableColumnByte.cpp`
- `0x0012acd4` 200B `gcTableColumnByte::Read(cFile &, cMemPool *)` — `src/gcTableColumnByte.cpp`
- `0x00270f64` 136B `gcTableColumnByte::New(cMemPool *, cBase *) static` — `src/gcTableColumnByte.cpp`
- `0x00270fec` 220B `gcTableColumnByte::GetType(void) const` — `src/gcTableColumnByte.cpp`
- `0x00271248` 64B `gcTableColumnByte::Set(int, const wchar_t *, bool)` — `src/gcTableColumnByte.cpp`

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

- session `673b88c6`; src `src/gcTableColumnByte.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/673b88c6/002710c8__agent_self_reported_failure__src_gcTableColumnByte.cpp`

category=REG_ALLOC. Closest source compiles to the correct 384-byte size and matches control flow, allocation call shape, copy loop, branch-likely forms, immediates, stack frame size, and calls. The remaining mismatch is 24 bytes in the old-buffer cleanup block starting at function offset 0xe8: our compiler assigns oldData/oldCount/oldIndex to a0/a1/a2/a3, while the original reuses s1 for oldData and shifts the scalar temporaries to a0/a1/a2. The first differing instruction is actual lw a0,8(s0) versus expected lw s1,8(s0), followed by equivalent beqz/lw/lui/and/slt branch instructions with register fields shifted.

Tried straightforward cArray-style SetSize, explicit memory barrier to preserve the nop after the allocation-failure bgtz, reusing the size parameter as the oldData carrier, register-bound local attempts, volatile oldData, and sched=1 (worse, 59-byte diff). Ran one permuter pass for 300s with --save-best: 9,360 candidates generated, 4,053 compiled, best remained 24 bytes, zero improvements. Root cause appears to be SNC context-sensitive general-purpose register allocation drift, not source-level structure.


## Disassembly

```asm
2710c8:	27bdffe0 	addiu	sp,sp,-32
2710cc:	afb00000 	sw	s0,0(sp)
2710d0:	00808025 	move	s0,a0
2710d4:	8e040008 	lw	a0,8(s0)
2710d8:	afb20008 	sw	s2,8(sp)
2710dc:	34120000 	li	s2,0x0
2710e0:	afb10004 	sw	s1,4(sp)
2710e4:	afb3000c 	sw	s3,12(sp)
2710e8:	afbf0010 	sw	ra,16(sp)
2710ec:	10800005 	beqz	a0,0x271104
2710f0:	00a08825 	move	s1,a1
2710f4:	8c92fffc 	lw	s2,-4(a0)
2710f8:	3c044000 	lui	a0,0x4000
2710fc:	2484ffff 	addiu	a0,a0,-1
271100:	02449024 	and	s2,s2,a0
271104:	12510049 	beq	s2,s1,0x27122c
271108:	00000000 	nop
27110c:	1e200003 	bgtz	s1,0x27111c
271110:	00000000 	nop
271114:	10000011 	b	0x27115c
271118:	34130000 	li	s3,0x0
27111c:	0c0011e8 	jal	0x47a0
271120:	26040008 	addiu	a0,s0,8
271124:	8c440024 	lw	a0,36(v0)
271128:	8c86001c 	lw	a2,28(a0)
27112c:	26250004 	addiu	a1,s1,4
271130:	24c60028 	addiu	a2,a2,40
271134:	84c70000 	lh	a3,0(a2)
271138:	8cc90004 	lw	t1,4(a2)
27113c:	00872021 	addu	a0,a0,a3
271140:	3c070037 	lui	a3,0x37
271144:	34060001 	li	a2,0x1
271148:	24e7da98 	addiu	a3,a3,-9576
27114c:	0120f809 	jalr	t1
271150:	34080112 	li	t0,0x112
271154:	ac510000 	sw	s1,0(v0)
271158:	24530004 	addiu	s3,v0,4
27115c:	16600004 	bnez	s3,0x271170
271160:	34040000 	li	a0,0x0
271164:	1e200031 	bgtz	s1,0x27122c
271168:	00000000 	nop
27116c:	34040000 	li	a0,0x0
271170:	0091282a 	slt	a1,a0,s1
271174:	50a0000f 	beqzl	a1,0x2711b4
271178:	8e110008 	lw	s1,8(s0)
27117c:	00932821 	addu	a1,a0,s3
271180:	10a00007 	beqz	a1,0x2711a0
271184:	0092382a 	slt	a3,a0,s2
271188:	10e00004 	beqz	a3,0x27119c
27118c:	34060000 	li	a2,0x0
271190:	8e060008 	lw	a2,8(s0)
271194:	00c43021 	addu	a2,a2,a0
271198:	90c60000 	lbu	a2,0(a2)
27119c:	a0a60000 	sb	a2,0(a1)
2711a0:	24840001 	addiu	a0,a0,1
2711a4:	0091282a 	slt	a1,a0,s1
2711a8:	14a0fff5 	bnez	a1,0x271180
2711ac:	00932821 	addu	a1,a0,s3
2711b0:	8e110008 	lw	s1,8(s0)
2711b4:	12200005 	beqz	s1,0x2711cc
2711b8:	34040000 	li	a0,0x0
2711bc:	8e24fffc 	lw	a0,-4(s1)
2711c0:	3c054000 	lui	a1,0x4000
2711c4:	24a5ffff 	addiu	a1,a1,-1
2711c8:	00852024 	and	a0,a0,a1
2711cc:	34050000 	li	a1,0x0
2711d0:	00a4302a 	slt	a2,a1,a0
2711d4:	10c00005 	beqz	a2,0x2711ec
2711d8:	00000000 	nop
2711dc:	24a50001 	addiu	a1,a1,1
2711e0:	00a4302a 	slt	a2,a1,a0
2711e4:	14c0fffe 	bnez	a2,0x2711e0
2711e8:	24a50001 	addiu	a1,a1,1
2711ec:	1220000e 	beqz	s1,0x271228
2711f0:	2631fffc 	addiu	s1,s1,-4
2711f4:	5220000c 	beqzl	s1,0x271228
2711f8:	ae000008 	sw	zero,8(s0)
2711fc:	0c0011e8 	jal	0x47a0
271200:	02202025 	move	a0,s1
271204:	8c440024 	lw	a0,36(v0)
271208:	02202825 	move	a1,s1
27120c:	8c86001c 	lw	a2,28(a0)
271210:	24c60030 	addiu	a2,a2,48
271214:	84c70000 	lh	a3,0(a2)
271218:	8cc60004 	lw	a2,4(a2)
27121c:	00c0f809 	jalr	a2
271220:	00872021 	addu	a0,a0,a3
271224:	ae000008 	sw	zero,8(s0)
271228:	ae130008 	sw	s3,8(s0)
27122c:	8fb00000 	lw	s0,0(sp)
271230:	8fb10004 	lw	s1,4(sp)
271234:	8fb20008 	lw	s2,8(sp)
271238:	8fb3000c 	lw	s3,12(sp)
27123c:	8fbf0010 	lw	ra,16(sp)
271240:	03e00008 	jr	ra
271244:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcTableColumnByte::SetSize(int)
// Address: 0x002710c8, Size: 384B
// Obj: gcAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */

void gcTableColumnByte__SetSize_int(void *arg0, s32 arg1) {
    ? (*temp_a2_3)(void *, void *, ?, s16);
    s16 temp_a3;
    s32 *temp_v0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1_2;
    s32 var_a2_2;
    s32 var_s2;
    u8 *temp_a0;
    u8 *temp_s1;
    u8 *var_a1;
    u8 *var_s3;
    u8 var_a2;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a2;
    void *temp_a2_2;
    void *temp_s1_2;

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
            temp_v0 = (temp_a2 + 0x28)->unk4(temp_a0_2 + temp_a2->unk28, arg1 + 4, 1, 0x36DA98);
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
            if (arg1 > 0) {
                var_a1 = var_s3;
                do {
                    if (var_a1 != NULL) {
                        var_a2 = 0;
                        if (var_a0 < var_s2) {
                            var_a2 = *(arg0->unk8 + var_a0);
                        }
                        *var_a1 = var_a2;
                    }
                    var_a0 += 1;
                    var_a1 = var_a0 + var_s3;
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
