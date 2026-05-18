# Battle Packet: `mBox::XformInv(const mBox &, const mOCS &)`

## Target

- Address: `0x001a0af8`
- Size: `368` bytes
- Status: `failed`
- Object: `mAll_psp.obj`
- Class: `mBox`
- Method family: `XformInv`
- Leaf: `False`

## Queue Metadata

- score: `148`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 13B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x001a0928` `matched` 260B `mBox::GetCorners(mVec3 *) const`

Callers:
- `0x001ff334` `untried` 1768B `eCollisionIterator<eBoxCullCollisionIterator, 0>::TraverseHeightmapShape(const eGeom *, const eHeightmapTemplate *, const eHeightmapShape *, const mOCS &, mVolume::mContainment)`
- `0x001ffa1c` `untried` 1840B `eCollisionIterator<eBoxCullCollisionIterator, 0>::TraverseMeshShape(const eGeom *, const eMeshShape *, const mOCS &, mVolume::mContainment, unsigned short, bool)`
- `0x0020014c` `untried` 1332B `eCollisionIterator<eBoxCullCollisionIterator, 0>::TraverseDynamicModel(const eGeom *, unsigned int, mVolume::mContainment)`

## Placement

- Canonical source: `src/mBox.cpp`
- Header(s): `include/mBox.h`
- Allowed source(s): `src/mBox.cpp`
- Split-TU prefix: `src/mBox_*.cpp`

## Class Header: `include/mBox.h`

```cpp
#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3;

// mBox: axis-aligned bounding box (32 bytes, 0x20)
// Confirmed from mBox::Extend (0x001a0a74): quadword accesses at offsets 0x00 and 0x10
// Two mVec3 members: min and max corners
struct mBox {
    SceULong128 min;  // offset 0x00 — mVec3
    SceULong128 max;  // offset 0x10 — mVec3

    void Extend(int, const mVec3 *, int);
    void Extend(const mBox &);
    void GetCorners(mVec3 *) const;
    void XformInv(const mBox &, const struct mOCS &);
};
```

## Matched Same-Class Neighbors

- `0x001a0928` 260B `mBox::GetCorners(mVec3 *) const` — `src/mBox.cpp`
- `0x001a0a2c` 72B `mBox::Extend(int, const mVec3 *, int)` — `src/mBox.cpp`
- `0x001a0a74` 132B `mBox::Extend(const mBox &)` — `src/mBox.cpp`

## Prior Failure Notes

### Note 1

- session `29e039dd`

Improved mBox::XformInv from the prior 21/368 byte mismatch to 13/368 bytes by introducing a self local exactly between the fmin bit-pattern construction and the fmin mtc1, then using that local for all this-based VFPU stores. This gets the prologue cluster to match the expected move s1,a0; mtc1 f20; andi invalid ordering. The remaining mismatches are still scheduling-only: the non-invalid path wants li a0,0 before the second initial sv.q and li a1,0 after it, while SNC keeps sv.q then li a1 then li a0; the loop tail wants addiu a1,a1,1 before the final max sv.q, while SNC keeps sv.q before the increment.

Manual retries included splitting the final loop sv.q out of the VFPU asm block, keeping the corner pointer live to prevent pointer increment hoisting, pointer-iterator loop form, and splitting the second initial invalid sv.q with a live zero temp. These either produced identical bytes or regressed by moving the pointer increment before sv.q and leaving a nop in the branch delay slot. Ran tools/permuter.py for 300s from the improved source: 7512 candidates generated, 4127 compiled, zero improvements (permuter raw counter stayed at 17 bytes; compare_func reports 13 bytes). This is not a bnel issue; no branch-likely form differs.

### Note 2

- session `5fffc620`

Improved from 65/368 to 21/368 bytes diff but could not fully match. Made three key fixes: (1) Switched initial other.min/other.max loads from inline asm to C-level v4sf_t loads (`v4sf_t vomin = *(const v4sf_t*)&other.min`). This let SNC schedule the lv.q instructions BEFORE the stack-save prologue, matching the expected layout (lv.q at 0x04/0x08 before the swc1/sw block). Fixed 7 instructions (28 bytes). (2) Split the big volatile asm block into small per-instruction blocks for the fmax/fmin construction and the invalid-flag andi/sltu/andi chain. Using a single `int t` temp variable for both fmax and fmin bit constructions let the compiler reuse t0 for both (matching expected register allocation instead of using t0+t1). Fixed 6 instructions (24 bytes). (3) Used `x` constraint for v4sf_t values passed to the vcmp.t asm (SNC supports this — allocates C120/C130).

Remaining 21 bytes diff = 4 scheduling disagreements that permuter could not solve (ran 2x 240s, 0x08 candidates, no further improvement): (A) 0x48-0x50: `move s1,a0` placement — expected schedules it between the ori and mtc1 of fmin construction; our SNC places it AFTER mtc1+andi. (B) 0x9c-0xa4: `li a0,0; sv.q C120,0x10(s1); li a1,0` vs our `sv.q; li a1; li a0` — expected interleaves `li a0,0` (for the b-delay-slot addu) before the second invalid-init sv.q; ours puts both `li`s after. Splitting the mtv+sv.q asm block did not help the compiler schedule the `li` earlier. (C) 0x140-0x144: `addiu a1,a1,1; sv.q` vs our `sv.q; addiu` — expected schedules i++ before the final max store in loop body; ours puts it after. (D) 0x60 is the jal-to-GetCorners relocation and is already masked by compare_func.

These remaining diffs are all compiler scheduling choices — where SNC places independent scalar ops (li, move, addiu) relative to VFPU stores (sv.q) and ALU ops (andi, mtc1). Every block with a diff has correct semantics and correct instruction counts; it's only position that differs. Does NOT meet criteria for unmatchable_bnel (no bnel/beql instructions involved, diff >8 bytes). Retry strategy for next agent: (i) try pointer-iterator loop form for corners (`mVec3 *c = corners; do { ...; c++; } while(++i<8);`) to influence loop scheduling at 0x140. (ii) try producing the mtv/sv.q sequences via a custom inline asm macro that the compiler can partially schedule around. (iii) verify whether `__asm__ volatile("")` barriers with different input operand lists can coax the move s1,a0 earlier. Current best source is saved.


## Disassembly

```asm
1a0af8:	27bdff60 	addiu	sp,sp,-160
1a0afc:	d8a60000 	lv.q        C120, 0x0($a1)
1a0b00:	d8a70010 	lv.q        C130, 0x10($a1)
1a0b04:	e7b40080 	swc1	$f20,128(sp)
1a0b08:	e7b60084 	swc1	$f22,132(sp)
1a0b0c:	afb00088 	sw	s0,136(sp)
1a0b10:	afb1008c 	sw	s1,140(sp)
1a0b14:	afbf0090 	sw	ra,144(sp)
1a0b18:	6c078607 	vcmp.t      gt, C120, C130
1a0b1c:	ffff0000 	vnop
1a0b20:	48670083 	mfvc        $a3, $131
1a0b24:	3c087f7f 	lui	t0,0x7f7f
1a0b28:	3508ffff 	ori	t0,t0,0xffff
1a0b2c:	4488b000 	mtc1	t0,$f22
1a0b30:	30e70020 	andi	a3,a3,0x20
1a0b34:	3c08ff7f 	lui	t0,0xff7f
1a0b38:	0007382b 	sltu	a3,zero,a3
1a0b3c:	3508ffff 	ori	t0,t0,0xffff
1a0b40:	00808825 	move	s1,a0
1a0b44:	4488a000 	mtc1	t0,$f20
1a0b48:	30e700ff 	andi	a3,a3,0xff
1a0b4c:	00a02025 	move	a0,a1
1a0b50:	14e00015 	bnez	a3,0x1a0ba8
1a0b54:	00c08025 	move	s0,a2
1a0b58:	0c06824a 	jal	0x1a0928
1a0b5c:	03a02825 	move	a1,sp
1a0b60:	4404b000 	mfc1	a0,$f22
1a0b64:	4405b000 	mfc1	a1,$f22
1a0b68:	4406b000 	mfc1	a2,$f22
1a0b6c:	48e40006 	mtv         $a0, S120
1a0b70:	48e50026 	mtv         $a1, S121
1a0b74:	48e60046 	mtv         $a2, S122
1a0b78:	fa260000 	sv.q        C120, 0x0($s1)
1a0b7c:	4404a000 	mfc1	a0,$f20
1a0b80:	4405a000 	mfc1	a1,$f20
1a0b84:	4406a000 	mfc1	a2,$f20
1a0b88:	48e40006 	mtv         $a0, S120
1a0b8c:	48e50026 	mtv         $a1, S121
1a0b90:	48e60046 	mtv         $a2, S122
1a0b94:	34040000 	li	a0,0x0
1a0b98:	fa260010 	sv.q        C120, 0x10($s1)
1a0b9c:	34050000 	li	a1,0x0
1a0ba0:	10000016 	b	0x1a0bfc
1a0ba4:	03a42021 	addu	a0,sp,a0
1a0ba8:	4404b000 	mfc1	a0,$f22
1a0bac:	4405b000 	mfc1	a1,$f22
1a0bb0:	4406b000 	mfc1	a2,$f22
1a0bb4:	48e40006 	mtv         $a0, S120
1a0bb8:	48e50026 	mtv         $a1, S121
1a0bbc:	48e60046 	mtv         $a2, S122
1a0bc0:	fa260000 	sv.q        C120, 0x0($s1)
1a0bc4:	4404a000 	mfc1	a0,$f20
1a0bc8:	4405a000 	mfc1	a1,$f20
1a0bcc:	4406a000 	mfc1	a2,$f20
1a0bd0:	48e40006 	mtv         $a0, S120
1a0bd4:	48e50026 	mtv         $a1, S121
1a0bd8:	48e60046 	mtv         $a2, S122
1a0bdc:	fa260010 	sv.q        C120, 0x10($s1)
1a0be0:	c7b40080 	lwc1	$f20,128(sp)
1a0be4:	c7b60084 	lwc1	$f22,132(sp)
1a0be8:	8fb00088 	lw	s0,136(sp)
1a0bec:	8fb1008c 	lw	s1,140(sp)
1a0bf0:	8fbf0090 	lw	ra,144(sp)
1a0bf4:	03e00008 	jr	ra
1a0bf8:	27bd00a0 	addiu	sp,sp,160
1a0bfc:	d8860000 	lv.q        C120, 0x0($a0)
1a0c00:	da000000 	lv.q        C000, 0x0($s0)
1a0c04:	da010010 	lv.q        C010, 0x10($s0)
1a0c08:	da020020 	lv.q        C020, 0x20($s0)
1a0c0c:	da030030 	lv.q        C030, 0x30($s0)
1a0c10:	60838606 	vsub.t      C120, C120, C030
1a0c14:	f1068007 	vtfm3.t     C130, M000, C120
1a0c18:	f8870000 	sv.q        C130, 0x0($a0)
1a0c1c:	d8860000 	lv.q        C120, 0x0($a0)
1a0c20:	da270000 	lv.q        C130, 0x0($s1)
1a0c24:	6d068706 	vmin.t      C120, C130, C120
1a0c28:	fa260000 	sv.q        C120, 0x0($s1)
1a0c2c:	d8860000 	lv.q        C120, 0x0($a0)
1a0c30:	da270010 	lv.q        C130, 0x10($s1)
1a0c34:	6d868706 	vmax.t      C120, C130, C120
1a0c38:	24a50001 	addiu	a1,a1,1
1a0c3c:	fa260010 	sv.q        C120, 0x10($s1)
1a0c40:	28a60008 	slti	a2,a1,8
1a0c44:	14c0ffed 	bnez	a2,0x1a0bfc
1a0c48:	24840010 	addiu	a0,a0,16
1a0c4c:	c7b40080 	lwc1	$f20,128(sp)
1a0c50:	c7b60084 	lwc1	$f22,132(sp)
1a0c54:	8fb00088 	lw	s0,136(sp)
1a0c58:	8fb1008c 	lw	s1,140(sp)
1a0c5c:	8fbf0090 	lw	ra,144(sp)
1a0c60:	03e00008 	jr	ra
1a0c64:	27bd00a0 	addiu	sp,sp,160
```

## m2c Starting Point

```c
// Decompiled: mBox::XformInv(const mBox &, const mOCS &)
// Address: 0x001a0af8, Size: 368B
// Obj: mAll_psp.obj

? mBox__GetCorners_mVec3ptrconst(?, void *, s32);   /* extern */

void mBox__XformInv_constmBoxref_constmOCSref(s32 arg0, ? arg1, ? arg2, s32 arg3) {
    s32 temp_a3;
    s32 var_a1;

    __asm__ volatile("lv.q C120, ($a1)");
    __asm__ volatile("lv.q C130, 0x10($a1)");
    __asm__ volatile("vcmp.t gt, C120, C130");
    __asm__ volatile("vnop");
    __asm__ volatile("mfvc $a3, $131");
    temp_a3 = ((arg3 & 0x20) != 0) & 0xFF;
    if (temp_a3 == 0) {
        mBox__GetCorners_mVec3ptrconst(arg1, sp, temp_a3);
        __asm__ volatile("mtv $a0, S120");
        __asm__ volatile("mtv $a1, S121");
        __asm__ volatile("mtv $a2, S122");
        __asm__ volatile("sv.q C120, ($s1)");
        __asm__ volatile("mtv $a0, S120");
        __asm__ volatile("mtv $a1, S121");
        __asm__ volatile("mtv $a2, S122");
        __asm__ volatile("sv.q C120, 0x10($s1)");
        var_a1 = 0;
        do {
            __asm__ volatile("lv.q C120, ($a0)");
            __asm__ volatile("lv.q C000, ($s0)");
            __asm__ volatile("lv.q C010, 0x10($s0)");
            __asm__ volatile("lv.q C020, 0x20($s0)");
            __asm__ volatile("lv.q C030, 0x30($s0)");
            __asm__ volatile("vsub.t C120, C120, C030");
            __asm__ volatile("vtfm3.t C130, M000, C120");
            __asm__ volatile("sv.q C130, ($a0)");
            __asm__ volatile("lv.q C120, ($a0)");
            __asm__ volatile("lv.q C130, ($s1)");
            __asm__ volatile("vmin.t C120, C130, C120");
            __asm__ volatile("sv.q C120, ($s1)");
            __asm__ volatile("lv.q C120, ($a0)");
            __asm__ volatile("lv.q C130, 0x10($s1)");
            __asm__ volatile("vmax.t C120, C130, C120");
            var_a1 += 1;
            __asm__ volatile("sv.q C120, 0x10($s1)");
        } while (var_a1 < 8);
        return;
    }
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, ($s1)");
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a1, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, 0x10($s1)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
