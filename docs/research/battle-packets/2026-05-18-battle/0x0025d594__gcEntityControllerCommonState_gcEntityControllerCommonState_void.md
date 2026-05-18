# Battle Packet: `gcEntityControllerCommonState::gcEntityControllerCommonState(void)`

## Target

- Address: `0x0025d594`
- Size: `260` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntityControllerCommonState`
- Method family: `Constructor`
- Leaf: `True`

## Queue Metadata

- score: `102`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 49B; codegen-specific diagnosis; Constructor retry family; manageable size`
- next_run_reason: `lane 0 retry/prep target`
- failure_action: `prep`
- near_miss_bytes: `49`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `49`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context


Callers:
- `0x00171e80` `untried` 4952B `gcEntity::Reset(cMemPool *, bool)`

## Placement

- Canonical source: `src/gcEntityControllerCommonState.cpp`
- Header(s): `include/gcEntityControllerCommonState.h`
- Allowed source(s): `src/gcEntityControllerCommonState.cpp`
- Split-TU prefix: `src/gcEntityControllerCommonState_*.cpp`

## Compiler Guidance

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

## Class Header: `include/gcEntityControllerCommonState.h`

Not found.

## Matched Same-Class Neighbors

- `0x00110738` 128B `gcEntityControllerCommonState::SetAttractor(gcEntity *, const cName &)` — `src/gcTableColumnByte.cpp`

## Prior Failure Notes

### Note 1

- session `9d51f518`

Implemented a real C++ constructor with a reconstructed layout and small VFPU inline-asm blocks for the five vector stores. The first three zero-vector stores match exactly once the class is given a trivial base so scalar fields compile as direct offset stores. The remaining diff is 49/260 bytes: scalar/VFPU scheduling is close, but the compiler keeps allocating the 0xD0 vector setup as a1/a2/a3 plus t0 for the 0xff byte constant, while the original uses a2/a3/t0 and keeps the 0xff constant in a1. Attempts with hard-coded inline asm registers, clobbers, dummy live registers, volatile byte fields, and a live 0xff variable either had no effect or made the early constant scheduling worse.

The tail also differs: our build emits the final move v0,a0 in the jr delay slot, while the target moves v0 before the last stores and places the final swc1 in the delay slot. This likely needs a more precise way to make SNC keep the 0xff value live in a1 and schedule the 0.5/1.0 tail stores across the final return, or a different high-level representation of the vector fields that lets the compiler rather than inline asm own the final scheduling.


## Disassembly

```asm
25d594:	44806000 	mtc1	zero,$f12
25d598:	44056000 	mfc1	a1,$f12
25d59c:	44066000 	mfc1	a2,$f12
25d5a0:	44076000 	mfc1	a3,$f12
25d5a4:	48e50006 	mtv         $a1, S120
25d5a8:	48e60026 	mtv         $a2, S121
25d5ac:	48e70046 	mtv         $a3, S122
25d5b0:	f8860000 	sv.q        C120, 0x0($a0)
25d5b4:	44056000 	mfc1	a1,$f12
25d5b8:	44066000 	mfc1	a2,$f12
25d5bc:	44076000 	mfc1	a3,$f12
25d5c0:	48e50006 	mtv         $a1, S120
25d5c4:	48e60026 	mtv         $a2, S121
25d5c8:	48e70046 	mtv         $a3, S122
25d5cc:	f8860010 	sv.q        C120, 0x10($a0)
25d5d0:	44056000 	mfc1	a1,$f12
25d5d4:	44066000 	mfc1	a2,$f12
25d5d8:	44076000 	mfc1	a3,$f12
25d5dc:	48e50006 	mtv         $a1, S120
25d5e0:	48e60026 	mtv         $a2, S121
25d5e4:	48e70046 	mtv         $a3, S122
25d5e8:	f8860020 	sv.q        C120, 0x20($a0)
25d5ec:	3c053f80 	lui	a1,0x3f80
25d5f0:	e48c0040 	swc1	$f12,64(a0)
25d5f4:	44856800 	mtc1	a1,$f13
25d5f8:	e48c0044 	swc1	$f12,68(a0)
25d5fc:	e48d0048 	swc1	$f13,72(a0)
25d600:	ac80004c 	sw	zero,76(a0)
25d604:	a0800050 	sb	zero,80(a0)
25d608:	2405ffff 	li	a1,-1
25d60c:	a4850052 	sh	a1,82(a0)
25d610:	ac800054 	sw	zero,84(a0)
25d614:	ac8000a0 	sw	zero,160(a0)
25d618:	44066000 	mfc1	a2,$f12
25d61c:	ac8000a4 	sw	zero,164(a0)
25d620:	44076000 	mfc1	a3,$f12
25d624:	e48c00a8 	swc1	$f12,168(a0)
25d628:	44086000 	mfc1	t0,$f12
25d62c:	e48c00ac 	swc1	$f12,172(a0)
25d630:	e48c00b0 	swc1	$f12,176(a0)
25d634:	ac8000b4 	sw	zero,180(a0)
25d638:	ac8000c0 	sw	zero,192(a0)
25d63c:	340500ff 	li	a1,0xff
25d640:	a08500e0 	sb	a1,224(a0)
25d644:	48e60006 	mtv         $a2, S120
25d648:	48e70026 	mtv         $a3, S121
25d64c:	48e80046 	mtv         $t0, S122
25d650:	f88600d0 	sv.q        C120, 0xD0($a0)
25d654:	44066000 	mfc1	a2,$f12
25d658:	ac8000f0 	sw	zero,240(a0)
25d65c:	44076000 	mfc1	a3,$f12
25d660:	a0850110 	sb	a1,272(a0)
25d664:	44056000 	mfc1	a1,$f12
25d668:	48e50006 	mtv         $a1, S120
25d66c:	48e60026 	mtv         $a2, S121
25d670:	48e70046 	mtv         $a3, S122
25d674:	3c053f00 	lui	a1,0x3f00
25d678:	f8860100 	sv.q        C120, 0x100($a0)
25d67c:	44857000 	mtc1	a1,$f14
25d680:	00801025 	move	v0,a0
25d684:	e48e0120 	swc1	$f14,288(a0)
25d688:	ac800124 	sw	zero,292(a0)
25d68c:	e48d012c 	swc1	$f13,300(a0)
25d690:	03e00008 	jr	ra
25d694:	e48d0130 	swc1	$f13,304(a0)
```

## m2c Starting Point

```c
// Decompiled: gcEntityControllerCommonState::gcEntityControllerCommonState(void)
// Address: 0x0025d594, Size: 260B
// Obj: gcAll_psp.obj

void *gcEntityControllerCommonState__gcEntityControllerCommonState_void(void *arg0) {
    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $a2, S121");
    __asm__ volatile("mtv $a3, S122");
    __asm__ volatile("sv.q C120, ($a0)");
    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $a2, S121");
    __asm__ volatile("mtv $a3, S122");
    __asm__ volatile("sv.q C120, 0x10($a0)");
    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $a2, S121");
    __asm__ volatile("mtv $a3, S122");
    __asm__ volatile("sv.q C120, 0x20($a0)");
    arg0->unk40 = 0.0f;
    arg0->unk44 = 0.0f;
    arg0->unk48 = 1.0f;
    arg0->unk4C = 0;
    arg0->unk50 = 0;
    arg0->unk52 = -1;
    arg0->unk54 = 0;
    arg0->unkA0 = 0;
    arg0->unkA4 = 0;
    arg0->unkA8 = 0.0f;
    arg0->unkAC = 0.0f;
    arg0->unkB0 = 0.0f;
    arg0->unkB4 = 0;
    arg0->unkC0 = 0;
    arg0->unkE0 = 0xFF;
    __asm__ volatile("mtv $a2, S120");
    __asm__ volatile("mtv $a3, S121");
    __asm__ volatile("mtv $t0, S122");
    __asm__ volatile("sv.q C120, 0xd0($a0)");
    arg0->unkF0 = 0;
    arg0->unk110 = 0xFF;
    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $a2, S121");
    __asm__ volatile("mtv $a3, S122");
    __asm__ volatile("sv.q C120, 0x100($a0)");
    arg0->unk120 = 0.5f;
    arg0->unk124 = 0;
    arg0->unk12C = 1.0f;
    arg0->unk130 = 1.0f;
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
