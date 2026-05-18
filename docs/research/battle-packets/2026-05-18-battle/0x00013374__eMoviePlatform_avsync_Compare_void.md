# Battle Packet: `eMoviePlatform::avsync_Compare(void)`

## Target

- Address: `0x00013374`
- Size: `172` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMoviePlatform`
- Method family: `avsync_Compare`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 25B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `25`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `25`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00012ae8` `matched` 20B `eMoviePlatform::dispbuf_getPts(void)`
- `0x00012fcc` `matched` 20B `eMoviePlatform::soundbuf_getPts(void)`

Callers:
- `0x00012b6c` `untried` 296B `eMoviePlatform::dispbuf_thread(void)`

## Placement

- Canonical source: `src/eMoviePlatform.cpp`
- Header(s): `include/eMoviePlatform.h`
- Allowed source(s): `src/eMoviePlatform.cpp`
- Split-TU prefix: `src/eMoviePlatform_*.cpp`

## Class Header: `include/eMoviePlatform.h`

Not found.

## Matched Same-Class Neighbors

- `0x000119f4` 36B `eMoviePlatform::checkDecodeEnd(void)` — `src/eMoviePlatform_misc.cpp`
- `0x00011a18` 48B `eMoviePlatform::checkPlayerErrCondition(void)` — `src/eMoviePlatform_control_cond.cpp`
- `0x00011a48` 140B `eMoviePlatform::startCheck(void)` — `src/eMoviePlatform.cpp`
- `0x00011c80` 168B `eMoviePlatform::GetWorkAreaFreeSize(int)` — `src/eMoviePlatform.cpp`
- `0x00011d28` 28B `eMoviePlatform::OnSuspend(void *) static` — `src/eMoviePlatform.cpp`

## Prior Failure Notes

### Note 1

- session `1c72153d`; src `src/eMoviePlatform.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/1c72153d/00013374__agent_self_reported_failure__src_eMoviePlatform.cpp`

Best match achieved: 25/172 bytes differ. Structure is functionally correct (returns 1/2/4 based on diff vs 2*threshold). Logic: if (m_soundbuf_start == 0 || m_dispbuf_start == 0) return 1; compute diff = soundbuf_getPts() - dispbuf_getPts(); if diff < -2*threshold OR diff > 2*threshold, compute ret in {2, 4}; else return 1. Final source uses goto end pattern with shared epilog.

Remaining diffs are concentrated in the final 'compute' block. The target uses the SNC bnezl branch-likely pattern (Pattern 5: 'if (cond) flag = lit') with `bnezl s0, +1; li a0, 2; move v0, a0` for the final `if (isGreater) ret = 2; return ret;`. SNC allocates ret to a0 and emits `move v0, a0` at the very end. Our compiler instead allocates ret to v0 directly and uses `beqz s0, +2; nop; li v0, 2`, missing the bnezl pattern and the move instruction. This is a 4-byte size difference (43 instr target vs 42 instr ours).

Additional differences: target recomputes 2*threshold (addu a1, negu a1, addu a0 = 3 instr) while ours uses (addu a0, negu a1 = 2 instr) saving an instruction; target fills two `b` delay slots with `li a0, 4` while ours has nops there.

Approaches tried (all variants of source structure for the if/else/compute logic):
1. `if (isLess || isGreater) { ret = 4; if (isGreater) ret = 2; return ret; } return 1;` -- 74 bytes diff (uses OR codegen)
2. `if/else if/else` cascade with `ret = 4` then `if (isGreater) ret = 2;` -- 72 bytes
3. `if (isLess) goto block_7; if (!isGreater) return 1; block_7: ret = 4; if (isGreater) ret = 2;` -- 56 bytes (DOES emit bnezl, .word 0x56000001)
4. Without explicit isGreater var, repeating `(threshold + threshold) < diff` -- 51 bytes
5. Shared epilog via `goto end` -- 33 bytes (lost bnezl)
6. Inverted polarity with `if (!isLess)` outer -- 25 bytes (best, BEQZ matches but no bnezl)
7. sched=1 pragma -- 61-68 bytes worse

Root cause hypothesis: SNC at -O2 sched=2 prefers bnezl + non-v0 register + move when the conditional set is followed only by a return AND the value lives across non-trivial branch shapes. Our current source has rv life-cycle rooted in v0 (initialized via `int rv = 4` early, hoisted to `li v0, 4` before any branch). Target's structure keeps the value in a0 throughout, which suggests the source initializes the value DEEP inside conditional branches (in delay slots) rather than upfront. Could not find a C source structure that triggers this exact register allocation choice.

Not a bnel divergence per ADR-006 (this is bnezl pattern selection, not bnel/beql heuristic mismatch). May benefit from permuter run; current permuter requires isolated source file.


## Disassembly

```asm
13374:	27bdfff0 	addiu	sp,sp,-16
13378:	8c850330 	lw	a1,816(a0)
1337c:	afb00000 	sw	s0,0(sp)
13380:	afb10004 	sw	s1,4(sp)
13384:	afbf0008 	sw	ra,8(sp)
13388:	10a00013 	beqz	a1,0x133d8
1338c:	00808025 	move	s0,a0
13390:	8e0402e8 	lw	a0,744(s0)
13394:	10800010 	beqz	a0,0x133d8
13398:	00000000 	nop
1339c:	0c004bf3 	jal	0x12fcc
133a0:	02002025 	move	a0,s0
133a4:	00408825 	move	s1,v0
133a8:	0c004aba 	jal	0x12ae8
133ac:	02002025 	move	a0,s0
133b0:	8e04037c 	lw	a0,892(s0)
133b4:	02228023 	subu	s0,s1,v0
133b8:	00842821 	addu	a1,a0,a0
133bc:	00052823 	negu	a1,a1
133c0:	00842021 	addu	a0,a0,a0
133c4:	0205282a 	slt	a1,s0,a1
133c8:	10a00009 	beqz	a1,0x133f0
133cc:	0090802a 	slt	s0,a0,s0
133d0:	1000000b 	b	0x13400
133d4:	34040004 	li	a0,0x4
133d8:	34020001 	li	v0,0x1
133dc:	8fb00000 	lw	s0,0(sp)
133e0:	8fb10004 	lw	s1,4(sp)
133e4:	8fbf0008 	lw	ra,8(sp)
133e8:	03e00008 	jr	ra
133ec:	27bd0010 	addiu	sp,sp,16
133f0:	16000003 	bnez	s0,0x13400
133f4:	34040004 	li	a0,0x4
133f8:	10000004 	b	0x1340c
133fc:	34020001 	li	v0,0x1
13400:	56000001 	bnezl	s0,0x13408
13404:	34040002 	li	a0,0x2
13408:	00801025 	move	v0,a0
1340c:	8fb00000 	lw	s0,0(sp)
13410:	8fb10004 	lw	s1,4(sp)
13414:	8fbf0008 	lw	ra,8(sp)
13418:	03e00008 	jr	ra
1341c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eMoviePlatform::avsync_Compare(void)
// Address: 0x00013374, Size: 172B
// Obj: eAll_psp.obj

s32 eMoviePlatform__dispbuf_getPts_void(void *);    /* extern */
s32 eMoviePlatform__soundbuf_getPts_void(void *, s32); /* extern */

s32 eMoviePlatform__avsync_Compare_void(void *arg0) {
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s1;
    s32 var_a0;

    temp_a1 = arg0->unk330;
    if ((temp_a1 != 0) && (arg0->unk2E8 != 0)) {
        temp_s1 = eMoviePlatform__soundbuf_getPts_void(arg0, temp_a1);
        temp_a0 = arg0->unk37C;
        temp_s0 = temp_s1 - eMoviePlatform__dispbuf_getPts_void(arg0);
        temp_s0_2 = (temp_a0 * 2) < temp_s0;
        if (temp_s0 < -(temp_a0 * 2)) {
            var_a0 = 4;
            goto block_7;
        }
        var_a0 = 4;
        if (temp_s0_2 == 0) {
            return 1;
        }
block_7:
        if (temp_s0_2 != 0) {
            var_a0 = 2;
        }
        return var_a0;
    }
    return 1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
