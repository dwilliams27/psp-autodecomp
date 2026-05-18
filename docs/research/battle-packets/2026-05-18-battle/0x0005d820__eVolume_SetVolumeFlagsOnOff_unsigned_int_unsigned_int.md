# Battle Packet: `eVolume::SetVolumeFlagsOnOff(unsigned int, unsigned int)`

## Target

- Address: `0x0005d820`
- Size: `264` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eVolume`
- Method family: `SetVolumeFlagsOnOff`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 32B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `32`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `32`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0003f474` `matched` 40B `eRoom::RemoveVolume(eVolume *)`
- `0x0005d928` `matched` 44B `eVolume::UpdateLocalToWorld(void)`

Callers:
- `0x0012d5b0` `untried` 312B `gcTrigger::Activate(bool)`

## Placement

- Canonical source: `src/eVolume.cpp`
- Header(s): `include/eVolume.h`
- Allowed source(s): `src/eVolume.cpp`
- Split-TU prefix: `src/eVolume_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eVolume.h`

Not found.

## Matched Same-Class Neighbors

- `0x0005d4a8` 156B `eVolume::Write(cFile &) const` — `src/eVolume.cpp`
- `0x0005d544` 272B `eVolume::Read(cFile &, cMemPool *)` — `src/eVolume.cpp`
- `0x0005d654` 156B `eVolume::eVolume(cBase *)` — `src/eVolume.cpp`
- `0x0005d7e8` 56B `eVolume::SetLocalToWorld(const mOCS &)` — `src/std.cpp`
- `0x0005d928` 44B `eVolume::UpdateLocalToWorld(void)` — `src/eVolume.cpp`

## Prior Failure Notes

### Note 1

- session `ca1d928d`; src `src/eVolume.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/ca1d928d/0005d820__agent_self_reported_failure__src_eVolume.cpp`

Implemented a C++ method body under sched=1. The flag math, old/new bit tests, store to +0x74, UpdateLocalToWorld call path, and eRoom::RemoveVolume call path can be made byte-identical. The remaining mismatch is isolated to the fallback circular-list unlink block after the room pointer is null. The closest source is 32/264 bytes different after a 180s permuter run improved the baseline from 34 bytes.

The mismatch appears to be SNC register allocation and branch-likely shaping around the world/head/prev/next pointers: expected keeps world in a1, uses a0 for prev/next loads, emits direct offset loads/stores at world+0x1c, and uses a bnel skip with a reload in the likely delay slot. The closest C keeps world/head as an address temporary, emits addiu for world+0x1c, uses beql with the store in the likely delay slot, and differs in the following prev/next reload register choices. Tried char-cast code, local prev/next variables, typed-member form, volatile head accesses, sched=2, fixed-register hints, and the permuter; none reached a match.


## Disassembly

```asm
5d820:	27bdfff0 	addiu	sp,sp,-16
5d824:	afb00000 	sw	s0,0(sp)
5d828:	afb10004 	sw	s1,4(sp)
5d82c:	afb20008 	sw	s2,8(sp)
5d830:	afbf000c 	sw	ra,12(sp)
5d834:	00a03825 	move	a3,a1
5d838:	00808025 	move	s0,a0
5d83c:	92050074 	lbu	a1,116(s0)
5d840:	00a72025 	or	a0,a1,a3
5d844:	00c03027 	nor	a2,a2,zero
5d848:	00862024 	and	a0,a0,a2
5d84c:	10850030 	beq	a0,a1,0x5d910
5d850:	34110000 	li	s1,0x0
5d854:	30a50001 	andi	a1,a1,0x1
5d858:	54a00001 	bnezl	a1,0x5d860
5d85c:	34110001 	li	s1,0x1
5d860:	34120000 	li	s2,0x0
5d864:	30850001 	andi	a1,a0,0x1
5d868:	54a00001 	bnezl	a1,0x5d870
5d86c:	34120001 	li	s2,0x1
5d870:	12400005 	beqz	s2,0x5d888
5d874:	a2040074 	sb	a0,116(s0)
5d878:	16200003 	bnez	s1,0x5d888
5d87c:	00000000 	nop
5d880:	0c01764a 	jal	0x5d928
5d884:	02002025 	move	a0,s0
5d888:	16400021 	bnez	s2,0x5d910
5d88c:	00000000 	nop
5d890:	1220001f 	beqz	s1,0x5d910
5d894:	00000000 	nop
5d898:	8e040024 	lw	a0,36(s0)
5d89c:	10800005 	beqz	a0,0x5d8b4
5d8a0:	00000000 	nop
5d8a4:	0c00fd1d 	jal	0x3f474
5d8a8:	02002825 	move	a1,s0
5d8ac:	10000018 	b	0x5d910
5d8b0:	00000000 	nop
5d8b4:	12000016 	beqz	s0,0x5d910
5d8b8:	8e050020 	lw	a1,32(s0)
5d8bc:	8e040078 	lw	a0,120(s0)
5d8c0:	10800013 	beqz	a0,0x5d910
5d8c4:	00000000 	nop
5d8c8:	8e04007c 	lw	a0,124(s0)
5d8cc:	10800010 	beqz	a0,0x5d910
5d8d0:	00000000 	nop
5d8d4:	8ca6001c 	lw	a2,28(a1)
5d8d8:	54d00004 	bnel	a2,s0,0x5d8ec
5d8dc:	8e060078 	lw	a2,120(s0)
5d8e0:	aca4001c 	sw	a0,28(a1)
5d8e4:	8e04007c 	lw	a0,124(s0)
5d8e8:	8e060078 	lw	a2,120(s0)
5d8ec:	acc4007c 	sw	a0,124(a2)
5d8f0:	8e060078 	lw	a2,120(s0)
5d8f4:	ac860078 	sw	a2,120(a0)
5d8f8:	ae000078 	sw	zero,120(s0)
5d8fc:	ae00007c 	sw	zero,124(s0)
5d900:	8ca4001c 	lw	a0,28(a1)
5d904:	14900002 	bne	a0,s0,0x5d910
5d908:	00000000 	nop
5d90c:	aca0001c 	sw	zero,28(a1)
5d910:	8fb00000 	lw	s0,0(sp)
5d914:	8fb10004 	lw	s1,4(sp)
5d918:	8fb20008 	lw	s2,8(sp)
5d91c:	8fbf000c 	lw	ra,12(sp)
5d920:	03e00008 	jr	ra
5d924:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eVolume::SetVolumeFlagsOnOff(unsigned int, unsigned int)
// Address: 0x0005d820, Size: 264B
// Obj: eAll_psp.obj

? eRoom__RemoveVolume_eVolumeptr(s32, void *);      /* extern */
? eVolume__UpdateLocalToWorld_void(void *, s32, s32, s32); /* extern */

void eVolume__SetVolumeFlagsOnOff_unsignedint_unsignedint(void *arg0, s32 arg1, ? arg2) {
    s32 temp_a0_2;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 var_s1;
    s32 var_s2;
    u8 temp_a0;
    u8 temp_a1;
    void *temp_a0_3;
    void *temp_a1_3;

    temp_a1 = arg0->unk74;
    temp_a2 = ~arg2;
    temp_a0 = (temp_a1 | arg1) & temp_a2;
    var_s1 = 0;
    if (temp_a0 != temp_a1) {
        if (temp_a1 & 1) {
            var_s1 = 1;
        }
        var_s2 = 0;
        temp_a1_2 = temp_a0 & 1;
        if (temp_a1_2 != 0) {
            var_s2 = 1;
        }
        arg0->unk74 = temp_a0;
        if ((var_s2 != 0) && (var_s1 == 0)) {
            eVolume__UpdateLocalToWorld_void(arg0, temp_a1_2, temp_a2, arg1);
        }
        if ((var_s2 == 0) && (var_s1 != 0)) {
            temp_a0_2 = arg0->unk24;
            if (temp_a0_2 != 0) {
                eRoom__RemoveVolume_eVolumeptr(temp_a0_2, arg0);
                return;
            }
            temp_a1_3 = arg0->unk20;
            if ((arg0 != NULL) && (arg0->unk78 != NULL)) {
                temp_a0_3 = arg0->unk7C;
                if (temp_a0_3 != NULL) {
                    if (temp_a1_3->unk1C == arg0) {
                        temp_a1_3->unk1C = temp_a0_3;
                    }
                    arg0->unk78->unk7C = arg0->unk7C;
                    arg0->unk7C->unk78 = (void *) arg0->unk78;
                    arg0->unk78 = NULL;
                    arg0->unk7C = NULL;
                    if (temp_a1_3->unk1C == arg0) {
                        temp_a1_3->unk1C = NULL;
                    }
                }
            }
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
