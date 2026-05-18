# Battle Packet: `gcSurface::AssignCopy(const cBase *)`

## Target

- Address: `0x00289f4c`
- Size: `236` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcSurface`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `186`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; correct function size; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00009d70` `matched` 84B `cObject::operator=(const cObject &)`
- `0x000d61d0` `matched` 68B `gcEvent::operator=(const gcEvent &)`
- `0x0022739c` `untried` 480B `dcast dcast<gcSurface *>(const cBase *)`

## Placement

- Canonical source: `src/gcSurface.cpp`
- Header(s): `include/gcSurface.h`
- Allowed source(s): `src/gcSurface.cpp`
- Split-TU prefix: `src/gcSurface_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcSurface.h`

Not found.

## Matched Same-Class Neighbors

- `0x00137ce4` 236B `gcSurface::Write(cFile &) const` — `src/gcSurface.cpp`
- `0x00137fc8` 152B `gcSurface::gcSurface(cBase *)` — `src/gcSurface.cpp`
- `0x0028a038` 124B `gcSurface::New(cMemPool *, cBase *) static` — `src/gcSurface.cpp`
- `0x0028a0b4` 360B `gcSurface::GetType(void) const` — `src/gcSurface.cpp`
- `0x0028a21c` 184B `gcSurface::~gcSurface(void)` — `src/gcSurface.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcPartialEntityController::AssignCopy(const cBase *)`

- Address: `0x002a0878`
- Size: `236` bytes
- Source: `src/gcPartialEntityController.cpp`

```cpp
//   0x002a0878 gcPartialEntityController::AssignCopy(const cBase *)
//   0x002a0964 gcPartialEntityController::New(cMemPool *, cBase *) static
//   0x002a09e0 gcPartialEntityController::GetType(void) const

inline void *operator new(unsigned int, void *p) { return p; }
```

### Exemplar 2: `gcPlayer::AssignCopy(const cBase *)`

- Address: `0x002610a0`
- Size: `240` bytes
- Source: `src/gcPlayer.cpp`

```cpp
// gcPlayer::AssignCopy(const cBase *)  @ 0x002610a0, 240B
// -----------------------------------------------------------------------------
struct gcPlayer_half3 {
    short a;
    short b;
    short c;
};
```

### Exemplar 3: `gcGeomCurveController::AssignCopy(const cBase *)`

- Address: `0x002c6ef4`
- Size: `240` bytes
- Source: `src/gcGeomCurveController.cpp`

```cpp
void gcGeomCurveController::AssignCopy(const cBase *base) {
    gcGeomCurveController *other = dcast<gcGeomCurveController>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 0xC) = *(int *)((char *)other + 0xC);
    *(float *)((char *)this + 0x10) = *(float *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);

    int i = 0;
    gcc_half3 *dst = (gcc_half3 *)((char *)this + 0x1C);
    gcc_half3 *src = (gcc_half3 *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    do {
        short a = src->a;
        short b = src->b;
        short c = src->c;
        __asm__ volatile("" ::: "memory");
        dst->a = a;
        dst->b = b;
        dst->c = c;
        i += 1;
        dst += 1;
        src += 1;
    } while (i <= 0);

    int *src24 = (int *)((char *)other + 0x24);
    int value24 = *src24;
    int *dst24 = (int *)((char *)this + 0x24);
    int *src28 = (int *)((char *)other + 0x28);
    int *dst28 = (int *)((char *)this + 0x28);
    *dst24 = value24;
    int value28 = *src28;
    *dst28 = value28;
    *(unsigned short *)((char *)this + 0x2C) =
        *(unsigned short *)((char *)other + 0x2C);
    *(short *)((char *)this + 0x2E) = *(short *)((char *)other + 0x2E);

    int j = 0;
    gcc_half3 *dst2 = (gcc_half3 *)((char *)this + 0x30);
    gcc_half3 *src2 = (gcc_half3 *)((char *)other + 0x30);
    do {
        short a = src2->a;
        short b = src2->b;
        short c = src2->c;
        __asm__ volatile("" ::: "memory");
        dst2->a = a;
        dst2->b = b;
        dst2->c = c;
        j += 1;
        dst2 += 1;
        src2 += 1;
    } while (j <= 0);
}
```

### Exemplar 4: `gcParticleSystemController::AssignCopy(const cBase *)`

- Address: `0x002d5bac`
- Size: `240` bytes
- Source: `src/gcParticleSystemController.cpp`

```cpp
void gcParticleSystemController::AssignCopy(const cBase *base) {
    gcParticleSystemController *other = dcast<gcParticleSystemController>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 0xC) = *(int *)((char *)other + 0xC);
    *(float *)((char *)this + 0x10) = *(float *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);

    int i = 0;
    gpsc_half3 *dst = (gpsc_half3 *)((char *)this + 0x1C);
    gpsc_half3 *src = (gpsc_half3 *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    do {
        short a = src->a;
        short b = src->b;
        short c = src->c;
        __asm__ volatile("" ::: "memory");
        dst->a = a;
        dst->b = b;
        dst->c = c;
        i += 1;
        dst += 1;
        src += 1;
    } while (i <= 0);

    int *src24 = (int *)((char *)other + 0x24);
    int value24 = *src24;
    int *dst24 = (int *)((char *)this + 0x24);
    int *src28 = (int *)((char *)other + 0x28);
    int *dst28 = (int *)((char *)this + 0x28);
    *dst24 = value24;
    int value28 = *src28;
    *dst28 = value28;
    *(unsigned short *)((char *)this + 0x2C) =
        *(unsigned short *)((char *)other + 0x2C);
    *(short *)((char *)this + 0x2E) = *(short *)((char *)other + 0x2E);

    int j = 0;
    gpsc_half3 *dst2 = (gpsc_half3 *)((char *)this + 0x30);
    gpsc_half3 *src2 = (gpsc_half3 *)((char *)other + 0x30);
    do {
        short a = src2->a;
        short b = src2->b;
        short c = src2->c;
        __asm__ volatile("" ::: "memory");
        dst2->a = a;
        dst2->b = b;
        dst2->c = c;
        j += 1;
        dst2 += 1;
        src2 += 1;
    } while (j <= 0);
}
```


## Prior Failure Notes

### Note 1

- session `3475be73`

Improved gcSurface::AssignCopy from the prior 12-byte mismatch to an 8-byte mismatch, but did not get a byte-exact match. The remaining differences are register allocation only, not branch-likely behavior: around the 0x4C/0x50 scalar copy cluster the target uses src4c in a2, then loads the 0x50 value into a1 and stores through a0; the current best still uses a1/a0/a1 for those roles. Around the 0x68 copy, the current best now matches the target schedule by hoisting the first gcEvent destination addiu before the store, but the loaded value and destination address remain swapped: current emits lw a1 / dst a2 / sw a1, target needs lw a2 / dst a1 / sw a2.

Tried restoring direct raw-offset copies, keeping src/dst live together, named register variables for src4c/src50/dst50/value68/dst68, delayed register assignments, tied zero-instruction asm outputs, removing and reordering barriers, and hoisting the first gcEvent assignment into the 0x68 copy block. Direct copies collapsed to a 212-byte function; tied asm outputs expanded live ranges into saved registers; other variants regressed to 12-22 byte mismatches. Ran tools/permuter.py for 300 seconds from the 8-byte baseline with --save-best: 7,296 candidates generated, 2,643 compiled, zero improvements. A future retry should focus on finding a C live-range shape that swaps a1/a2 in the 0x68 store without losing the now-correct event addiu schedule, and separately on forcing the 0x4C source address into a2 without moving the destination addiu early.

### Note 2

- session `239a81e4`

Best source compiles to the correct 236-byte size but remains 12 bytes different. The remaining mismatch is not a branch-likely issue: it is confined to register allocation and scheduling in the scalar copy cluster around offsets 0x4C/0x50/0x68. The target uses address temporaries like addiu a2,s1,0x4C and value registers a1/a2 for later stores; the best C version still chooses a1/a0 in those spots or moves one address calculation across the store. All calls, the prolog/epilog, FPU copies, byte copy, and five gcEvent::operator= calls are aligned.

Tried direct raw offset stores, explicit pointer temporaries, zero-instruction register constraints, memory-clobber constraints, reordered source/destination pointer creation, a typed class layout, and wrapper word assignment. The typed variants regressed to the compact 204-byte form or emitted an unwanted helper symbol. Running tools/permuter.py for 300 seconds generated 7,992 candidates / 3,714 compiled candidates with zero improvement from the 12-byte baseline. A manual sched=1 compile was worse at 28 bytes different, so this should stay sched=2. A retry should focus specifically on preserving a0 for the 0x48 value while forcing the 0x4C source into a2, then forcing the 0x50 loaded value into a1 and the 0x68 loaded value into a2 without adding real instructions.


## Disassembly

```asm
289f4c:	27bdfff0 	addiu	sp,sp,-16
289f50:	afb00000 	sw	s0,0(sp)
289f54:	00808025 	move	s0,a0
289f58:	afb10004 	sw	s1,4(sp)
289f5c:	afbf0008 	sw	ra,8(sp)
289f60:	0c089ce7 	jal	0x22739c
289f64:	00a02025 	move	a0,a1
289f68:	00408825 	move	s1,v0
289f6c:	02002025 	move	a0,s0
289f70:	0c00275c 	jal	0x9d70
289f74:	02202825 	move	a1,s1
289f78:	26240044 	addiu	a0,s1,68
289f7c:	8c840000 	lw	a0,0(a0)
289f80:	26050044 	addiu	a1,s0,68
289f84:	aca40000 	sw	a0,0(a1)
289f88:	8e240048 	lw	a0,72(s1)
289f8c:	2626004c 	addiu	a2,s1,76
289f90:	ae040048 	sw	a0,72(s0)
289f94:	8cc40000 	lw	a0,0(a2)
289f98:	2605004c 	addiu	a1,s0,76
289f9c:	26260050 	addiu	a2,s1,80
289fa0:	aca40000 	sw	a0,0(a1)
289fa4:	8cc50000 	lw	a1,0(a2)
289fa8:	26040050 	addiu	a0,s0,80
289fac:	ac850000 	sw	a1,0(a0)
289fb0:	c62c0054 	lwc1	$f12,84(s1)
289fb4:	e60c0054 	swc1	$f12,84(s0)
289fb8:	c62c0058 	lwc1	$f12,88(s1)
289fbc:	e60c0058 	swc1	$f12,88(s0)
289fc0:	9224005c 	lbu	a0,92(s1)
289fc4:	a204005c 	sb	a0,92(s0)
289fc8:	c62c0060 	lwc1	$f12,96(s1)
289fcc:	e60c0060 	swc1	$f12,96(s0)
289fd0:	c62c0064 	lwc1	$f12,100(s1)
289fd4:	26240068 	addiu	a0,s1,104
289fd8:	e60c0064 	swc1	$f12,100(s0)
289fdc:	8c860000 	lw	a2,0(a0)
289fe0:	26050068 	addiu	a1,s0,104
289fe4:	2604006c 	addiu	a0,s0,108
289fe8:	aca60000 	sw	a2,0(a1)
289fec:	0c035874 	jal	0xd61d0
289ff0:	2625006c 	addiu	a1,s1,108
289ff4:	26040088 	addiu	a0,s0,136
289ff8:	0c035874 	jal	0xd61d0
289ffc:	26250088 	addiu	a1,s1,136
28a000:	260400a4 	addiu	a0,s0,164
28a004:	0c035874 	jal	0xd61d0
28a008:	262500a4 	addiu	a1,s1,164
28a00c:	260400c0 	addiu	a0,s0,192
28a010:	0c035874 	jal	0xd61d0
28a014:	262500c0 	addiu	a1,s1,192
28a018:	260400dc 	addiu	a0,s0,220
28a01c:	0c035874 	jal	0xd61d0
28a020:	262500dc 	addiu	a1,s1,220
28a024:	8fb00000 	lw	s0,0(sp)
28a028:	8fb10004 	lw	s1,4(sp)
28a02c:	8fbf0008 	lw	ra,8(sp)
28a030:	03e00008 	jr	ra
28a034:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcSurface::AssignCopy(const cBase *)
// Address: 0x00289f4c, Size: 236B
// Obj: gcAll_psp.obj

? cObject__operator_eq_constcObjectref(void *, void *); /* extern */
void *dcastdcast_gcSurfaceptr__constcBaseptr(?);    /* extern */
? gcEvent__operator_eq_constgcEventref(void *, void *, void *, s32); /* extern */

void gcSurface__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 temp_a2;
    void *temp_f12;
    void *temp_v0;

    temp_v0 = dcastdcast_gcSurfaceptr__constcBaseptr(arg1);
    cObject__operator_eq_constcObjectref(arg0, temp_v0);
    arg0->unk44 = (s32) temp_v0->unk44;
    arg0->unk48 = (s32) temp_v0->unk48;
    arg0->unk4C = (s32) temp_v0->unk4C;
    arg0->unk50 = (s32) temp_v0->unk50;
    arg0->unk54 = (f32) temp_v0->unk54;
    arg0->unk58 = (f32) temp_v0->unk58;
    arg0->unk5C = (u8) temp_v0->unk5C;
    arg0->unk60 = (f32) temp_v0->unk60;
    temp_f12 = temp_v0->unk64;
    arg0->unk64 = temp_f12;
    temp_a2 = temp_v0->unk68;
    arg0->unk68 = temp_a2;
    gcEvent__operator_eq_constgcEventref(temp_f12, arg0 + 0x6C, temp_v0 + 0x6C, temp_a2);
    gcEvent__operator_eq_constgcEventref(arg0 + 0x88, temp_v0 + 0x88);
    gcEvent__operator_eq_constgcEventref(arg0 + 0xA4, temp_v0 + 0xA4);
    gcEvent__operator_eq_constgcEventref(arg0 + 0xC0, temp_v0 + 0xC0);
    gcEvent__operator_eq_constgcEventref(arg0 + 0xDC, temp_v0 + 0xDC);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
