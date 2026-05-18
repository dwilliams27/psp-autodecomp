# Battle Packet: `eSound::SetSoundFlagsOnOff(unsigned int, unsigned int)`

## Target

- Address: `0x00021354`
- Size: `268` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSound`
- Method family: `SetSoundFlagsOnOff`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 9B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `9`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `9`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00020358` `matched` 64B `eWorld::AddSound(eSound *)`
- `0x00020398` `matched` 132B `eWorld::RemoveSound(eSound *)`

Callers:
- `0x0001d0a4` `untried` 384B `eAudioChannel::OnMemPoolReset(const cMemPool *, unsigned int)`
- `0x0001e3f4` `untried` 408B `eAudio::StopSound(int, cTimeValue) static`
- `0x0001ebc8` `untried` 344B `eAudio::StopAllSounds(void) static`
- `0x00091674` `untried` 1740B `eAudioChannel::Update(cTimeValue, float, int, const mOCS *)`
- `0x000f3404` `untried` 584B `gcEntity::RemoveFromActiveList(int)`
- `0x001184bc` `untried` 456B `gcEntity::PlaySound(int, cHandleT<eSoundData>, float, cHandleT<eSoundData>, cTimeValue, bool, short)`
- `0x00118684` `untried` 248B `gcEntity::StopSound(int)`
- `0x001746f8` `untried` 11232B `gcEntity::Replicate(gcReplicationVisitor *)`
- `0x001772d8` `untried` 1692B `gcEntity::Update(cTimeValue)`

## Placement

- Canonical source: `src/eSound.cpp`
- Header(s): `include/eSound.h`
- Allowed source(s): `src/eSound.cpp`, `src/eVolume.cpp`
- Split-TU prefix: `src/eSound_*.cpp`

## Class Header: `include/eSound.h`

Not found.

## Matched Same-Class Neighbors

- `0x000211b0` 40B `eSound::Write(cFile &) const` — `src/eSound.cpp`
- `0x000212c8` 140B `eSound::~eSound(void)` — `src/eSound.cpp`
- `0x00021460` 64B `eSound::IsPlaying(void) const` — `src/gcValHeadset.cpp`
- `0x000215e0` 44B `eSound::Stop(void)` — `src/cObjectKey.cpp`
- `0x001e01a8` 168B `eSound::AssignCopy(const cBase *)` — `src/eSound.cpp`

## Prior Failure Notes

### Note 1

- session `ca1d928d`; src `src/eVolume.cpp`; snapshot `logs/failure_snapshots/match_20260507_103437/ca1d928d/00021354__agent_self_reported_failure__src_eVolume.cpp`

Implemented eSound::SetSoundFlagsOnOff as a local split-TU redeclaration in src/eVolume.cpp, per the prompt. The function compiles to the correct 268-byte size and matches the prologue, flag update, AddSound/RemoveSound call paths, float zeroing path, and overall control flow. A 180s permuter run compiled 2482 candidates and found no improvement over the 9-byte baseline.

The remaining 9-byte mismatch is entirely in the handle-table validation block. Expected loads the global table base into a3, builds the indexed address and loaded entry in a2, uses a3 as the valid-result register, and checks beqz a3 before proceeding. The closest C emits the same instructions and branches but swaps entry/valid between a2 and a3 after the table lookup, including the addu operand order and subsequent beqz/lw/move register fields. Tried table[index] vs char* indexed loads, declaration reordering, fixed-register hints, sched=1, and the permuter; the root cause looks like low-level register allocation with no obvious C leverage left.


## Disassembly

```asm
21354:	27bdfff0 	addiu	sp,sp,-16
21358:	afb00000 	sw	s0,0(sp)
2135c:	00808025 	move	s0,a0
21360:	00a02025 	move	a0,a1
21364:	9605000c 	lhu	a1,12(s0)
21368:	00c03027 	nor	a2,a2,zero
2136c:	00a42025 	or	a0,a1,a0
21370:	00862024 	and	a0,a0,a2
21374:	afb10004 	sw	s1,4(sp)
21378:	afb20008 	sw	s2,8(sp)
2137c:	afbf000c 	sw	ra,12(sp)
21380:	10850031 	beq	a0,a1,0x21448
21384:	34110000 	li	s1,0x0
21388:	30a50001 	andi	a1,a1,0x1
2138c:	54a00001 	bnezl	a1,0x21394
21390:	34110001 	li	s1,0x1
21394:	34120000 	li	s2,0x0
21398:	30850001 	andi	a1,a0,0x1
2139c:	54a00001 	bnezl	a1,0x213a4
213a0:	34120001 	li	s2,0x1
213a4:	12400013 	beqz	s2,0x213f4
213a8:	00000000 	nop
213ac:	8e05002c 	lw	a1,44(s0)
213b0:	14a00003 	bnez	a1,0x213c0
213b4:	30a6ffff 	andi	a2,a1,0xffff
213b8:	1000000c 	b	0x213ec
213bc:	34070000 	li	a3,0x0
213c0:	3c070004 	lui	a3,0x4
213c4:	00063080 	sll	a2,a2,0x2
213c8:	24e78890 	addiu	a3,a3,-30576
213cc:	00c73021 	addu	a2,a2,a3
213d0:	8cc60000 	lw	a2,0(a2)
213d4:	10c00005 	beqz	a2,0x213ec
213d8:	34070000 	li	a3,0x0
213dc:	8cc80030 	lw	t0,48(a2)
213e0:	15050002 	bne	t0,a1,0x213ec
213e4:	00000000 	nop
213e8:	00c03825 	move	a3,a2
213ec:	10e00016 	beqz	a3,0x21448
213f0:	00000000 	nop
213f4:	12400009 	beqz	s2,0x2141c
213f8:	a604000c 	sh	a0,12(s0)
213fc:	16200007 	bnez	s1,0x2141c
21400:	00000000 	nop
21404:	8e040008 	lw	a0,8(s0)
21408:	0c0080d6 	jal	0x20358
2140c:	02002825 	move	a1,s0
21410:	9604000c 	lhu	a0,12(s0)
21414:	34840002 	ori	a0,a0,0x2
21418:	a604000c 	sh	a0,12(s0)
2141c:	1640000a 	bnez	s2,0x21448
21420:	00000000 	nop
21424:	12200008 	beqz	s1,0x21448
21428:	00000000 	nop
2142c:	8e040008 	lw	a0,8(s0)
21430:	50800004 	beqzl	a0,0x21444
21434:	44806000 	mtc1	zero,$f12
21438:	0c0080e6 	jal	0x20398
2143c:	02002825 	move	a1,s0
21440:	44806000 	mtc1	zero,$f12
21444:	e60c0034 	swc1	$f12,52(s0)
21448:	8fb00000 	lw	s0,0(sp)
2144c:	8fb10004 	lw	s1,4(sp)
21450:	8fb20008 	lw	s2,8(sp)
21454:	8fbf000c 	lw	ra,12(sp)
21458:	03e00008 	jr	ra
2145c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eSound::SetSoundFlagsOnOff(unsigned int, unsigned int)
// Address: 0x00021354, Size: 268B
// Obj: eAll_psp.obj

? eWorld__AddSound_eSoundptr(s32, void *, void *, void *); /* extern */
? eWorld__RemoveSound_eSoundptr(s32, void *);       /* extern */

void eSound__SetSoundFlagsOnOff_unsignedint_unsignedint(void *arg0, s32 arg1, ? arg2, s32 arg3) {
    s32 temp_a0_2;
    s32 temp_a1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_s1;
    s32 var_s2;
    u16 temp_a0;
    u16 temp_a1;

    var_a3 = arg3;
    temp_a1 = arg0->unkC;
    var_a2 = ~arg2;
    temp_a0 = (temp_a1 | arg1) & var_a2;
    var_s1 = 0;
    if (temp_a0 != temp_a1) {
        if (temp_a1 & 1) {
            var_s1 = 1;
        }
        var_s2 = 0;
        if (temp_a0 & 1) {
            var_s2 = 1;
        }
        if (var_s2 != 0) {
            temp_a1_2 = arg0->unk2C;
            var_a2 = temp_a1_2 & 0xFFFF;
            if (temp_a1_2 == 0) {
                var_a3 = 0;
            } else {
                var_a2 = (var_a2 * 4)->unk38890;
                var_a3 = 0;
                if ((var_a2 != 0) && (var_a2->unk30 == temp_a1_2)) {
                    var_a3 = var_a2;
                }
            }
            if (var_a3 != 0) {
                goto block_12;
            }
        } else {
block_12:
            arg0->unkC = temp_a0;
            if ((var_s2 != 0) && (var_s1 == 0)) {
                eWorld__AddSound_eSoundptr(arg0->unk8, arg0, (void *) var_a2, (void *) var_a3);
                arg0->unkC = (u16) (arg0->unkC | 2);
            }
            if ((var_s2 == 0) && (var_s1 != 0)) {
                temp_a0_2 = arg0->unk8;
                if (temp_a0_2 != 0) {
                    eWorld__RemoveSound_eSoundptr(temp_a0_2, arg0);
                }
                arg0->unk34 = 0.0f;
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
