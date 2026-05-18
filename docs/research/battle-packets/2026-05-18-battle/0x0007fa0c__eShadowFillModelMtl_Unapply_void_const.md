# Battle Packet: `eShadowFillModelMtl::Unapply(void) const`

## Target

- Address: `0x0007fa0c`
- Size: `60` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eShadowFillModelMtl`
- Method family: `Unapply`
- Leaf: `True`

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
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/eShadowFillModelMtl.cpp`
- Header(s): `include/eShadowFillModelMtl.h`
- Allowed source(s): `src/eShadowFillModelMtl.cpp`
- Split-TU prefix: `src/eShadowFillModelMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eShadowFillModelMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x0007f1bc` 76B `eShadowFillModelMtl::Write(cFile &) const` — `src/eShadowFillModelMtl.cpp`
- `0x0007f208` 212B `eShadowFillModelMtl::Read(cFile &, cMemPool *)` — `src/eShadowFillModelMtl.cpp`
- `0x0007f2dc` 168B `eShadowFillModelMtl::PlatformRead(cFile &, cMemPool *)` — `src/eShadowFillModelMtl.cpp`
- `0x0007f384` 8B `eShadowFillModelMtl::PlatformFree(void)` — `src/eTextureMap.cpp`
- `0x0007f38c` 76B `eShadowFillModelMtl::eShadowFillModelMtl(cBase *)` — `src/eShadowFillModelMtl.cpp`

## Matched Method Exemplars

### Exemplar 1: `eSilhouetteModelMtl::Unapply(void) const`

- Address: `0x0008767c`
- Size: `72` bytes
- Source: `src/eSilhouetteModelMtl.cpp`

```cpp
void eSilhouetteModelMtl::Unapply(void) const {
    if (gSilhouetteGpuState.unk50 != 2) {
        int *p = gSilhouetteGpuDList.cursor;
        gSilhouetteGpuDList.cursor = (int *)((char *)p + 8);
        p[0] = 0x1D000001;
        p[1] = 0x9B000000;
        gSilhouetteGpuState.unk50 = 2;
    }
}
```

### Exemplar 2: `eStandardSpriteMtl::Unapply(void) const`

- Address: `0x00032f30`
- Size: `8` bytes
- Source: `src/eStandardSpriteMtl.cpp`

```cpp
void eStandardSpriteMtl::Unapply(void) const {
}
```

### Exemplar 3: `eProjectedModelMtl::Unapply(void) const`

- Address: `0x0007e714`
- Size: `8` bytes
- Source: `src/eTextureMap.cpp`

```cpp
void eProjectedModelMtl::Unapply(void) const {
}
```

### Exemplar 4: `eDynamicLightHeightmapMtl::Unapply(void) const`

- Address: `0x00080b9c`
- Size: `8` bytes
- Source: `src/eDynamicLightHeightmapMtl_Unapply.cpp`

```cpp
void eDynamicLightHeightmapMtl::Unapply(void) const {
}
```


## Prior Failure Notes

### Note 1

- session `ee9d1ec6`

Unapply remains at the same semantic implementation as the prior attempt and still compiles to a 13/60 byte mismatch. The live logic is correct, but SNC schedules the D_000984D0 address setup into the beqz delay slot and allocates the GE constant in a different register; expected instead hoists lui a1,0x2400 into the delay slot, then forms D_000984D0 in a fresh register. I reverified this state after the surrounding destructor edits; the mismatch is unchanged and isolated to the known scheduling/register-allocation divergence, not a source-level control-flow issue.

I did not re-run a long permuter pass for this function because the supplied prior session already tried the direct, hoisted-constant, variable-reuse, post-increment, and store-order variants plus sched=1/sched=2 permuter runs with no improvement. This still should not be classified unmatchable_bnel: the branch is not branch-likely, the diff is greater than the strict bnel threshold, and the problem is literal/address delay-slot selection.

### Note 2

- session `bb467d78`

Scheduling/register-allocation divergence between our SNC and the original compiler. Stuck at 13/60 bytes differ. The semantic code is correct and produces the same logic, but SNC fills the branch delay slot differently.

Expected (at 0x7fa0c) has `lui a1, 0x2400` (the upper half of the 0x24000000 GE command) hoisted into the beqz delay slot, then uses a fresh register a2 to form the D_000984D0 address (lui+addiu), and reuses a1 for three values (field_20 load, 0x24000000, then final 0). Our SNC (via wibo on pspsnc 1.2.7503.0) consistently fills the delay slot with the D_000984D0 HI16 lui instead — i.e. hoists the pointer address load rather than the constant — and then allocates a fresh register for 0x24000000 later. Register roles swap accordingly (ours: a1=D_984D0, a2=ptr, a3=0x24000000; expected: a1=const/0, a2=D_984D0, a3=ptr).

I confirmed this is not a sched-flag issue: -Xsched=0/1/2 all produce identical output for this function (only sched=0 leaves a nop in the delay slot; 1 and 2 both hoist the D_984D0 lui). -Xmopt=0..4 just swaps which global goes first (D_A vs D_B) without producing the expected pattern. The #pragma control sched=1 is applied and verified — other functions in this file match identically to their sched=2 and sched=1 outputs (they produce same bytes either way). A minimal standalone reproduction (/tmp/pragma_test2.cpp) confirms the divergence is independent of the surrounding class context.

Tried formulations (all produced 13+ byte diffs):
  - direct: `if (D_00098428.field_20 != 0) { ...; *p = 0x24000000; D_00098428.field_20 = 0; }` (4 bytes shorter — compiler folds `= 0` to `sw zero`)
  - variable for zero: `int f = D_A.field_20; ...; f = 0; D_A.field_20 = f;` (length matches, still 13 byte diff from delay-slot/regalloc swap)
  - hoisted constant: `int cmd = 0x24000000; int f = ...; *p = cmd;` (forces cmd into a1 in delay slot, but introduces a live-range for cmd that prevents reusing the same reg for the final `= 0`, growing the diff to 16 bytes)
  - variable reuse: `int tmp = field_20; ...; tmp = 0x24000000; *p = tmp; tmp = 0;` — SNC still chose D_984D0 for the delay slot
  - post-increment: `*D_B.ptr++ = 0x24000000` — same diff
  - store-before-ptr-update reordering — same diff

Ran tools/permuter.py for 150s at sched=2 (2926 compiled candidates, 0 improvements) and 90s at sched=1 (1698 candidates, 0 improvements). Permuter can't find a source mutation that produces the expected scheduling.

Root cause: this looks like a genuine compiler-version scheduling-heuristic divergence, analogous to the bnel divergence documented in docs/decisions/006. The original compiler preferred hoisting the literal-upper-half constant into the delay slot; our compiler prefers hoisting the multi-insn address formation. NOT classifiable as `unmatchable_bnel` because (1) the branch is bne/beq with nop, not bnel/beql, (2) diff is 13 bytes > 8 threshold, and (3) the divergence is in register allocation + scheduling, not branch-likely promotion.

For a retry agent: the matched sibling function eRenderTarget::Apply (at 0x317a4) compiled with identical source structure produces the OPPOSITE scheduling choice (D_984D0 in delay slot) and matches its expected — see the expected dump I generated at expected/build/func/000317a4.o. So the divergence is local to Unapply at 0x7fa0c. Possibly the expected was built with a slightly different SNC point-release than our pspsnc 1.2.7503.0, or there's a flag like -Xsched_heuristic that we haven't discovered. Worth checking if any other 60B leaf Mtl::Unapply functions in the codebase show the same pattern and were matched via some trick.


## Disassembly

```asm
7fa0c:	3c04000a 	lui	a0,0xa
7fa10:	24848428 	addiu	a0,a0,-31704
7fa14:	8c850020 	lw	a1,32(a0)
7fa18:	10a00009 	beqz	a1,0x7fa40
7fa1c:	3c052400 	lui	a1,0x2400
7fa20:	3c06000a 	lui	a2,0xa
7fa24:	24c684d0 	addiu	a2,a2,-31536
7fa28:	8cc70008 	lw	a3,8(a2)
7fa2c:	24e80004 	addiu	t0,a3,4
7fa30:	acc80008 	sw	t0,8(a2)
7fa34:	ace50000 	sw	a1,0(a3)
7fa38:	34050000 	li	a1,0x0
7fa3c:	ac850020 	sw	a1,32(a0)
7fa40:	03e00008 	jr	ra
7fa44:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eShadowFillModelMtl::Unapply(void) const
// Address: 0x0007fa0c, Size: 60B
// Obj: eAll_psp.obj

void eShadowFillModelMtl__Unapply_voidconst(void) {
    s32 *temp_a3;

    if ((void *)0x98428->unk20 != 0) {
        temp_a3 = (void *)0x984D0->unk8;
        (void *)0x984D0->unk8 = (s32 *) (temp_a3 + 4);
        *temp_a3 = 0x24000000;
        (void *)0x98428->unk20 = 0;
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
