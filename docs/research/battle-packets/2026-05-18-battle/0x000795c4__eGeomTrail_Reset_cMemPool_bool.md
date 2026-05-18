# Battle Packet: `eGeomTrail::Reset(cMemPool *, bool)`

## Target

- Address: `0x000795c4`
- Size: `344` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eGeomTrail`
- Method family: `Reset`
- Leaf: `False`

## Queue Metadata

- score: `123`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 19B; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `19`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `19`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00211e44` `untried` 568B `cArrayBase<eGeomTrailParticle>::SetSize(int, const cArrayBase &)`

## Placement

- Canonical source: `src/eGeomTrail.cpp`
- Header(s): `include/eGeomTrail.h`
- Allowed source(s): `src/eGeomTrail.cpp`
- Split-TU prefix: `src/eGeomTrail_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eGeomTrail.h`

Not found.

## Matched Same-Class Neighbors

- `0x00079358` 76B `eGeomTrail::Write(cFile &) const` — `src/eGeomTrail.cpp`
- `0x000793a4` 188B `eGeomTrail::Read(cFile &, cMemPool *)` — `src/eGeomTrail.cpp`
- `0x00079460` 64B `eGeomTrail::eGeomTrail(cBase *)` — `src/eGeomTrail.cpp`
- `0x000794a0` 292B `eGeomTrail::~eGeomTrail(void)` — `src/eGeomTrail.cpp`
- `0x002119f4` 704B `eGeomTrail::AssignCopy(const cBase *)` — `src/eGeomTrail_AssignCopy.cpp`

## Matched Method Exemplars

### Exemplar 1: `eParticleSystemTemplate::Reset(cMemPool *, bool)`

- Address: `0x0007b578`
- Size: `188` bytes
- Source: `src/eParticleSystemTemplate.cpp`

```cpp
// -- eParticleSystemTemplate::Reset(cMemPool *, bool) @ 0x0007b578 --
#pragma control sched=1
void eParticleSystemTemplate::Reset(cMemPool *, bool) {
    eParticleSystemTemplateLayout *self = (eParticleSystemTemplateLayout *)this;
    cArrayBase<eParticle> *particles = &self->particles;
    int count = 0;
    __asm__ volatile("" : "+r"(count));
    if (self->enabled != 0) {
        count = self->particleCount;
    }
    int size = count;

    ParticleArrayDefault def;
    float zero = 0.0f;
    def.f20 = zero;
    __asm__ volatile("" ::: "memory");
    def.i24 = 0;
    def.i28 = 0;
    def.f2C = zero;
    def.f30 = zero;
    def.i34 = 0;
    def.h38 = 0;
    __asm__ volatile("" : "+m"(def));
    particles->SetSize(size, *(cArrayBase<eParticle> *)&def);

    self->field150 = 0;
    WordValue word;
    word.value = 0;
    self->word154 = word;
    word.value = 0;
    self->word158 = word;
    word.value = 0;
    self->word15C = word;
    __asm__ volatile("sw $0, 0x160(%0)" : : "r"(self) : "memory");

    __asm__ volatile(
        "lv.q C120, 0xa0($16)\n"
        "sv.q C120, 0x170($16)"
        :
        :
        : "memory");

    if (self->enabled != 0) {
        WordValue reset;
        reset.value = 0;
        self->word54 = reset;
        __asm__ volatile("sb %0, 0x4d(%1)" : : "r"(1), "r"(self) : "memory");
    }
}
```

### Exemplar 2: `eInputJoystick::Reset(void) static`

- Address: `0x0003a768`
- Size: `120` bytes
- Source: `src/eInputJoystick.cpp`

```cpp
void eInputJoystick::Reset() {
    int i = 0;
    char *state0 = (char *)0x41118;
    char *state1 = (char *)0;
    char *addr1 = (char *)0x43228;
    state0 = state1 + (unsigned int)state0;
    state1 = state1 + (unsigned int)addr1;
    do {
        ((eInputState *)state0)->Reset();
        ((eInputState *)state1)->Reset();
        i++;
        state0 += 0x844;
        state1 += 0x844;
    } while (i < 4);
    StopRumble(-1);
}
```

### Exemplar 3: `ePath::Reset(cMemPool *, bool)`

- Address: `0x0005b70c`
- Size: `92` bytes
- Source: `src/ePath.cpp`

```cpp
void ePath::Reset(cMemPool *, bool needSize) {
    ePathPoint *points = mPoints;
    needSize = 1;
    if (points != 0) {
        unsigned int count = *(unsigned int *)((char *)points - 4);
        needSize = (count & 0x3FFFFFFF) == 0;
    }
    if (needSize != 0) {
        ePathPoint defaultPoint;
        ePathPointArray_SetSize((cArrayBase<ePathPoint> *)&mPoints, 1,
                                ePathPoint_ctor(&defaultPoint));
    }
}
```

### Exemplar 4: `eAudio::Reset(void) static`

- Address: `0x0001d7c8`
- Size: `80` bytes
- Source: `src/eAudio.cpp`

```cpp
void eAudio::Reset() {
    unsigned int addr = 0x40E88;
    int i = 0;
    char *p = (char *)0;
    p = p + addr;
    do {
        ((eAudioGroup *)p)->ResetUserFaders();
        i += 1;
        p += 0x38;
    } while (i < 4);
}
```


## Prior Failure Notes

### Note 1

- session `88cb6737`; src `src/eGeomTrail.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/88cb6737/000795c4__agent_self_reported_failure__src_eGeomTrail.cpp`

Implemented eGeomTrail::Reset(cMemPool *, bool) as canonical C++ in src/eGeomTrail.cpp. The current source reconstructs the stack frame, clears fields 0xFC and 0xF4, sizes the particle cArrayBase at 0xF8 with a zeroed 32-byte default particle, handles the null-template early return, validates the template handle at +0x48 through D_00038890, and conditionally copies byte +0x5D to this+0x8D. Existing matched eGeomTrail constructor/destructor/New/GetType/Write/Read still compare as MATCH.

Closest retained source is size-correct at 344 bytes but compare_func reports MISMATCH: 19/344 bytes differ. Manual variants tried included direct if/else, explicit goto layout to place the null-template block before the first handle lookup, stack padding to force locals at sp+0x10/sp+0x30, pointer-shaped default initialization to match the SetSize setup, duplicated null-handle flag load to recover the extra lbu, register hints for handle/candidate/flag, and raw SetSize call experiments. Permuter ran 300s under default sched=2 and improved 27 -> 19 bytes, then a save-improved run retained the 19-byte source; a separate 300s --sched 1 run from that baseline generated 7560 candidates / 3285 compiled with zero improvement. Remaining diff is register-selection drift in the two D_00038890 lookups: generated code uses candidate/result in a1/a2 opposite the original in several lw/beqz/move/lbu/sb encodings. This is retryable failed, not unmatchable_bnel, because the diff is 19 bytes and not isolated to bnel/beql.


## Disassembly

```asm
795c4:	27bdffa0 	addiu	sp,sp,-96
795c8:	afb00050 	sw	s0,80(sp)
795cc:	afb10054 	sw	s1,84(sp)
795d0:	afbf0058 	sw	ra,88(sp)
795d4:	00808025 	move	s0,a0
795d8:	afa00000 	sw	zero,0(sp)
795dc:	260400fc 	addiu	a0,s0,252
795e0:	ac800000 	sw	zero,0(a0)
795e4:	ae0000f4 	sw	zero,244(s0)
795e8:	8e040060 	lw	a0,96(s0)
795ec:	10800016 	beqz	a0,0x79648
795f0:	260500f8 	addiu	a1,s0,248
795f4:	00808825 	move	s1,a0
795f8:	8e240050 	lw	a0,80(s1)
795fc:	24870001 	addiu	a3,a0,1
79600:	afa00010 	sw	zero,16(sp)
79604:	27a60010 	addiu	a2,sp,16
79608:	acc00004 	sw	zero,4(a2)
7960c:	acc00008 	sw	zero,8(a2)
79610:	acc0000c 	sw	zero,12(a2)
79614:	acc00010 	sw	zero,16(a2)
79618:	acc00014 	sw	zero,20(a2)
7961c:	acc00018 	sw	zero,24(a2)
79620:	acc0001c 	sw	zero,28(a2)
79624:	00a02025 	move	a0,a1
79628:	0c084791 	jal	0x211e44
7962c:	00e02825 	move	a1,a3
79630:	a200008d 	sb	zero,141(s0)
79634:	8e240048 	lw	a0,72(s1)
79638:	14800014 	bnez	a0,0x7968c
7963c:	3085ffff 	andi	a1,a0,0xffff
79640:	1000001d 	b	0x796b8
79644:	34050000 	li	a1,0x0
79648:	afa00030 	sw	zero,48(sp)
7964c:	27a60030 	addiu	a2,sp,48
79650:	acc00004 	sw	zero,4(a2)
79654:	acc00008 	sw	zero,8(a2)
79658:	acc0000c 	sw	zero,12(a2)
7965c:	acc00010 	sw	zero,16(a2)
79660:	acc00014 	sw	zero,20(a2)
79664:	acc00018 	sw	zero,24(a2)
79668:	acc0001c 	sw	zero,28(a2)
7966c:	00a02025 	move	a0,a1
79670:	0c084791 	jal	0x211e44
79674:	00002825 	move	a1,zero
79678:	8fb00050 	lw	s0,80(sp)
7967c:	8fb10054 	lw	s1,84(sp)
79680:	8fbf0058 	lw	ra,88(sp)
79684:	03e00008 	jr	ra
79688:	27bd0060 	addiu	sp,sp,96
7968c:	00052880 	sll	a1,a1,0x2
79690:	3c060004 	lui	a2,0x4
79694:	24c68890 	addiu	a2,a2,-30576
79698:	00a62821 	addu	a1,a1,a2
7969c:	8ca60000 	lw	a2,0(a1)
796a0:	10c00005 	beqz	a2,0x796b8
796a4:	34050000 	li	a1,0x0
796a8:	8cc70030 	lw	a3,48(a2)
796ac:	14e40002 	bne	a3,a0,0x796b8
796b0:	00000000 	nop
796b4:	00c02825 	move	a1,a2
796b8:	10a00013 	beqz	a1,0x79708
796bc:	00000000 	nop
796c0:	14800004 	bnez	a0,0x796d4
796c4:	3085ffff 	andi	a1,a0,0xffff
796c8:	34060000 	li	a2,0x0
796cc:	1000000d 	b	0x79704
796d0:	90c6005d 	lbu	a2,93(a2)
796d4:	00052880 	sll	a1,a1,0x2
796d8:	3c060004 	lui	a2,0x4
796dc:	24c68890 	addiu	a2,a2,-30576
796e0:	00a62821 	addu	a1,a1,a2
796e4:	8ca50000 	lw	a1,0(a1)
796e8:	10a00005 	beqz	a1,0x79700
796ec:	34060000 	li	a2,0x0
796f0:	8ca70030 	lw	a3,48(a1)
796f4:	54e40003 	bnel	a3,a0,0x79704
796f8:	90c6005d 	lbu	a2,93(a2)
796fc:	00a03025 	move	a2,a1
79700:	90c6005d 	lbu	a2,93(a2)
79704:	a206008d 	sb	a2,141(s0)
79708:	8fb00050 	lw	s0,80(sp)
7970c:	8fb10054 	lw	s1,84(sp)
79710:	8fbf0058 	lw	ra,88(sp)
79714:	03e00008 	jr	ra
79718:	27bd0060 	addiu	sp,sp,96
```

## m2c Starting Point

```c
// Decompiled: eGeomTrail::Reset(cMemPool *, bool)
// Address: 0x000795c4, Size: 344B
// Obj: eAll_psp.obj

? boolcArrayBase_eGeomTrailParticle___SetSize_int_constcArrayBaseref(void *, s32, s32 *, s32); /* extern */

void eGeomTrail__Reset_cMemPoolptr_bool(void *arg0) {
    s32 sp10;
    s32 sp30;
    s32 temp_a0_2;
    s32 temp_a3;
    void *temp_a0;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_a2;
    void *var_a1;
    void *var_a2;

    arg0->unkFC = 0;
    arg0->unkF4 = 0;
    temp_a0 = arg0->unk60;
    temp_a1 = arg0 + 0xF8;
    if (temp_a0 != NULL) {
        temp_a3 = temp_a0->unk50 + 1;
        sp10 = 0;
        sp10.unk4 = 0;
        sp10.unk8 = 0;
        sp10.unkC = 0;
        sp10.unk10 = 0;
        sp10.unk14 = 0;
        sp10.unk18 = 0;
        sp10.unk1C = 0;
        boolcArrayBase_eGeomTrailParticle___SetSize_int_constcArrayBaseref(temp_a1, temp_a3, &sp10, temp_a3);
        arg0->unk8D = 0U;
        temp_a0_2 = temp_a0->unk48;
        if (temp_a0_2 == 0) {
            var_a1 = NULL;
        } else {
            temp_a2 = ((temp_a0_2 & 0xFFFF) * 4)->unk38890;
            var_a1 = NULL;
            if ((temp_a2 != NULL) && (temp_a2->unk30 == temp_a0_2)) {
                var_a1 = temp_a2;
            }
        }
        if (var_a1 != NULL) {
            if (temp_a0_2 == 0) {
                var_a2 = NULL;
            } else {
                temp_a1_2 = ((temp_a0_2 & 0xFFFF) * 4)->unk38890;
                var_a2 = NULL;
                if ((temp_a1_2 != NULL) && (temp_a1_2->unk30 == temp_a0_2)) {
                    var_a2 = temp_a1_2;
                }
            }
            arg0->unk8D = (u8) var_a2->unk5D;
        }
        return;
    }
    sp30 = 0;
    sp30.unk4 = 0;
    sp30.unk8 = 0;
    sp30.unkC = 0;
    sp30.unk10 = 0;
    sp30.unk14 = 0;
    sp30.unk18 = 0;
    sp30.unk1C = 0;
    boolcArrayBase_eGeomTrailParticle___SetSize_int_constcArrayBaseref(temp_a1, 0, &sp30);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
