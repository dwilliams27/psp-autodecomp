# Battle Packet: `eConvexHullShape::ePolyFace::Write(cWriteBlock &) const`

## Target

- Address: `0x00072950`
- Size: `92` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eConvexHullShape::ePolyFace`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `156`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 10B; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0000553c` `matched` 44B `cWriteBlock::Write(unsigned char)`

Callers:
- `0x000700fc` `failed` 676B `eConvexHullShape::Write(cFile &) const`

## Placement

- Canonical source: `src/eConvexHullShape_ePolyFace.cpp`
- Header(s): `include/eConvexHullShape_ePolyFace.h`
- Allowed source(s): `src/eConvexHullShape_ePolyFace.cpp`
- Split-TU prefix: `src/eConvexHullShape_ePolyFace_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/eConvexHullShape::ePolyFace.h`

Not found.

## Matched Same-Class Neighbors

- `0x000729ac` 124B `eConvexHullShape::ePolyFace::Read(cReadBlock &)` — `src/eConvexHullShape_ePolyFace.cpp`

## Matched Method Exemplars

### Exemplar 1: `eMeshShapeTriList::Write(cWriteBlock &) const`

- Address: `0x0004e950`
- Size: `92` bytes
- Source: `src/eMeshShapeTriList.cpp`

```cpp
// eMeshShapeTriList::Write(cWriteBlock &) const — 0x0004e950
void eMeshShapeTriList::Write(cWriteBlock &wb) const {
    cWriteBlock *out = &wb;
    const eMeshShapeTriList *self = this;
    __asm__ volatile("" : "+r"(out), "+r"(self));
    out->Write(self->mFlags);
    out->Write(self->mNumVerts);
    out->Write(self->mPad);
    out->Write(self->mNumTris);
}
```

### Exemplar 2: `eStaticSurfaceLight::Write(cFile &) const`

- Address: `0x0005ff54`
- Size: `92` bytes
- Source: `src/eStaticSurfaceLight.cpp`

```cpp
// eStaticSurfaceLight::Write(cFile &) const — 0x0005ff54
void eStaticSurfaceLight::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eStaticLight *)this)->Write(file);
    wb.Write(2, (const float *)((const char *)this + 0x90));
    wb.End();
}
```

### Exemplar 3: `eBoxShape::Write(cFile &) const`

- Address: `0x0006d368`
- Size: `92` bytes
- Source: `src/eBoxShape.cpp`

```cpp
// eBoxShape::Write(cFile &) const — 0x0006d368
#pragma control sched=1
void eBoxShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape::Write(file);
    wb.Write(3, mHalfExtents);
    wb.End();
}
```

### Exemplar 4: `eMaterial::Write(cFile &) const`

- Address: `0x0002bf4c`
- Size: `88` bytes
- Source: `src/eMaterial.cpp`

```cpp
void eMaterial::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(mField44);
    wb.End();
}
```


## Prior Failure Notes

### Note 1

- session `7d15adb3`

match wrote to out-of-scope path; reverted by Phase 1 ledger

### Note 2

- session `c6a303c4`

10-byte mismatch on eConvexHullShape::ePolyFace::Write. The diff is purely a callee-saved register naming swap: original allocates s0=wb, s1=this (matching the sister function ePathPoint::Write); our SNC build allocates s0=this, s1=wb. This affects 6 instructions (the 2 prologue moves and 4 'move a0,sX' delay-slot setups), worth 10 differing bytes. Tried: (1) straight 4-field struct with wb.Write(b0..b3); (2) wb aliased to local cWriteBlock& reference + pointer cast for `this`; (3) for-loop over `((unsigned char*)this)[i]` (compiler tightened to 88B — wrong size). Permuter ran 135s @ sched=2 and 180s @ sched=1 (4368 candidates, 2918 compiled), zero improvement. NOT a bnel issue (10-byte diff with all instructions being plain `move`s, no branch-likely opcodes). Root cause hypothesis: SNC's saved-register coloring picks based on first-use position; in matched ePathPoint::Write the first call `wb.Write(3, pos)` references wb-receiver-then-this-via-pos, so wb wins s0; in this function each call references this (via lbu) before wb (via move-in-delay-slot), so this gets s0. No source restructure I tried changes that order without altering the function's structural shape (size or call count). Read function MATCHED in same file. The Write needs either a future SNC heuristic discovery or a structural source pattern that forces wb to be touched before any this-byte load.


## Disassembly

```asm
72950:	27bdfff0 	addiu	sp,sp,-16
72954:	afb00000 	sw	s0,0(sp)
72958:	afb10004 	sw	s1,4(sp)
7295c:	afbf0008 	sw	ra,8(sp)
72960:	00808825 	move	s1,a0
72964:	00a08025 	move	s0,a1
72968:	92250000 	lbu	a1,0(s1)
7296c:	0c00154f 	jal	0x553c
72970:	02002025 	move	a0,s0
72974:	92250001 	lbu	a1,1(s1)
72978:	0c00154f 	jal	0x553c
7297c:	02002025 	move	a0,s0
72980:	92250002 	lbu	a1,2(s1)
72984:	0c00154f 	jal	0x553c
72988:	02002025 	move	a0,s0
7298c:	92250003 	lbu	a1,3(s1)
72990:	0c00154f 	jal	0x553c
72994:	02002025 	move	a0,s0
72998:	8fb00000 	lw	s0,0(sp)
7299c:	8fb10004 	lw	s1,4(sp)
729a0:	8fbf0008 	lw	ra,8(sp)
729a4:	03e00008 	jr	ra
729a8:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eConvexHullShape::ePolyFace::Write(cWriteBlock &) const
// Address: 0x00072950, Size: 92B
// Obj: eAll_psp.obj

? cWriteBlock__Write_unsignedchar(?, u8);           /* extern */

void eConvexHullShape__ePolyFace__Write_cWriteBlockrefconst(void *arg0, ? arg1) {
    cWriteBlock__Write_unsignedchar(arg1, arg0->unk0);
    cWriteBlock__Write_unsignedchar(arg1, arg0->unk1);
    cWriteBlock__Write_unsignedchar(arg1, arg0->unk2);
    cWriteBlock__Write_unsignedchar(arg1, arg0->unk3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
