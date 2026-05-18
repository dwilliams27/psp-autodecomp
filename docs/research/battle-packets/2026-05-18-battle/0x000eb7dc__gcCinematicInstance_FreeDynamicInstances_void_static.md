# Battle Packet: `gcCinematicInstance::FreeDynamicInstances(void) static`

## Target

- Address: `0x000eb7dc`
- Size: `92` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcCinematicInstance`
- Method family: `FreeDynamicInstances`
- Leaf: `True`

## Queue Metadata

- score: `138`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `8`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x000eb73c` `failed` 160B `gcCinematicInstance::UpdateAll(cTimeValue) static`

## Placement

- Canonical source: `src/gcCinematicInstance.cpp`
- Header(s): `include/gcCinematicInstance.h`
- Allowed source(s): `src/gcCinematicInstance.cpp`
- Split-TU prefix: `src/gcCinematicInstance_*.cpp`

## Class Header: `include/gcCinematicInstance.h`

Not found.

## Matched Same-Class Neighbors

- `0x000eaf28` 40B `gcCinematicInstance::Write(cFile &) const` — `src/gcCinematicInstance.cpp`
- `0x000eaf50` 120B `gcCinematicInstance::Read(cFile &, cMemPool *)` — `src/gcCinematicInstance.cpp`
- `0x000eb3e0` 8B `gcCinematicInstance::Reset(cMemPool *, bool)` — `src/gcCinematic.cpp`
- `0x000eb838` 8B `gcCinematicInstance::UpdateProfile(void) static` — `src/gcCinematic.cpp`
- `0x000ebdb4` 24B `gcCinematicInstance::Chain(cHandleT<gcCinematic>, cHandle, float)` — `src/gcCinematicInstance.cpp`

## Prior Failure Notes

### Note 1

- session `61501237`

The existing C body still compiles to the exact target body through the natural epilogue, but the symbol is 84B while the expected function is 92B. The only missing bytes are the trailing `b .; nop` after `jr ra`/`addiu sp,sp,16`; the objdump confirms our `CloseDialogs` symbol begins immediately where the expected trap tail should be.

I tested two source-level alternatives not covered by the prior attempt: adding `__attribute__((noreturn))` to the method definition, which SNC ignored for layout and still emitted the same 84B symbol, and adding a forced `*(volatile int *)0 = 0` at function end, which emitted an ordinary `sw zero,0(zero)` before the epilogue and shifted following code rather than producing the target post-epilogue trap. This remains the known TRAILING_B_TRAP compiler/source-idiom blocker, not a body mismatch.

### Note 2

- session `dce77d20`

FreeDynamicInstances: Our C compiles to 84 bytes (21 insns), but target is 92 bytes (23 insns). Expected function has trailing 'b .; nop' trap after the natural jr ra/addiu delay epilogue. Body bytes match (confirmed via manual diff at offsets 0-0x53 when aligned). Tried: (1) original do-while form (84B, 21 insns — body matches target body), (2) while(true)+return form (80B, 21 insns — loop restructured with unconditional b back and no bnezl). Could not produce the trailing 8-byte trap. Root cause hypothesis: the trap is emitted because the original source has some construct the compiler considers unreachable at function end — perhaps an explicit infinite loop pattern, a call to a __noreturn__ function, or compiler-emitted alignment padding. Next function (UpdateProfile) starts at 0xeb838 which is 8-byte aligned but not 16-byte — so it's not simple 16-byte alignment. The 8-byte diff between our 84 and target 92 is purely the trap tail; the code body bytes match after relocation masking. Permuter would not help since we're missing bytes, not rearranging them.


## Disassembly

```asm
eb7dc:	27bdfff0 	addiu	sp,sp,-16
eb7e0:	afb00000 	sw	s0,0(sp)
eb7e4:	3c100038 	lui	s0,0x38
eb7e8:	8e04d7c8 	lw	a0,-10296(s0)
eb7ec:	afbf0004 	sw	ra,4(sp)
eb7f0:	1080000b 	beqz	a0,0xeb820
eb7f4:	00000000 	nop
eb7f8:	8c860004 	lw	a2,4(a0)
eb7fc:	34050003 	li	a1,0x3
eb800:	24c60050 	addiu	a2,a2,80
eb804:	84c70000 	lh	a3,0(a2)
eb808:	8cc60004 	lw	a2,4(a2)
eb80c:	00c0f809 	jalr	a2
eb810:	00872021 	addu	a0,a0,a3
eb814:	8e04d7c8 	lw	a0,-10296(s0)
eb818:	5480fff8 	bnezl	a0,0xeb7fc
eb81c:	8c860004 	lw	a2,4(a0)
eb820:	8fb00000 	lw	s0,0(sp)
eb824:	8fbf0004 	lw	ra,4(sp)
eb828:	03e00008 	jr	ra
eb82c:	27bd0010 	addiu	sp,sp,16
eb830:	1000ffff 	b	0xeb830
eb834:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcCinematicInstance::FreeDynamicInstances(void) static
// Address: 0x000eb7dc, Size: 92B
// Obj: gcAll_psp.obj

void gcCinematicInstance__FreeDynamicInstances_voidstatic(void) {
    ? (*temp_a2_2)(void *, ?, ?, s16);
    s16 temp_a3;
    void *temp_a2;

    if (*(void **)0x37D7C8 != NULL) {
        do {
            temp_a2 = (*(void **)0x37D7C8)->unk4;
            temp_a3 = temp_a2->unk50;
            temp_a2_2 = (temp_a2 + 0x50)->unk4;
            temp_a2_2(*(void **)0x37D7C8 + temp_a3, 3, temp_a2_2, temp_a3);
        } while (*(void **)0x37D7C8 != NULL);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
