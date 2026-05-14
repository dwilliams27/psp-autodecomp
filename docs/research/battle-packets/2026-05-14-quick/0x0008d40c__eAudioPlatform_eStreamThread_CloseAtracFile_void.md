# Battle Packet: `eAudioPlatform::eStreamThread::CloseAtracFile(void)`

## Target

- Address: `0x0008d40c`
- Size: `156` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eAudioPlatform::eStreamThread`
- Method family: `CloseAtracFile`
- Leaf: `False`

## Queue Metadata

- score: `108`
- priority: `1`
- reason: `has failure_notes; has failure snapshot; near miss 22B; manageable size`
- failure_action: `prep`
- near_miss_bytes: `22`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `22`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x0000e208` `matched` 28B `cFilePlatform::Close(void)`
- `0x0036bdbc` `untried` 0B `sceAtracReleaseAtracID`

Callers:
- `0x000b9568` `untried` 1012B `eAudioPlatform::eStreamThread::PlayStream(void)`

## Placement

- Canonical source: `src/eAudioPlatform_eStreamThread.cpp`
- Header(s): `include/eAudioPlatform_eStreamThread.h`
- Allowed source(s): `src/eAudioPlatform_eStreamThread.cpp`
- Split-TU prefix: `src/eAudioPlatform_eStreamThread_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eAudioPlatform::eStreamThread.h`

Not found.

## Prior Failure Notes

### Note 1

- session `f5d1bf53`; src `src/eAudioPlatform_eStreamThread.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/f5d1bf53/0008d40c__agent_self_reported_failure__src_eAudioPlatform_eStreamThread.cpp`

22/156 bytes diff with sched=1. All diffs are 3 instruction swap-pairs within the unrolled 276-byte struct copy from local AtracInfo to this+0x128: at 0x50/0x54 (li a2,0x22 vs move a1,sp), 0x68/0x6c (sw a1,4(s0) vs addiu a3,a3,8), 0x70/0x74 (addiu a2,-1 vs addu a1,sp,a3). All within-basic-block scheduler reorderings. Tried sched=2 (worsens to 34) and multiple struct layout permutations preserving 0x128 offset for this->member assignment. SNC scheduler chose different ordering than original; not controllable from C source. Exceeds unmatchable threshold.


## Disassembly

```asm
8d40c:	27bdfee0 	addiu	sp,sp,-288
8d410:	afb00114 	sw	s0,276(sp)
8d414:	afb10118 	sw	s1,280(sp)
8d418:	afbf011c 	sw	ra,284(sp)
8d41c:	00808025 	move	s0,a0
8d420:	0c003882 	jal	0xe208
8d424:	2604023c 	addiu	a0,s0,572
8d428:	8e04035c 	lw	a0,860(s0)
8d42c:	04800004 	bltz	a0,0x8d440
8d430:	2411ffff 	li	s1,-1
8d434:	0c0daf6f 	jal	0x36bdbc
8d438:	00000000 	nop
8d43c:	ae11035c 	sw	s1,860(s0)
8d440:	a3a00000 	sb	zero,0(sp)
8d444:	afa00100 	sw	zero,256(sp)
8d448:	afb10108 	sw	s1,264(sp)
8d44c:	afa0010c 	sw	zero,268(sp)
8d450:	afa00110 	sw	zero,272(sp)
8d454:	26080128 	addiu	t0,s0,296
8d458:	34070000 	li	a3,0x0
8d45c:	34060022 	li	a2,0x22
8d460:	03a02825 	move	a1,sp
8d464:	8ca40000 	lw	a0,0(a1)
8d468:	01008025 	move	s0,t0
8d46c:	8ca50004 	lw	a1,4(a1)
8d470:	ae040000 	sw	a0,0(s0)
8d474:	ae050004 	sw	a1,4(s0)
8d478:	24e70008 	addiu	a3,a3,8
8d47c:	24c6ffff 	addiu	a2,a2,-1
8d480:	03a72821 	addu	a1,sp,a3
8d484:	8ca40000 	lw	a0,0(a1)
8d488:	14c0fff8 	bnez	a2,0x8d46c
8d48c:	01078021 	addu	s0,t0,a3
8d490:	ae040000 	sw	a0,0(s0)
8d494:	8fb00114 	lw	s0,276(sp)
8d498:	8fb10118 	lw	s1,280(sp)
8d49c:	8fbf011c 	lw	ra,284(sp)
8d4a0:	03e00008 	jr	ra
8d4a4:	27bd0120 	addiu	sp,sp,288
```

## m2c Starting Point

```c
// Decompiled: eAudioPlatform::eStreamThread::CloseAtracFile(void)
// Address: 0x0008d40c, Size: 156B
// Obj: eAll_psp.obj

? cFilePlatform__Close_void(s32);                   /* extern */
? sceAtracReleaseAtracID(s32);                      /* extern */

void eAudioPlatform__eStreamThread__CloseAtracFile_void(void *arg0) {
    s32 *temp_t0;
    s32 *var_a1;
    s32 *var_s0;
    s32 temp_a0;
    s32 var_a0;
    s32 var_a2;
    s32 var_a3;

    cFilePlatform__Close_void(arg0 + 0x23C);
    temp_a0 = arg0->unk35C;
    if (temp_a0 >= 0) {
        sceAtracReleaseAtracID(temp_a0);
        arg0->unk35C = -1;
    }
    temp_t0 = arg0 + 0x128;
    var_a3 = 0;
    var_a2 = 0x22;
    var_a1 = sp;
    var_a0 = *sp;
    var_s0 = temp_t0;
    do {
        var_s0->unk0 = var_a0;
        var_s0->unk4 = (s32) var_a1->unk4;
        var_a3 += 8;
        var_a2 -= 1;
        var_a1 = sp + var_a3;
        var_a0 = *var_a1;
        var_s0 = temp_t0 + var_a3;
    } while (var_a2 != 0);
    *var_s0 = var_a0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
