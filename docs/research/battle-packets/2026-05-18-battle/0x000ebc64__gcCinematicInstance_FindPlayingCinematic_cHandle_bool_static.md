# Battle Packet: `gcCinematicInstance::FindPlayingCinematic(cHandle, bool) static`

## Target

- Address: `0x000ebc64`
- Size: `168` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcCinematicInstance`
- Method family: `FindPlayingCinematic`
- Leaf: `False`

## Queue Metadata

- score: `148`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 29B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `29`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `29`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000eab04` `untried` 476B `gcCinematic::IsStreamed(void) const`

Callers:
- `0x000f41b8` `untried` 236B `gcMap::PlayCinematic(gcCinematic *, cHandle, float, bool) static`

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

- session `eceb75ce`; src `src/gcCinematicInstance.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/eceb75ce/000ebc64__agent_self_reported_failure__src_gcCinematicInstance.cpp`

FindPlayingCinematic(cHandle, bool) static — closest at 29/168 bytes diff after permuter (240s, 7632 candidates, 0 improvements). Function is a circular doubly-linked list iteration over D_0037D7C4. The bool materialization pattern `((handle.mIndex ^ nodeHandle) == 0) & 0xFF` generates the correct xor;sltiu;andi prologue, and the bnel at loop continuation matches with `lw a0,8(s1)` in the delay slot.

Remaining diff is purely scheduling/register allocation: original allocates the bool result to register a1 (intermediate xor result in a2), keeping a0 free to preload the IsStreamed `this` argument in the delay slot of `beqz a1`. My version reuses register a0 for the entire xor;sltiu;andi chain, which forces the load to be deferred to the jal delay slot, and produces `beqzl a0` (branch-likely) instead of `beqz a1` — with `lw s1, 84(s1)` (advance node) in the beqzl delay slot, plus a duplicated `lw s1, 84(s1)` after the IsStreamed check.

Tried: explicit int eq local; bool eq variable; separate diff/eq variables; single-expression && form; explicit goto-style control flow; || form (worse, 82 bytes); explicit gcCinematic* local cached before the if; permuter for 240s. All produced the same a0-only register allocation chain. The compiler's choice between (beqz + preload-this) vs (beqzl + advance-node) appears to be driven by a deeper internal heuristic not directly controllable from C++ source. Likely matchable only with a structural insight not yet found — perhaps a different cHandle/cBase access pattern that gives the compiler reason to allocate the bool to a non-a0 register.


## Disassembly

```asm
ebc64:	27bdffe0 	addiu	sp,sp,-32
ebc68:	afb2000c 	sw	s2,12(sp)
ebc6c:	afa40000 	sw	a0,0(sp)
ebc70:	3c120038 	lui	s2,0x38
ebc74:	afb10008 	sw	s1,8(sp)
ebc78:	8e51d7c4 	lw	s1,-10300(s2)
ebc7c:	afb00004 	sw	s0,4(sp)
ebc80:	afb30010 	sw	s3,16(sp)
ebc84:	afbf0014 	sw	ra,20(sp)
ebc88:	12200010 	beqz	s1,0xebccc
ebc8c:	30b300ff 	andi	s3,a1,0xff
ebc90:	8fb00000 	lw	s0,0(sp)
ebc94:	8e240008 	lw	a0,8(s1)
ebc98:	02043026 	xor	a2,s0,a0
ebc9c:	2cc50001 	sltiu	a1,a2,1
ebca0:	30a500ff 	andi	a1,a1,0xff
ebca4:	10a00005 	beqz	a1,0xebcbc
ebca8:	8e240000 	lw	a0,0(s1)
ebcac:	0c03aac1 	jal	0xeab04
ebcb0:	00000000 	nop
ebcb4:	1053000d 	beq	v0,s3,0xebcec
ebcb8:	00000000 	nop
ebcbc:	8e310054 	lw	s1,84(s1)
ebcc0:	8e44d7c4 	lw	a0,-10300(s2)
ebcc4:	5624fff4 	bnel	s1,a0,0xebc98
ebcc8:	8e240008 	lw	a0,8(s1)
ebccc:	00001025 	move	v0,zero
ebcd0:	8fb00004 	lw	s0,4(sp)
ebcd4:	8fb10008 	lw	s1,8(sp)
ebcd8:	8fb2000c 	lw	s2,12(sp)
ebcdc:	8fb30010 	lw	s3,16(sp)
ebce0:	8fbf0014 	lw	ra,20(sp)
ebce4:	03e00008 	jr	ra
ebce8:	27bd0020 	addiu	sp,sp,32
ebcec:	02201025 	move	v0,s1
ebcf0:	8fb00004 	lw	s0,4(sp)
ebcf4:	8fb10008 	lw	s1,8(sp)
ebcf8:	8fb2000c 	lw	s2,12(sp)
ebcfc:	8fb30010 	lw	s3,16(sp)
ebd00:	8fbf0014 	lw	ra,20(sp)
ebd04:	03e00008 	jr	ra
ebd08:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcCinematicInstance::FindPlayingCinematic(cHandle, bool) static
// Address: 0x000ebc64, Size: 168B
// Obj: gcAll_psp.obj

s32 gcCinematic__IsStreamed_voidconst(s32, s32, s32); /* extern */

void *gcCinematicInstance__FindPlayingCinematic_cHandle_boolstatic(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;
    void *var_s1;

    var_s1 = *(void **)0x37D7C4;
    if (var_s1 != NULL) {
loop_2:
        temp_a0 = var_s1->unk8;
        temp_a1 = (subroutine_arg0 == temp_a0) & 0xFF;
        if ((temp_a1 == 0) || (gcCinematic__IsStreamed_voidconst(var_s1->unk0, temp_a1, subroutine_arg0 ^ temp_a0) != (arg1 & 0xFF))) {
            var_s1 = var_s1->unk54;
            if (var_s1 == *(void **)0x37D7C4) {
                /* Duplicate return node #5. Try simplifying control flow for better match */
                return NULL;
            }
            goto loop_2;
        }
        return var_s1;
    }
    return NULL;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
