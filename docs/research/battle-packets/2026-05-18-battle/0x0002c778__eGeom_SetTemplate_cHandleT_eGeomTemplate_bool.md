# Battle Packet: `eGeom::SetTemplate(cHandleT<eGeomTemplate>, bool)`

## Target

- Address: `0x0002c778`
- Size: `156` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eGeom`
- Method family: `SetTemplate`
- Leaf: `False`

## Queue Metadata

- score: `163`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; manageable size`
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
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

Callers:
- `0x0003dac0` `untried` 532B `eRoom::Reset(cMemPool *, bool)`
- `0x0005cd40` `failed` 1048B `eSky::Read(cFile &, cMemPool *)`
- `0x000a1264` `failed` 1336B `eRoom::Read(cFile &, cMemPool *)`
- `0x00113d50` `failed` 1456B `gcEntity::Read(cFile &, cMemPool *)`
- `0x00136f68` `failed` 508B `gcStaticInstance::Read(cFile &, cMemPool *)`
- `0x00137490` `untried` 1212B `gcStaticInstance::Reset(cMemPool *, bool)`
- `0x00171e80` `untried` 4952B `gcEntity::Reset(cMemPool *, bool)`

## Placement

- Canonical source: `src/eGeom.cpp`
- Header(s): `include/eGeom.h`
- Allowed source(s): `src/eGeom.cpp`
- Split-TU prefix: `src/eGeom_*.cpp`

## Class Header: `include/eGeom.h`

Not found.

## Matched Same-Class Neighbors

- `0x0002c498` 40B `eGeom::Write(cFile &) const` — `src/eGeom.cpp`
- `0x0002cb20` 184B `eGeom::UpdateLocalToWorld(void)` — `src/eGeom.cpp`
- `0x001e1da4` 152B `eGeom::GetType(void) const` — `src/eGeom.cpp`
- `0x001e1e3c` 8B `eGeom::CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const` — `src/eGeom.cpp`
- `0x001e1e44` 8B `eGeom::CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const` — `src/eGeom.cpp`

## Prior Failure Notes

### Note 1

- session `c0c40dde`

Retried the register-allocation problem from a different angle. Tested local register variables for the handle/reset swap, explicit inline-asm prologue variants, a volatile stored handle, a static inline helper with logical argument order (this, reset, handle), and memory-barrier/source-order variants. The only C-clean output still keeps the handle in a1 and reset in a2, producing a 152-byte function where expected is 156 bytes: expected starts with sw a1,0(sp); andi a1,a2,0xff; lw a2,0(sp), while SNC keeps sw/lw in a1 and places andi a2,a2,0xff in a later delay slot. Inline-asm variants could force parts of the prologue but then SNC saved registers in the wrong locations or duplicated the bool mask, increasing the diff substantially, so they were reverted.

Root cause still appears to be SNC argument/register allocation around the by-value cHandleT plus bool. The table lookup and virtual-call tail are structurally correct, but the early a1/a2 swap never emerges from clean C. This is not a bnel/unmatchable case: the mismatch is register allocation, instruction count, and scheduling, not a <=8-byte branch-likely divergence.

### Note 2

- session `77caca94`

Stuck at ~41/156-byte diff, with one variant producing a size mismatch (compiled to 156B vs expected 152B from .sym — DB metadata says 156, sym says 152, suggesting the function was actually 38 instructions not 39). The fundamental divergence is register allocation: expected does `andi a1,a2,0xff` early to move the bool into a1 plus `lw a2,0(sp)` to bring the spilled handle into a2; my output keeps the handle in a1 and the bool in a2. This swap propagates through the rest of the function (different temp register choices for templPtr, swapped operands on `addu`). Tried: (1) reordering the if/else around `templ.mIndex == 0`; (2) capturing `bool r = reset` early; (3) capturing `int h = *(int *)&templ` early; (4) inline pointer-cast access vs local var; (5) `unsigned int` vs `int` for handle width; (6) struct vs class for cHandleT; (7) renaming `mIndex` to `handle`; (8) shape-equivalent rewrites of the virtual-call block. None triggered the desired register pre-swap. Permuter is not usable here — tools/permuter.py uses BASE_CFLAGS without `-Xvfpumatrix=1 -Xvfpuscalar=8` so eGeom.cpp does not compile under it (reports 'Original source does not compile'). Does NOT meet `unmatchable_bnel` criteria (norm requires ≤8-byte diff confirmed as bnel-vs-bne+nop); 41-byte diff is a register-allocation/ordering issue not a branch-likely issue. Likely root cause: expected was compiled with slightly different SNC inlining or argument-pre-spill heuristics that this session could not reverse-engineer from the C++ side. Leaving as `failed` for retry with a different approach (possibly different cHandleT layout, or hoist the spill/reload via a deliberately written temporary).


## Disassembly

```asm
2c778:	27bdfff0 	addiu	sp,sp,-16
2c77c:	afa50000 	sw	a1,0(sp)
2c780:	30c500ff 	andi	a1,a2,0xff
2c784:	8fa60000 	lw	a2,0(sp)
2c788:	afb00004 	sw	s0,4(sp)
2c78c:	afb10008 	sw	s1,8(sp)
2c790:	afbf000c 	sw	ra,12(sp)
2c794:	14c00003 	bnez	a2,0x2c7a4
2c798:	30c7ffff 	andi	a3,a2,0xffff
2c79c:	1000000c 	b	0x2c7d0
2c7a0:	34070000 	li	a3,0x0
2c7a4:	3c080004 	lui	t0,0x4
2c7a8:	00073880 	sll	a3,a3,0x2
2c7ac:	25088890 	addiu	t0,t0,-30576
2c7b0:	00e83821 	addu	a3,a3,t0
2c7b4:	8ce80000 	lw	t0,0(a3)
2c7b8:	11000005 	beqz	t0,0x2c7d0
2c7bc:	34070000 	li	a3,0x0
2c7c0:	8d090030 	lw	t1,48(t0)
2c7c4:	15260002 	bne	t1,a2,0x2c7d0
2c7c8:	00000000 	nop
2c7cc:	01003825 	move	a3,t0
2c7d0:	10a0000b 	beqz	a1,0x2c800
2c7d4:	ac870060 	sw	a3,96(a0)
2c7d8:	8c860004 	lw	a2,4(a0)
2c7dc:	24d00038 	addiu	s0,a2,56
2c7e0:	86060000 	lh	a2,0(s0)
2c7e4:	0c0011e8 	jal	0x47a0
2c7e8:	00868821 	addu	s1,a0,a2
2c7ec:	8e070004 	lw	a3,4(s0)
2c7f0:	02202025 	move	a0,s1
2c7f4:	00402825 	move	a1,v0
2c7f8:	00e0f809 	jalr	a3
2c7fc:	34060001 	li	a2,0x1
2c800:	8fb00004 	lw	s0,4(sp)
2c804:	8fb10008 	lw	s1,8(sp)
2c808:	8fbf000c 	lw	ra,12(sp)
2c80c:	03e00008 	jr	ra
2c810:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eGeom::SetTemplate(cHandleT<eGeomTemplate>, bool)
// Address: 0x0002c778, Size: 156B
// Obj: eAll_psp.obj

s32 cMemPool__GetPoolFromPtr_constvoidptrstatic(s32, s16, void *); /* extern */

void eGeom__SetTemplate_cHandleT_eGeomTemplate__bool(void *arg0, s32 arg1, s32 arg2) {
    ? (*temp_a3)(void *, s32, ?, ?);
    s16 temp_a2_2;
    s32 temp_a1;
    void *temp_a2;
    void *temp_t0;
    void *var_a3;

    temp_a1 = arg2 & 0xFF;
    if (subroutine_arg0 == 0) {
        var_a3 = NULL;
    } else {
        temp_t0 = ((subroutine_arg0 & 0xFFFF) * 4)->unk38890;
        var_a3 = NULL;
        if ((temp_t0 != NULL) && (temp_t0->unk30 == subroutine_arg0)) {
            var_a3 = temp_t0;
        }
    }
    arg0->unk60 = var_a3;
    if (temp_a1 != 0) {
        temp_a2 = arg0->unk4;
        temp_a2_2 = temp_a2->unk38;
        temp_a3 = (temp_a2 + 0x38)->unk4;
        temp_a3(arg0 + temp_a2_2, cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_a1, temp_a2_2, var_a3), 1, temp_a3);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
