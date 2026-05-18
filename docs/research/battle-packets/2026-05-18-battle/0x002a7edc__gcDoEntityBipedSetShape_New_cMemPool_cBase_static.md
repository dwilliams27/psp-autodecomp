# Battle Packet: `gcDoEntityBipedSetShape::New(cMemPool *, cBase *) static`

## Target

- Address: `0x002a7edc`
- Size: `292` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDoEntityBipedSetShape`
- Method family: `New`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 4B; New retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `constant_cse_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `constant_cse_codegen`
- Near miss: `4`
- Summary: Compiler hoists/reuses constants differently from target.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x0011b6f4` `matched` 32B `gcDesiredObject::gcDesiredObject(cBase *)`
- `0x0011b714` `failed` 24B `gcDesiredEntityHelper::gcDesiredEntityHelper(gcDesiredEntityHelper::gcPrimary, gcDesiredEntityHelper::gcRelationship, gcDesiredEntityHelper::gcRelationship)`
- `0x0012f4c8` `matched` 28B `gcAction::gcAction(cBase *)`

## Placement

- Canonical source: `src/gcDoEntityBipedSetShape.cpp`
- Header(s): `include/gcDoEntityBipedSetShape.h`
- Allowed source(s): `src/gcDoEntityBipedSetShape.cpp`
- Split-TU prefix: `src/gcDoEntityBipedSetShape_*.cpp`

## Compiler Guidance

New template: use pool allocation table lookup, null guard, placement construction or ctor wrapper, and return-null shape matching nearby matched factories.

## Class Header: `include/gcDoEntityBipedSetShape.h`

Not found.

## Matched Same-Class Neighbors

- `0x002a8000` 280B `gcDoEntityBipedSetShape::GetType(void) const` — `src/gcDoEntityBipedSetShape.cpp`
- `0x002a8118` 136B `gcDoEntityBipedSetShape::Write(cFile &) const` — `src/eTexture.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDoCameraFollowEntity::New(cMemPool *, cBase *) static`

- Address: `0x0029520c`
- Size: `292` bytes
- Source: `src/gcDoCameraFollowEntity.cpp`

```cpp
cBase *gcDoCameraFollowEntity::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoCameraFollowEntity *result = 0;
    gcDoCameraFollowEntity *obj =
        (gcDoCameraFollowEntity *)entry->fn(base, 0x7C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoCameraModevirtualtable;

        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x0C, (cBase *)obj);

        *(int *)((char *)obj + 0x44) = (int)obj | 1;
        *(unsigned char *)((char *)obj + 0x48) = 0;
        ((void **)obj)[1] = gcDoCameraFollowEntityvirtualtable;

        char *desired = (char *)obj + 0x4C;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x50) = D_00000338;

        char *helper = (char *)obj + 0x58;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 0x50) = (void *)0x388A48;
        *(void **)((char *)obj + 0x64) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x60) = desired;
        *(void **)((char *)obj + 0x64) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x68) = one;
        *(unsigned char *)((char *)obj + 0x69) = 0;
        *(int *)((char *)obj + 0x6C) = 0;
        *(int *)((char *)obj + 0x70) = 0;
        *(int *)((char *)obj + 0x74) = (int)desired | 1;
        *(int *)((char *)obj + 0x78) = 0;
        result = obj;
    }
    return (cBase *)result;
}
```

### Exemplar 2: `gcDoEntityActivatePartialController::New(cMemPool *, cBase *) static`

- Address: `0x002a0d98`
- Size: `292` bytes
- Source: `src/gcDoEntityActivatePartialController.cpp`

```cpp
cBase *gcDoEntityActivatePartialController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityActivatePartialController *result = 0;
    gcDoEntityActivatePartialController *obj =
        (gcDoEntityActivatePartialController *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityActivatePartialControllervirtualtable;

        char *desired = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[4] = D_00000338;

        char *helper = (char *)obj + 0x18;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        ((void **)obj)[4] = (void *)0x388A48;
        void *baseType = (void *)0x37E6A8;
        ((void **)obj)[9] = baseType;
        ((void **)obj)[8] = desired;
        void *desiredVtable = (void *)0x388568;
        ((void **)obj)[9] = desiredVtable;
        *(unsigned char *)((char *)obj + 0x28) = one;
        *(unsigned char *)((char *)obj + 0x29) = 0;
        *(int *)((char *)obj + 0x2C) = 0;
        int encodedDesired = (int)desired | 1;
        __asm__ volatile("" ::: "memory");
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = encodedDesired;
        __asm__ volatile("" ::: "memory");
        *(void **)((char *)obj + 0x40) = baseType;
        *(int *)((char *)obj + 0x38) = 0;
        *(void **)((char *)obj + 0x3C) = obj;
        *(void **)((char *)obj + 0x40) = desiredVtable;
        *(unsigned char *)((char *)obj + 0x44) = one;
        *(unsigned char *)((char *)obj + 0x45) = 0;
        *(int *)((char *)obj + 0x48) = 0;
        int encodedObj = (int)obj | 1;
        __asm__ volatile("" ::: "memory");
        *(int *)((char *)obj + 0x4C) = 0;
        *(int *)((char *)obj + 0x50) = encodedObj;
        result = obj;
    }
    return (cBase *)result;
}
```

### Exemplar 3: `gcDoCameraOp::New(cMemPool *, cBase *) static`

- Address: `0x00298784`
- Size: `300` bytes
- Source: `src/gcDoCameraOp.cpp`

```cpp
cBase *gcDoCameraOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoCameraOp *result = 0;
    gcDoCameraOp *obj = (gcDoCameraOp *)entry->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoCameraOpvirtualtable;

        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x0C, (cBase *)obj);
        *(int *)((char *)obj + 0x44) = 0;

        int encoded = (int)obj | 1;
        *(unsigned char *)((char *)obj + 0x48) = 0;
        *(int *)((char *)obj + 0x4C) = encoded;
        *(int *)((char *)obj + 0x50) = encoded;
        *(int *)((char *)obj + 0x54) = -1;
        *(int *)((char *)obj + 0x58) = 0;
        *(int *)((char *)obj + 0x5C) = 0;

        char *desired = (char *)obj + 0x60;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x64) = D_00000338;

        char *helper = (char *)obj + 0x6C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 0x64) = (void *)0x388A48;
        *(void **)((char *)obj + 0x78) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x74) = desired;
        *(void **)((char *)obj + 0x78) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x7C) = one;
        *(unsigned char *)((char *)obj + 0x7D) = 0;
        *(int *)((char *)obj + 0x80) = 0;
        *(int *)((char *)obj + 0x84) = 0;
        *(int *)((char *)obj + 0x88) = (int)desired | 1;
        result = obj;
    }
    return (cBase *)result;
}
```

### Exemplar 4: `gcDoEntityPlaySound::New(cMemPool *, cBase *) static`

- Address: `0x002bdc10`
- Size: `304` bytes
- Source: `src/gcDoEntityPlaySound.cpp`

```cpp
cBase *gcDoEntityPlaySound::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityPlaySound *result = 0;
    gcDoEntityPlaySound *obj =
        (gcDoEntityPlaySound *)entry->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityPlaySoundvirtualtable;

        int encodedObj = (int)obj | 1;
        char *desired = (char *)obj + 0x10;
        *(int *)((char *)obj + 0x0C) = encodedObj;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        *(void **)((char *)obj + 0x14) = D_00000338;

        char *helper = (char *)obj + 0x1C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        *(void **)((char *)obj + 0x14) = (void *)0x388A48;
        *(void **)((char *)obj + 0x28) = (void *)0x37E6A8;
        *(void **)((char *)obj + 0x24) = desired;
        *(void **)((char *)obj + 0x28) = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x2C) = one;
        *(unsigned char *)((char *)obj + 0x2D) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = 0;
        *(int *)((char *)obj + 0x38) = (int)desired | 1;
        *(int *)((char *)obj + 0x3C) = 0;
        *(void **)((char *)obj + 0x40) = obj;
        *(int *)((char *)obj + 0x44) = encodedObj;
        *(int *)((char *)obj + 0x48) = encodedObj;
        *(int *)((char *)obj + 0x4C) = encodedObj;
        *(int *)((char *)obj + 0x50) = encodedObj;
        *(unsigned char *)((char *)obj + 0x54) = 0;
        *(unsigned char *)((char *)obj + 0x55) = 0;
        *(int *)((char *)obj + 0x58) = encodedObj;
        result = obj;
    }
    return (cBase *)result;
}
```


## Prior Failure Notes

### Note 1

- session `1a939e2d`; src `src/gcDoEntityBipedSetShape.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/1a939e2d/002a7edc__agent_self_reported_failure__src_gcDoEntityBipedSetShape.cpp`

Closest attempt is now 292B and only 4 bytes differ. I added small inline asm materialization for the two values SNC would not naturally keep live: `addiu s0,s1,12` before the desired-object ctor path and `ori s2,zero,1` before the helper ctor path. That fixed the prior size gap and made the VFPU tail use the right live `s0`/`s2` values. The remaining mismatch is the call argument setup for `gcDesiredObject_gcDesiredObject`: generated code still emits `addiu a0,s1,12` at the call site, while target wants `move a0,s0`. I tried register-bound locals and an asm output constraint to make the compiler recognize the `s0` value as the argument; SNC either ignored the binding or deleted the materialization, reverting to the 288B/284B variants. Root cause appears to be argument rematerialization across the ctor call rather than a structural C issue.

### Note 2

- session `d6b43e12`

Built a new gcDoEntityBipedSetShape TU and matched GetType and Write first, then iterated on New using the embedded gcDesiredEntity pattern from gcDesiredEntity.cpp and the type/factory shape from nearby gcAll_psp.obj exemplars. The closest compiling version uses raw 0x388A48/0x37E6A8 stores plus a compact inline asm tail for the mtc1/mfc1/mtv/sv.q sequence, but compare_func still reports a size mismatch: 284B generated vs 292B expected.

The remaining gap is very specific. The target wants `addiu s0,s1,12` before the gcDesiredObject ctor call and `li s2,1` before the gcDesiredEntityHelper ctor call, then reuses those live values in the final VFPU tail (`sw s0,32(s1)`, `sb s2,40(s1)`, `ori a3,s0,1`). SNC keeps collapsing those into direct `a0` setup for the ctor call and never materializes the `li s2,1`, so the near-match loses exactly those two instructions. I tried: pure-C ordering with symbol loads, raw-immediate ordering, explicit register-variable attempts, a helper-tail asm block, and a broader ctor-scaffold asm attempt. The register-variable attempt made codegen much worse, and SNC would not resolve helper-call symbol names from inline asm (`Undefined local label` on `gcDesiredObject_gcDesiredObject` / `gcDesiredEntityHelper_ctor`). The current best state is the compiling 284B version in src/gcDoEntityBipedSetShape.cpp plus the already-matched GetType/Write.


## Disassembly

```asm
2a7edc:	27bdfff0 	addiu	sp,sp,-16
2a7ee0:	8c840024 	lw	a0,36(a0)
2a7ee4:	afb00000 	sw	s0,0(sp)
2a7ee8:	00a08025 	move	s0,a1
2a7eec:	8c85001c 	lw	a1,28(a0)
2a7ef0:	24a50028 	addiu	a1,a1,40
2a7ef4:	84a60000 	lh	a2,0(a1)
2a7ef8:	8ca90004 	lw	t1,4(a1)
2a7efc:	00862021 	addu	a0,a0,a2
2a7f00:	afb20008 	sw	s2,8(sp)
2a7f04:	34120000 	li	s2,0x0
2a7f08:	34050050 	li	a1,0x50
2a7f0c:	34060010 	li	a2,0x10
2a7f10:	00003825 	move	a3,zero
2a7f14:	afb10004 	sw	s1,4(sp)
2a7f18:	afbf000c 	sw	ra,12(sp)
2a7f1c:	0120f809 	jalr	t1
2a7f20:	00004025 	move	t0,zero
2a7f24:	00408825 	move	s1,v0
2a7f28:	1220002e 	beqz	s1,0x2a7fe4
2a7f2c:	02202025 	move	a0,s1
2a7f30:	0c04bd32 	jal	0x12f4c8
2a7f34:	02002825 	move	a1,s0
2a7f38:	3c040000 	lui	a0,0x0
2a7f3c:	24842348 	addiu	a0,a0,9032
2a7f40:	ae240004 	sw	a0,4(s1)
2a7f44:	2630000c 	addiu	s0,s1,12
2a7f48:	02002025 	move	a0,s0
2a7f4c:	0c046dbd 	jal	0x11b6f4
2a7f50:	02202825 	move	a1,s1
2a7f54:	3c040000 	lui	a0,0x0
2a7f58:	24840338 	addiu	a0,a0,824
2a7f5c:	ae240010 	sw	a0,16(s1)
2a7f60:	26240018 	addiu	a0,s1,24
2a7f64:	34120001 	li	s2,0x1
2a7f68:	34050001 	li	a1,0x1
2a7f6c:	00003025 	move	a2,zero
2a7f70:	0c046dc5 	jal	0x11b714
2a7f74:	00003825 	move	a3,zero
2a7f78:	3c040039 	lui	a0,0x39
2a7f7c:	24848a48 	addiu	a0,a0,-30136
2a7f80:	3c050038 	lui	a1,0x38
2a7f84:	24a5e6a8 	addiu	a1,a1,-6488
2a7f88:	ae240010 	sw	a0,16(s1)
2a7f8c:	ae250024 	sw	a1,36(s1)
2a7f90:	3c040039 	lui	a0,0x39
2a7f94:	44806000 	mtc1	zero,$f12
2a7f98:	ae300020 	sw	s0,32(s1)
2a7f9c:	24848568 	addiu	a0,a0,-31384
2a7fa0:	44056000 	mfc1	a1,$f12
2a7fa4:	ae240024 	sw	a0,36(s1)
2a7fa8:	44066000 	mfc1	a2,$f12
2a7fac:	a2320028 	sb	s2,40(s1)
2a7fb0:	44046000 	mfc1	a0,$f12
2a7fb4:	a2200029 	sb	zero,41(s1)
2a7fb8:	ae20002c 	sw	zero,44(s1)
2a7fbc:	36070001 	ori	a3,s0,0x1
2a7fc0:	ae200030 	sw	zero,48(s1)
2a7fc4:	36280001 	ori	t0,s1,0x1
2a7fc8:	ae270034 	sw	a3,52(s1)
2a7fcc:	ae280038 	sw	t0,56(s1)
2a7fd0:	48e40006 	mtv         $a0, S120
2a7fd4:	48e50026 	mtv         $a1, S121
2a7fd8:	48e60046 	mtv         $a2, S122
2a7fdc:	fa260040 	sv.q        C120, 0x40($s1)
2a7fe0:	02209025 	move	s2,s1
2a7fe4:	02401025 	move	v0,s2
2a7fe8:	8fb00000 	lw	s0,0(sp)
2a7fec:	8fb10004 	lw	s1,4(sp)
2a7ff0:	8fb20008 	lw	s2,8(sp)
2a7ff4:	8fbf000c 	lw	ra,12(sp)
2a7ff8:	03e00008 	jr	ra
2a7ffc:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcDoEntityBipedSetShape::New(cMemPool *, cBase *) static
// Address: 0x002a7edc, Size: 292B
// Obj: gcAll_psp.obj

? gcAction__gcAction_cBaseptr(void *, ?);           /* extern */
? gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship(void *, ?, ?, ?); /* extern */
? gcDesiredObject__gcDesiredObject_cBaseptr(void *, void *); /* extern */

void *gcDoEntityBipedSetShape__New_cMemPoolptr_cBaseptrstatic(void *arg0, ? arg1) {
    void *temp_a0;
    void *temp_a1;
    void *temp_s0;
    void *temp_v0;
    void *var_s2;

    temp_a0 = arg0->unk24;
    temp_a1 = temp_a0->unk1C;
    var_s2 = NULL;
    temp_v0 = (temp_a1 + 0x28)->unk4(temp_a0 + temp_a1->unk28, 0x50, 0x10, 0);
    if (temp_v0 != NULL) {
        gcAction__gcAction_cBaseptr(temp_v0, arg1);
        temp_v0->unk4 = 0x2348;
        temp_s0 = temp_v0 + 0xC;
        gcDesiredObject__gcDesiredObject_cBaseptr(temp_s0, temp_v0);
        temp_v0->unk10 = 0x338;
        gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship(temp_v0 + 0x18, 1, 0, 0);
        temp_v0->unk10 = 0x388A48;
        temp_v0->unk24 = 0x37E6A8;
        temp_v0->unk20 = temp_s0;
        temp_v0->unk24 = 0x388568;
        temp_v0->unk28 = 1;
        temp_v0->unk29 = 0;
        temp_v0->unk2C = 0;
        temp_v0->unk30 = 0;
        temp_v0->unk34 = (s32) ((s32) temp_s0 | 1);
        temp_v0->unk38 = (s32) ((s32) temp_v0 | 1);
        __asm__ volatile("mtv $a0, S120");
        __asm__ volatile("mtv $a1, S121");
        __asm__ volatile("mtv $a2, S122");
        __asm__ volatile("sv.q C120, 0x40($s1)");
        var_s2 = temp_v0;
    }
    return var_s2;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
