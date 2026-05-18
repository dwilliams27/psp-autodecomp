# Battle Packet: `gcUIGeom::Reset(cMemPool *, bool)`

## Target

- Address: `0x0013b458`
- Size: `140` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcUIGeom`
- Method family: `Reset`
- Leaf: `True`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 6B; codegen-specific diagnosis; manageable size`
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

## Placement

- Canonical source: `src/gcUIGeom.cpp`
- Header(s): `include/gcUIGeom.h`
- Allowed source(s): `src/gcUIGeom.cpp`
- Split-TU prefix: `src/gcUIGeom_*.cpp`

## Class Header: `include/gcUIGeom.h`

Not found.

## Matched Same-Class Neighbors

- `0x0013b268` 76B `gcUIGeom::Write(cFile &) const` — `src/gcUIGeom.cpp`
- `0x0013b2b4` 188B `gcUIGeom::Read(cFile &, cMemPool *)` — `src/gcUIGeom.cpp`
- `0x0013b370` 68B `gcUIGeom::gcUIGeom(cBase *)` — `src/gcUIGeom.cpp`
- `0x0013b3b4` 164B `gcUIGeom::~gcUIGeom(void)` — `src/gcUIGeom.cpp`
- `0x0013b8c0` 88B `gcUIGeom::Update(cTimeValue)` — `src/gcUIGeom.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcUI::Reset(void)`

- Address: `0x000e0520`
- Size: `48` bytes
- Source: `src/gcEntityCustomAnimationGroup.cpp`

```cpp
void gcUI::Reset() {
    mFader0.Reset();
    mFader1.Reset();
}
```

### Exemplar 2: `gcUIDialog::Reset(cMemPool *, bool)`

- Address: `0x000e38cc`
- Size: `32` bytes
- Source: `src/gcUIDialog.cpp`

```cpp
void gcUIDialog::Reset(cMemPool *pool, bool flag) {
    ((gcUIWidgetGroup *)((char *)this + 0x98))->Reset(pool, true);
}
```

### Exemplar 3: `gcUIGeomTemplate::Reset(cMemPool *, bool)`

- Address: `0x0013bcd0`
- Size: `8` bytes
- Source: `src/gcUIGeomTemplate.cpp`

```cpp
void gcUIGeomTemplate::Reset(cMemPool *, bool) {
}
```

### Exemplar 4: `gcRoomInstance::Reset(cMemPool *, bool)`

- Address: `0x0013c6e8`
- Size: `176` bytes
- Source: `src/gcRoomInstance.cpp`

```cpp
void gcRoomInstance::Reset(cMemPool *, bool) {
    int value = *(int *)((char *)this + 0x44);
    int valid;
    if (value == 0) {
        valid = 0;
    } else {
        HandleEntry *entry = D_00038890[value & 0xFFFF];
        HandleEntry *found = 0;
        if (entry != 0) {
            if (entry->handle == value) {
                found = entry;
            }
        }
        valid = found != 0;
    }

    if ((valid & 0xFF) != 0) {
        HandleEntry *entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        int *nextRoom = (int *)((char *)this + 0x48);
        int next = *nextRoom;
        __asm__ volatile(
            "addiu $a3, %1, 0x00e8\n"
            "sw %0, 0($a3)"
            :
            : "r"(next), "r"(entry)
            : "a3", "memory");

        value = *(volatile int *)((char *)this + 0x44);
        entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        entry->roomInstance = (int)this;
    }
}
```


## Prior Failure Notes

### Note 1

- session `9396db86`; src `src/gcUIGeom.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/9396db86/0013b458__agent_self_reported_failure__src_gcUIGeom.cpp`

Closest source is a C++ implementation of the Reset dirty-update path: load geom at +0x60, compute geom+0x50 * 1.4142135f * 0.5f, compare against this+0x74, store the new value, call the vtable entry at +0x70, then set this+0x8D. Tried direct expression, split geom/factor/half locals, volatile old-value load, condition inversion, sched=1, register-bound float locals, and a short inline-asm old-value load. The best stable form is 6/140 bytes off; all control flow, integer registers, store/call sequence, and size match.

The remaining diff is isolated to FPU register allocation/order in the first arithmetic block: current uses mtc1 constant into $f14 and old value in $f13, with c.eq.s $f12,$f13, while target uses constant in $f13, old value in $f14, and c.eq.s $f14,$f12. A 120s permuter run generated 2688 candidates / 1191 compiled with zero improvement (best stayed 6 bytes). This looks like an SNC FPU register-allocation/scheduling artifact with no obvious source leverage beyond the tried restructurings.


## Disassembly

```asm
13b458:	27bdfff0 	addiu	sp,sp,-16
13b45c:	afb00000 	sw	s0,0(sp)
13b460:	00808025 	move	s0,a0
13b464:	8e040060 	lw	a0,96(s0)
13b468:	afbf0004 	sw	ra,4(sp)
13b46c:	50800018 	beqzl	a0,0x13b4d0
13b470:	34040001 	li	a0,0x1
13b474:	c48c0050 	lwc1	$f12,80(a0)
13b478:	3c043fb5 	lui	a0,0x3fb5
13b47c:	348404f3 	ori	a0,a0,0x4f3
13b480:	44846800 	mtc1	a0,$f13
13b484:	c60e0074 	lwc1	$f14,116(s0)
13b488:	460d6302 	mul.s	$f12,$f12,$f13
13b48c:	3c043f00 	lui	a0,0x3f00
13b490:	44847800 	mtc1	a0,$f15
13b494:	460f6302 	mul.s	$f12,$f12,$f15
13b498:	460c7032 	c.eq.s	$f14,$f12
13b49c:	00000000 	nop
13b4a0:	4501000b 	bc1t	0x13b4d0
13b4a4:	34040001 	li	a0,0x1
13b4a8:	8e040004 	lw	a0,4(s0)
13b4ac:	e60c0074 	swc1	$f12,116(s0)
13b4b0:	24840070 	addiu	a0,a0,112
13b4b4:	84850000 	lh	a1,0(a0)
13b4b8:	8c870004 	lw	a3,4(a0)
13b4bc:	02052021 	addu	a0,s0,a1
13b4c0:	34050004 	li	a1,0x4
13b4c4:	00e0f809 	jalr	a3
13b4c8:	00003025 	move	a2,zero
13b4cc:	34040001 	li	a0,0x1
13b4d0:	a204008d 	sb	a0,141(s0)
13b4d4:	8fb00000 	lw	s0,0(sp)
13b4d8:	8fbf0004 	lw	ra,4(sp)
13b4dc:	03e00008 	jr	ra
13b4e0:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcUIGeom::Reset(cMemPool *, bool)
// Address: 0x0013b458, Size: 140B
// Obj: gcAll_psp.obj

void gcUIGeom__Reset_cMemPoolptr_bool(void *arg0) {
    ? (*temp_a3)(f32, f32, void *, ?, ?, ?);
    f32 temp_f12;
    f32 temp_f14;
    void *temp_a0;
    void *temp_a0_2;

    temp_a0 = arg0->unk60;
    if (temp_a0 != NULL) {
        temp_f14 = arg0->unk74;
        temp_f12 = temp_a0->unk50 * 1.4142135f * 0.5f;
        if (temp_f14 != temp_f12) {
            temp_a0_2 = arg0->unk4;
            arg0->unk74 = temp_f12;
            temp_a3 = (temp_a0_2 + 0x70)->unk4;
            temp_a3(temp_f12, temp_f14, arg0 + temp_a0_2->unk70, 4, 0, temp_a3);
        }
    }
    arg0->unk8D = 1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
