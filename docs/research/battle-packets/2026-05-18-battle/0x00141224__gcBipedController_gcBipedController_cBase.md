# Battle Packet: `gcBipedController::gcBipedController(cBase *)`

## Target

- Address: `0x00141224`
- Size: `108` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcBipedController`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; Constructor retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `constant_cse_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `constant_cse_codegen`
- Near miss: `8`
- Summary: Compiler hoists/reuses constants differently from target.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x0013cb70` `matched` 52B `gcCreatureController::gcCreatureController(cBase *)`

Callers:
- `0x0015285c` `matched` 84B `gcPCBipedController::gcPCBipedController(cBase *)`
- `0x00154e78` `matched` 52B `gcNPCBipedController::gcNPCBipedController(cBase *)`

## Placement

- Canonical source: `src/gcBipedController.cpp`
- Header(s): `include/gcBipedController.h`
- Allowed source(s): `src/gcBipedController.cpp`
- Split-TU prefix: `src/gcBipedController_*.cpp`

## Compiler Guidance

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

## Class Header: `include/gcBipedController.h`

Not found.

## Matched Same-Class Neighbors

- `0x0014111c` 76B `gcBipedController::Write(cFile &) const` — `src/gcBipedController.cpp`
- `0x00141168` 188B `gcBipedController::Read(cFile &, cMemPool *)` — `src/gcBipedController.cpp`
- `0x00141290` 184B `gcBipedController::Update(cTimeValue)` — `src/gcBipedController.cpp`
- `0x0014200c` 80B `gcBipedController::OnDeselected(void)` — `src/gcBipedController.cpp`
- `0x00142154` 48B `gcBipedController::OnDeactivated(void)` — `src/gcBipedController.cpp`

## Prior Failure Notes

### Note 1

- session `d9a1284f`

Best result remains the existing source at 8/108 bytes different. The full body matches through the vtable store, zero-vector setup, VFPU store, and scalar stores except for the final tail order: expected has `mtc1 a0,$f12; move v0,s0; swc1 $f12,0xB8(s0)`, while SNC emits `mtc1 a0,$f12; swc1 $f12,0xB8(s0); move v0,s0`. The differing bytes are only that `move`/`swc1` swap.

Tried whole scalar tail as C, mixed asm/C using an `$f12` float output, explicit `$v0` moves with and without clobbers, bound-register operands intended to make `$v0` live across the final store, and single/two-block asm tails. These either returned to the same 8-byte mismatch or introduced duplicate loads/moves. The SNC permuter ran for 300 seconds from the 8-byte baseline, generated 9528 candidates, compiled 3740, and found zero improvements. Root cause still appears to be constructor return-value scheduling around an inline-asm-driven final float store.

### Note 2

- session `825a28f3`

Started from a normal C++ constructor body with split mfc1 temporaries and a VFPU mtv/sv.q block. That got the overall field order right but left ~30 bytes off because SNC hoisted mtc1 too early, chose a0/a1/a2 instead of a1/a2/a0 for the three mfc1 results, and scheduled the float tail differently. I then replaced the zero-vector setup with explicit inline asm for mtc1/mfc1/mtv/sv.q and iterated on the tail using plain C stores, a single asm tail block, and a split two-block tail. The best result is the current source: 8/108 bytes differ, with the body matching through the VFPU store and only the final two instructions swapped.

The remaining mismatch is at the tail after offset 0xB0. Expected is `lui a0,0x3f80; swc1 $f12,0xB4(s0); mtc1 a0,$f12; move v0,s0; swc1 $f12,0xB8(s0)`. Best current build emits `lui a0,0x3f80; swc1 $f12,0xB4(s0); mtc1 a0,$f12; swc1 $f12,0xB8(s0); move v0,s0`, so the only live diff is the `move v0,s0` versus final `swc1` ordering. I also tested bound-register attempts to make SNC treat `$f12` as a live C float, but those paths either had no effect or introduced worse codegen/spills. Root cause looks like SNC scheduling the implicit constructor return after an explicit tail store whenever that last store is driven by inline asm.


## Disassembly

```asm
141224:	27bdfff0 	addiu	sp,sp,-16
141228:	afb00000 	sw	s0,0(sp)
14122c:	afbf0004 	sw	ra,4(sp)
141230:	0c04f2dc 	jal	0x13cb70
141234:	00808025 	move	s0,a0
141238:	3c040039 	lui	a0,0x39
14123c:	2484bbd8 	addiu	a0,a0,-17448
141240:	44806000 	mtc1	zero,$f12
141244:	ae040004 	sw	a0,4(s0)
141248:	44056000 	mfc1	a1,$f12
14124c:	44066000 	mfc1	a2,$f12
141250:	a6000090 	sh	zero,144(s0)
141254:	44046000 	mfc1	a0,$f12
141258:	48e50006 	mtv         $a1, S120
14125c:	48e40026 	mtv         $a0, S121
141260:	48e60046 	mtv         $a2, S122
141264:	fa0600a0 	sv.q        C120, 0xA0($s0)
141268:	ae0000b0 	sw	zero,176(s0)
14126c:	3c043f80 	lui	a0,0x3f80
141270:	e60c00b4 	swc1	$f12,180(s0)
141274:	44846000 	mtc1	a0,$f12
141278:	02001025 	move	v0,s0
14127c:	e60c00b8 	swc1	$f12,184(s0)
141280:	8fb00000 	lw	s0,0(sp)
141284:	8fbf0004 	lw	ra,4(sp)
141288:	03e00008 	jr	ra
14128c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcBipedController::gcBipedController(cBase *)
// Address: 0x00141224, Size: 108B
// Obj: gcAll_psp.obj

? gcCreatureController__gcCreatureController_cBaseptr(); /* extern */

void *gcBipedController__gcBipedController_cBaseptr(void *arg0) {
    gcCreatureController__gcCreatureController_cBaseptr();
    arg0->unk4 = 0x38BBD8;
    arg0->unk90 = 0;
    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $a0, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("sv.q C120, 0xa0($s0)");
    arg0->unkB0 = 0;
    arg0->unkB4 = 0.0f;
    arg0->unkB8 = 1.0f;
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
