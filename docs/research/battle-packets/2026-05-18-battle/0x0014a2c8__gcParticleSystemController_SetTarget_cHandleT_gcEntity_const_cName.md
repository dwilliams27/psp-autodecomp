# Battle Packet: `gcParticleSystemController::SetTarget(cHandleT<gcEntity>, const cName &)`

## Target

- Address: `0x0014a2c8`
- Size: `244` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcParticleSystemController`
- Method family: `SetTarget`
- Leaf: `True`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 12B; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x002d6724` `untried` 1824B `gcDoEntitySubGeomOp::Evaluate(void) const`

## Placement

- Canonical source: `src/gcParticleSystemController.cpp`
- Header(s): `include/gcParticleSystemController.h`
- Allowed source(s): `src/gcParticleSystemController.cpp`
- Split-TU prefix: `src/gcParticleSystemController_*.cpp`

## Class Header: `include/gcParticleSystemController.h`

Not found.

## Matched Same-Class Neighbors

- `0x0014a0a8` 76B `gcParticleSystemController::Write(cFile &) const` — `src/gcParticleSystemController.cpp`
- `0x0014a0f4` 188B `gcParticleSystemController::Read(cFile &, cMemPool *)` — `src/cLanguageSubscriber.cpp`
- `0x0014a1b0` 96B `gcParticleSystemController::gcParticleSystemController(cBase *)` — `src/gcParticleSystemController.cpp`
- `0x0014a210` 128B `gcParticleSystemController::~gcParticleSystemController(void)` — `src/gcParticleSystemController.cpp`
- `0x0014a290` 56B `gcParticleSystemController::Reset(cMemPool *, bool)` — `src/gcParticleSystemController.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcGeomCurveController::SetTarget(cHandleT<gcEntity>, const cName &)`

- Address: `0x001483fc`
- Size: `264` bytes
- Source: `src/gcParticleSystemController.cpp`

```cpp
void gcGeomCurveController::SetTarget(cHandleT<gcEntity> p, const cName &name) {
    m_flag = 0;
    cHandleT<gcEntity> *entityTarget = (cHandleT<gcEntity> *)((char *)this + 0x28);
    *entityTarget = p;

    short subIndex = -1;
    gcHandleEntry *entry;
    if (p.mIndex == 0) {
        entry = 0;
    } else {
        gcHandleEntry *lookup = (gcHandleEntry *)D_00038890[p.mIndex & 0xFFFF];
        entry = 0;
        if (lookup != 0 && lookup->handle == p.mIndex) {
            entry = lookup;
        }
    }

    if (entry != 0) {
        int handle = *(int *)((char *)this + 0x28);
        void *entity = 0;
        if (handle != 0) {
            entity = D_00038890[handle & 0xFFFF];
        }
        gcEntitySubObject *subObject = (gcEntitySubObject *)((char *)entity + 0x80);
        char *classDesc = subObject->classDesc;
        gcSubObjectSlot *slot = (gcSubObjectSlot *)(classDesc + 0xE0);
        int result = slot->fn((char *)subObject + slot->offset, name, 0);
        int signedResult = (short)result;
        subIndex = (short)signedResult;
    }

    gcPointHandleInit pointTarget;
    pointTarget.index = 0;
    m_pad2E = subIndex;
    cHandleT<ePoint> *point = (cHandleT<ePoint> *)((char *)this + 0x24);
    *point = pointTarget.handle;
    char *geom = *(char **)((char *)this + 0xC);
    if (geom != 0) {
        geom[0x229] = 0;
        geom[0x228] = 0;
    }
}
```

### Exemplar 2: `gcGeomCurveController::SetTarget(cHandleT<ePoint>)`

- Address: `0x00148504`
- Size: `28` bytes
- Source: `src/gcGeomCurveController.cpp`

```cpp
void gcGeomCurveController::SetTarget(cHandleT<ePoint> p) {
    m_flag = 1;
    m_target = p;
    cHandleT<ePoint> *p2 = (cHandleT<ePoint> *)((char *)this + 0x28);
    p2->mIndex = 0;
}
```


## Prior Failure Notes

### Note 1

- session `fe522f1e`; src `src/gcParticleSystemController.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/fe522f1e/0014a2c8__agent_self_reported_failure__src_gcParticleSystemController.cpp`

Closest source implements the same handle store, D_00038890 validation, sub-object slot dispatch at classdesc+0xE0, sign-extension pattern, and zero point-handle copy. The function is down to a 12-byte mismatch: all bytes after the initial gcEntity handle validation match, including the final stack temporary copy via gcPointHandleInit. The remaining diff is a register-allocation swap in the first lookup block: generated code keeps the validated entry in t0 and the table load in a3, while expected keeps the final entry in a3 and uses t0 for the table value. Tried lookup declarations inside/outside the block, entry pre-initialization, direct entry validation, void* lookup, volatile/stack temporaries, aggregate init, union init, and sched pragma. The variants either regressed the whole block or fixed unrelated final-copy size only. This appears to be a small register-allocation drift specific to this method; the sibling gcGeomCurveController version matches from the same shared structure.


## Disassembly

```asm
14a2c8:	27bdfff0 	addiu	sp,sp,-16
14a2cc:	afa50000 	sw	a1,0(sp)
14a2d0:	a480002c 	sh	zero,44(a0)
14a2d4:	8fa50000 	lw	a1,0(sp)
14a2d8:	00c03825 	move	a3,a2
14a2dc:	24860028 	addiu	a2,a0,40
14a2e0:	acc50000 	sw	a1,0(a2)
14a2e4:	8fa60000 	lw	a2,0(sp)
14a2e8:	afb00008 	sw	s0,8(sp)
14a2ec:	00808025 	move	s0,a0
14a2f0:	2405ffff 	li	a1,-1
14a2f4:	afbf000c 	sw	ra,12(sp)
14a2f8:	14c00003 	bnez	a2,0x14a308
14a2fc:	00e02025 	move	a0,a3
14a300:	1000000d 	b	0x14a338
14a304:	34070000 	li	a3,0x0
14a308:	30c7ffff 	andi	a3,a2,0xffff
14a30c:	3c080004 	lui	t0,0x4
14a310:	00073880 	sll	a3,a3,0x2
14a314:	25088890 	addiu	t0,t0,-30576
14a318:	00e83821 	addu	a3,a3,t0
14a31c:	8ce80000 	lw	t0,0(a3)
14a320:	11000005 	beqz	t0,0x14a338
14a324:	34070000 	li	a3,0x0
14a328:	8d090030 	lw	t1,48(t0)
14a32c:	15260002 	bne	t1,a2,0x14a338
14a330:	00000000 	nop
14a334:	01003825 	move	a3,t0
14a338:	50e00018 	beqzl	a3,0x14a39c
14a33c:	afa00004 	sw	zero,4(sp)
14a340:	8e060028 	lw	a2,40(s0)
14a344:	10c00007 	beqz	a2,0x14a364
14a348:	34050000 	li	a1,0x0
14a34c:	30c5ffff 	andi	a1,a2,0xffff
14a350:	3c060004 	lui	a2,0x4
14a354:	00052880 	sll	a1,a1,0x2
14a358:	24c68890 	addiu	a2,a2,-30576
14a35c:	00a62821 	addu	a1,a1,a2
14a360:	8ca50000 	lw	a1,0(a1)
14a364:	24a60080 	addiu	a2,a1,128
14a368:	8cc70004 	lw	a3,4(a2)
14a36c:	00802825 	move	a1,a0
14a370:	24e400e0 	addiu	a0,a3,224
14a374:	84870000 	lh	a3,0(a0)
14a378:	8c880004 	lw	t0,4(a0)
14a37c:	00c72021 	addu	a0,a2,a3
14a380:	0100f809 	jalr	t0
14a384:	00003025 	move	a2,zero
14a388:	00022400 	sll	a0,v0,0x10
14a38c:	00042403 	sra	a0,a0,0x10
14a390:	00042c00 	sll	a1,a0,0x10
14a394:	00052c03 	sra	a1,a1,0x10
14a398:	afa00004 	sw	zero,4(sp)
14a39c:	a605002e 	sh	a1,46(s0)
14a3a0:	8fa40004 	lw	a0,4(sp)
14a3a4:	26050024 	addiu	a1,s0,36
14a3a8:	aca40000 	sw	a0,0(a1)
14a3ac:	8fb00008 	lw	s0,8(sp)
14a3b0:	8fbf000c 	lw	ra,12(sp)
14a3b4:	03e00008 	jr	ra
14a3b8:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcParticleSystemController::SetTarget(cHandleT<gcEntity>, const cName &)
// Address: 0x0014a2c8, Size: 244B
// Obj: gcAll_psp.obj

void gcParticleSystemController__SetTarget_cHandleT_gcEntity__constcNameref(void *arg0, s32 arg1, ? arg2) {
    s16 temp_a3_2;
    s16 var_a1;
    s32 temp_a2;
    s32 var_a1_2;
    void *temp_a2_2;
    void *temp_a3;
    void *temp_t0;
    void *var_a3;

    arg0->unk2C = 0;
    arg0->unk28 = (s32) subroutine_arg0;
    var_a1 = -1;
    if (subroutine_arg0 == 0) {
        var_a3 = NULL;
    } else {
        temp_t0 = ((subroutine_arg0 & 0xFFFF) * 4)->unk38890;
        var_a3 = NULL;
        if ((temp_t0 != NULL) && (temp_t0->unk30 == subroutine_arg0)) {
            var_a3 = temp_t0;
        }
    }
    if (var_a3 != NULL) {
        temp_a2 = arg0->unk28;
        var_a1_2 = 0;
        if (temp_a2 != 0) {
            var_a1_2 = ((temp_a2 & 0xFFFF) * 4)->unk38890;
        }
        temp_a2_2 = var_a1_2 + 0x80;
        temp_a3 = temp_a2_2->unk4;
        temp_a3_2 = temp_a3->unkE0;
        var_a1 = (temp_a3 + 0xE0)->unk4(temp_a2_2 + temp_a3_2, arg2, 0, temp_a3_2);
    }
    arg0->unk2E = var_a1;
    arg0->unk24 = (s32) subroutine_arg1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
