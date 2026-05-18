# Battle Packet: `eConvexHullUtil::TensorSubExpression(float, float, float, float &, float &, float &) const`

## Target

- Address: `0x0006e77c`
- Size: `64` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eConvexHullUtil`
- Method family: `TensorSubExpression`
- Leaf: `True`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 12B; codegen-specific diagnosis; manageable size`
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
- `0x0006e4f8` `untried` 352B `eConvexHullUtil::AddTriangle(const mVec3 &, const mVec3 &, const mVec3 &)`

## Placement

- Canonical source: `src/eConvexHullUtil.cpp`
- Header(s): `include/eConvexHullUtil.h`
- Allowed source(s): `src/eConvexHullUtil.cpp`
- Split-TU prefix: `src/eConvexHullUtil_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eConvexHullUtil.h`

Not found.

## Matched Same-Class Neighbors

- `0x0006e3e8` 36B `eConvexHullUtil::Begin(void)` — `src/eEmbedVolumeCollisionHandler.cpp`

## Prior Failure Notes

### Note 1

- session `d1492e79`

Canonical C++ method in src/eConvexHullUtil.cpp compiles to the correct 16-instruction, 64-byte shape with correct FPU register allocation and store serialization, but remains 12/64 bytes different. The only differences are commutative FPU operand field encodings: expected add/mul forms include f15+f14, f15*f13, f15+f16, f17*f12, f15*f13, and f12+f14, while SNC consistently emits the opposite operand order for those same operations. I tested direct source operand reversal, compound assignments to make the accumulator explicit, and FPU register-bound locals with empty constraints; all produced the same 12-byte diff.

I also ran `python3 tools/permuter.py src/eConvexHullUtil.cpp 0x0006e77c --time 300 --sched 1 --save-best`. It generated 9024 candidates, compiled 5655, and found zero improvement from the 12-byte baseline. This looks like the same commutative FPU operand-order divergence noted by the prior session, not a control-flow, register allocation, or store-order issue.

### Note 2

- session `904463ce`

Structure matches (16 insns, 64 bytes, correct mul/add/store sequence with P/S/T stores serialized by volatile float* and memory barriers) — only 12/64 bytes differ. Root cause: SNC commutes add.s/mul.s operands on specific instructions despite source declaration order. Per snc-fpu-scheduling.md, source order normally controls scheduling priority, but in sched=1 zone and with volatile forcing serialization, the scheduler still picks the 'newer result + older operand' ordering for commutative operations. Tried: sched=1 pragma, sched=2 pragma, reordering source subexpressions, splitting into more locals, inserting barriers at different points. The 12-byte diff is isolated to instruction operand field bits; opcode and register allocation are correct. Likely needs either a permuter pass on operand order or a flag combination not yet identified.


## Disassembly

```asm
6e77c:	460d63c0 	add.s	$f15,$f12,$f13
6e780:	460e7c00 	add.s	$f16,$f15,$f14
6e784:	e4b00000 	swc1	$f16,0(a1)
6e788:	460c6442 	mul.s	$f17,$f12,$f12
6e78c:	460d7bc2 	mul.s	$f15,$f15,$f13
6e790:	460f8bc0 	add.s	$f15,$f17,$f15
6e794:	46107402 	mul.s	$f16,$f14,$f16
6e798:	46107c00 	add.s	$f16,$f15,$f16
6e79c:	e4d00000 	swc1	$f16,0(a2)
6e7a0:	460c8b02 	mul.s	$f12,$f17,$f12
6e7a4:	460d7b42 	mul.s	$f13,$f15,$f13
6e7a8:	460d6300 	add.s	$f12,$f12,$f13
6e7ac:	46107382 	mul.s	$f14,$f14,$f16
6e7b0:	460e6300 	add.s	$f12,$f12,$f14
6e7b4:	03e00008 	jr	ra
6e7b8:	e4ec0000 	swc1	$f12,0(a3)
```

## m2c Starting Point

```c
// Decompiled: eConvexHullUtil::TensorSubExpression(float, float, float, float &, float &, float &) const
// Address: 0x0006e77c, Size: 64B
// Obj: eAll_psp.obj

void eConvexHullUtil__TensorSubExpression_float_float_float_floatref_floatref_floatrefconst(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3) {
    f32 temp_f15;
    f32 temp_f15_2;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f17;

    temp_f15 = arg0 + arg1;
    temp_f16 = temp_f15 + arg1;
    *arg1 = temp_f16;
    temp_f17 = arg0 * arg0;
    temp_f15_2 = temp_f17 + (temp_f15 * arg1);
    temp_f16_2 = temp_f15_2 + (arg1 * temp_f16);
    *arg2 = temp_f16_2;
    *arg3 = (temp_f17 * arg0) + (temp_f15_2 * arg1) + (arg1 * temp_f16_2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
