# Battle Packet: `cBaseArray::SetSize(int)`

## Target

- Address: `0x00006f60`
- Size: `348` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cBaseArray`
- Method family: `SetSize`
- Leaf: `False`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 16B; manageable size`
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
- `0x00007430` `untried` 120B `cBaseArray::Allocate(int)`

Callers:
- `0x000072d8` `matched` 176B `cBaseArray::Read(cReadBlock &)`
- `0x0004c8d0` `untried` 956B `eDynamicModelTemplate::Reset(cMemPool *, bool)`
- `0x0005cd40` `failed` 1048B `eSky::Read(cFile &, cMemPool *)`
- `0x00061b48` `untried` 520B `eWeatherSystem::Reset(cMemPool *, bool)`
- `0x00077144` `matched` 324B `eRigidBodyControllerTemplate::Read(cFile &, cMemPool *)`
- `0x000a1264` `failed` 1336B `eRoom::Read(cFile &, cMemPool *)`
- `0x000a6098` `untried` 3244B `eDynamicModel::Reset(cMemPool *, bool)`
- `0x000ac0e8` `untried` 1072B `eSky::Reset(cMemPool *, bool)`
- `0x000af7f8` `untried` 972B `eSimulatedController::Reset(cMemPool *, bool)`
- `0x000d4ba4` `failed` 424B `gcEnumeration::AddNew(gcEnumerationEntry *, cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > *)`
- `0x000ee134` `failed` 796B `gcRegionBase::Read(cFile &, cMemPool *)`
- `0x00101de8` `failed` 2084B `gcGameSettings::Read(cFile &, cMemPool *)`
- `0x0010c83c` `failed` 1012B `gcStateMachine::Read(cFile &, cMemPool *)`
- `0x00128e90` `failed` 608B `gcDesiredEventParams::Read(cFile &, cMemPool *)`
- `0x001290f0` `failed` 112B `gcDesiredEventParams::gcDesiredEventParams(cBase *)`
- `0x0012b190` `failed` 108B `gcTableColumnStringTable::SetSize(int)`
- `0x0012bc18` `failed` 216B `gcTableTemplate::Reset(cMemPool *, bool)`
- `0x0014fec0` `matched` 96B `gcDoSwitchCase::gcDoSwitchCase(cBase *)`
- `0x00150218` `matched` 72B `gcValCaseRange::gcValCaseRange(cBase *)`
- `0x001586f0` `matched` 76B `gcValBinaryOp::gcValBinaryOp(cBase *)`
- `0x0015c8bc` `matched` 80B `gcValTrinaryOp::gcValTrinaryOp(cBase *)`
- `0x00164f48` `failed` 1296B `gcMap::Read(cFile &, cMemPool *)`
- `0x00171e80` `untried` 4952B `gcEntity::Reset(cMemPool *, bool)`
- `0x0025aa80` `matched` 28B `gcEntityAnimationConfigSet::SetSize(int)`

## Placement

- Canonical source: `src/cBaseArray.cpp`
- Header(s): `include/cBaseArray.h`
- Allowed source(s): `src/cBaseArray.cpp`
- Split-TU prefix: `src/cBaseArray_*.cpp`

## Class Header: `include/cBaseArray.h`

Not found.

## Matched Same-Class Neighbors

- `0x00006e7c` 228B `cBaseArray::operator=(const cBaseArray &)` — `src/cBaseArray.cpp`
- `0x000071ac` 152B `cBaseArray::Set(int, cBase *)` — `src/cBaseArray.cpp`
- `0x00007244` 148B `cBaseArray::Write(cWriteBlock &) const` — `src/cBaseArray.cpp`
- `0x000072d8` 176B `cBaseArray::Read(cReadBlock &)` — `src/cBaseArray.cpp`
- `0x00007388` 168B `cBaseArray::Reset(cMemPool *)` — `src/cBaseArray.cpp`

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

- session `cda7e2a2`; src `src/cBaseArray.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/cda7e2a2/00006f60__agent_self_reported_failure__src_cBaseArray.cpp`

Closest attempt is a structurally correct C++ cBaseArray::SetSize implementation: it matches the prologue, Allocate call, truncation delete loop, old backing-store free, final assignment, padding branch, and all previously matched cBaseArray siblings remain matched. The remaining diff is 16/348 bytes isolated to the initial copy loop. Expected uses a0=copy index, a3=source offset, t0=destination pointer, and a2=value; SNC keeps rotating the last three caller-save temporaries in this TU, typically emitting a2=offset, a3=destination, t0=value while preserving the same control flow and memory offsets.

Tried pointer loops, indexed loops, int vs cBase* temporaries, splitting copy/remove indices, declaration/increment reordering, local register declarations, and two permuter passes. The longer 300s permuter generated 10,800 candidates / compiled 5,433 and only improved 17 to 16 bytes. This looks like local register-allocation drift in caller-save temporaries for the copy loop rather than a semantic structure issue; next retry should focus only on forcing the copy loop temp assignment without perturbing the matched tail or sibling methods.


## Disassembly

```asm
6f60:	27bdffe0 	addiu	sp,sp,-32
6f64:	afb00004 	sw	s0,4(sp)
6f68:	00808025 	move	s0,a0
6f6c:	8e040000 	lw	a0,0(s0)
6f70:	afb10008 	sw	s1,8(sp)
6f74:	34110000 	li	s1,0x0
6f78:	afb2000c 	sw	s2,12(sp)
6f7c:	afb30010 	sw	s3,16(sp)
6f80:	afb40014 	sw	s4,20(sp)
6f84:	afbf0018 	sw	ra,24(sp)
6f88:	54800001 	bnezl	a0,0x6f90
6f8c:	8c91fffc 	lw	s1,-4(a0)
6f90:	12250040 	beq	s1,a1,0x7094
6f94:	00000000 	nop
6f98:	afa50000 	sw	a1,0(sp)
6f9c:	0c001d0c 	jal	0x7430
6fa0:	02002025 	move	a0,s0
6fa4:	8fa50000 	lw	a1,0(sp)
6fa8:	34040000 	li	a0,0x0
6fac:	0085302a 	slt	a2,a0,a1
6fb0:	10c0000f 	beqz	a2,0x6ff0
6fb4:	00409025 	move	s2,v0
6fb8:	34070000 	li	a3,0x0
6fbc:	02404025 	move	t0,s2
6fc0:	0091482a 	slt	t1,a0,s1
6fc4:	11200004 	beqz	t1,0x6fd8
6fc8:	34060000 	li	a2,0x0
6fcc:	8e060000 	lw	a2,0(s0)
6fd0:	00c73021 	addu	a2,a2,a3
6fd4:	8cc60000 	lw	a2,0(a2)
6fd8:	ad060000 	sw	a2,0(t0)
6fdc:	24840001 	addiu	a0,a0,1
6fe0:	24e70004 	addiu	a3,a3,4
6fe4:	0085302a 	slt	a2,a0,a1
6fe8:	14c0fff5 	bnez	a2,0x6fc0
6fec:	25080004 	addiu	t0,t0,4
6ff0:	00a09825 	move	s3,a1
6ff4:	0271282a 	slt	a1,s3,s1
6ff8:	10a00016 	beqz	a1,0x7054
6ffc:	8e040000 	lw	a0,0(s0)
7000:	0013a080 	sll	s4,s3,0x2
7004:	00942821 	addu	a1,a0,s4
7008:	8ca50000 	lw	a1,0(a1)
700c:	50a0000e 	beqzl	a1,0x7048
7010:	26730001 	addiu	s3,s3,1
7014:	00a02025 	move	a0,a1
7018:	8c860004 	lw	a2,4(a0)
701c:	34050003 	li	a1,0x3
7020:	24c60050 	addiu	a2,a2,80
7024:	84c70000 	lh	a3,0(a2)
7028:	8cc60004 	lw	a2,4(a2)
702c:	00c0f809 	jalr	a2
7030:	00872021 	addu	a0,a0,a3
7034:	8e040000 	lw	a0,0(s0)
7038:	00942021 	addu	a0,a0,s4
703c:	ac800000 	sw	zero,0(a0)
7040:	8e040000 	lw	a0,0(s0)
7044:	26730001 	addiu	s3,s3,1
7048:	0271282a 	slt	a1,s3,s1
704c:	14a0ffed 	bnez	a1,0x7004
7050:	26940004 	addiu	s4,s4,4
7054:	1080000e 	beqz	a0,0x7090
7058:	2491fffc 	addiu	s1,a0,-4
705c:	5220000c 	beqzl	s1,0x7090
7060:	ae000000 	sw	zero,0(s0)
7064:	0c0011e8 	jal	0x47a0
7068:	02202025 	move	a0,s1
706c:	8c440024 	lw	a0,36(v0)
7070:	02202825 	move	a1,s1
7074:	8c86001c 	lw	a2,28(a0)
7078:	24c60030 	addiu	a2,a2,48
707c:	84c70000 	lh	a3,0(a2)
7080:	8cc60004 	lw	a2,4(a2)
7084:	00c0f809 	jalr	a2
7088:	00872021 	addu	a0,a0,a3
708c:	ae000000 	sw	zero,0(s0)
7090:	ae120000 	sw	s2,0(s0)
7094:	8fb00004 	lw	s0,4(sp)
7098:	8fb10008 	lw	s1,8(sp)
709c:	8fb2000c 	lw	s2,12(sp)
70a0:	8fb30010 	lw	s3,16(sp)
70a4:	8fb40014 	lw	s4,20(sp)
70a8:	8fbf0018 	lw	ra,24(sp)
70ac:	03e00008 	jr	ra
70b0:	27bd0020 	addiu	sp,sp,32
70b4:	1000ffff 	b	0x70b4
70b8:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: cBaseArray::SetSize(int)
// Address: 0x00006f60, Size: 348B
// Obj: cAll_psp.obj

s32 *cBaseArray__Allocate_int(s32 **);              /* extern */
void *cMemPool__GetPoolFromPtr_constvoidptrstatic(s32, s32); /* extern */

void cBaseArray__SetSize_int(s32 **arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, ?, ?, s16);
    ? (*temp_a2_4)(void *, s32, ?, s16);
    s16 temp_a3;
    s16 temp_a3_2;
    s32 *temp_a0;
    s32 *temp_s2;
    s32 *var_t0;
    s32 temp_s1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_s1;
    s32 var_s3;
    s32 var_s4;
    void *temp_a0_2;
    void *temp_a1;
    void *temp_a2;
    void *temp_a2_3;

    temp_a0 = *arg0;
    var_s1 = 0;
    if (temp_a0 != NULL) {
        var_s1 = temp_a0->unk-4;
    }
    if (var_s1 != arg1) {
        var_a0 = 0;
        temp_s2 = cBaseArray__Allocate_int(arg0);
        if (subroutine_arg0 > 0) {
            var_a3 = 0;
            var_t0 = temp_s2;
            do {
                var_a2 = 0;
                if (var_a0 < var_s1) {
                    var_a2 = *(*arg0 + var_a3);
                }
                *var_t0 = var_a2;
                var_a0 += 1;
                var_a3 += 4;
                var_t0 += 4;
            } while (var_a0 < subroutine_arg0);
        }
        var_s3 = subroutine_arg0;
        var_a1 = var_s3 < var_s1;
        if (var_a1 != 0) {
            var_s4 = var_s3 * 4;
            do {
                temp_a1 = *(*arg0 + var_s4);
                if (temp_a1 != NULL) {
                    temp_a2 = temp_a1->unk4;
                    temp_a3 = temp_a2->unk50;
                    temp_a2_2 = (temp_a2 + 0x50)->unk4;
                    temp_a2_2(temp_a1 + temp_a3, 3, temp_a2_2, temp_a3);
                    *(*arg0 + var_s4) = 0;
                }
                var_s3 += 1;
                var_a1 = var_s3 < var_s1;
                var_s4 += 4;
            } while (var_a1 != 0);
        }
        temp_s1 = *arg0 - 4;
        if (*arg0 != NULL) {
            if (temp_s1 != 0) {
                temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s1, var_a1)->unk24;
                temp_a2_3 = temp_a0_2->unk1C;
                temp_a3_2 = temp_a2_3->unk30;
                temp_a2_4 = (temp_a2_3 + 0x30)->unk4;
                temp_a2_4(temp_a0_2 + temp_a3_2, temp_s1, temp_a2_4, temp_a3_2);
            }
            *arg0 = NULL;
        }
        *arg0 = temp_s2;
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
