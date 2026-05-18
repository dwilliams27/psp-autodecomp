# Battle Packet: `eMoviePlatform::avsync_video_setPts(unsigned int)`

## Target

- Address: `0x000132f0`
- Size: `80` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMoviePlatform`
- Method family: `avsync_video_setPts`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `8`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x00012078` `untried` 276B `eMoviePlatform::decode_videoDecode(void)`

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

- session `c831991d`

Improved eMoviePlatform::avsync_video_setPts from the prior 11/80-byte mismatch to 8/80 bytes by scoping this function to #pragma control sched=1. The first 12 instructions still match exactly; the remaining diff is the three-instruction scheduling window after div. Current compiled order is lw a3,0x370; sw a1,0x378; sll a2,a2,2, while expected is sll a2,a2,2; sw a1,0x378; lw a3,0x370. sched=2 gives the prior lw; sll; sw order, and direct RMW of the count gives sll; sw; address/buffer store; lw, so the target order sits between the two natural schedules.

Tried scoped sched=1, hard memory barrier after the pts store, volatile count load, direct count RMW, storing count before the buffer slot, an explicit slot pointer, and several condition/source-order variants. Barrier/volatile variants moved the divide/mfhi region and regressed to 27/80 bytes; direct RMW was 12/80; explicit slot pointer introduced an extra addiu and regressed. Ran tools/permuter.py for 300s with --sched 1 and --save-best: 8040 candidates generated, 5855 compiled, zero improvements from the 8-byte baseline. Root cause still appears to be an SNC scheduler heuristic divergence around placing the independent count load after the div and pts store.

### Note 2

- session `013bfa7a`

eMoviePlatform::avsync_video_setPts — 11/80 bytes differ (pure instruction-scheduling divergence, NOT bnel). The first 18 instructions match exactly. The 3-instruction sequence {sll a2,a2,2; sw a1,0x378; lw a3,0x370} at offset 0x1C-0x24 of the function is PERMUTED in our compile to {lw a3,0x370; sll a2,a2,2; sw a1,0x378}. Expected schedules the count load (lw 0x370) AFTER the pts store (sw 0x378); ours schedules it BEFORE the sll. Tried: (a) direct RMW `*p = *p + 1` — gives 12 bytes with lw moved even later (after sw 0x358). (b) Extract local `int count = *p` — gives 11 bytes with lw hoisted too early. (c) Pre-compute new_idx via local `size` var, various source statement orderings. (d) __asm__ volatile("" ::: "memory") barriers in 3 positions — worse (33 bytes, forces mfhi early). (e) Pointer aliasing `unsigned int *buf_slot = ...` — adds an extra addiu instruction (29 bytes). (f) Permuter run of 240s / 7656 mutations / 5211 compiles — found zero improvements from baseline of 12. Root cause: SNC scheduler heuristic picks different position for the load-to-hide-latency slot after the div+sw 0x378. The load latency is satisfied in either position (5-instruction vs 3-instruction gap to first use), so both are valid schedules and the compiler just picks one. Neither local-variable nor direct-RMW forms coax SNC into the specific middle placement. Does NOT qualify as unmatchable_bnel (no bnel/bne divergence; diff is 11 bytes of scheduler-permuted instructions, not branch form). Next agent should try: (1) a different flag combination — perhaps -Xsched=1 or adjusting -Xxopt; (2) reorganizing the if-block to expose different data flow; (3) exploring if a helper inline function changes register pressure enough to flip the schedule.


## Disassembly

```asm
132f0:	2406ffff 	li	a2,-1
132f4:	54a60005 	bnel	a1,a2,0x1330c
132f8:	8c860368 	lw	a2,872(a0)
132fc:	8c850378 	lw	a1,888(a0)
13300:	8c86037c 	lw	a2,892(a0)
13304:	00a62821 	addu	a1,a1,a2
13308:	8c860368 	lw	a2,872(a0)
1330c:	8c870374 	lw	a3,884(a0)
13310:	24c80001 	addiu	t0,a2,1
13314:	0107001a 	div	zero,t0,a3
13318:	00063080 	sll	a2,a2,0x2
1331c:	ac850378 	sw	a1,888(a0)
13320:	8c870370 	lw	a3,880(a0)
13324:	00863021 	addu	a2,a0,a2
13328:	acc50358 	sw	a1,856(a2)
1332c:	24e50001 	addiu	a1,a3,1
13330:	ac850370 	sw	a1,880(a0)
13334:	00002810 	mfhi	a1
13338:	03e00008 	jr	ra
1333c:	ac850368 	sw	a1,872(a0)
```

## m2c Starting Point

```c
// Decompiled: eMoviePlatform::avsync_video_setPts(unsigned int)
// Address: 0x000132f0, Size: 80B
// Obj: eAll_psp.obj

void eMoviePlatform__avsync_video_setPts_unsignedint(void *arg0, s32 arg1) {
    s32 temp_a2;
    s32 var_a1;

    var_a1 = arg1;
    if (var_a1 == -1) {
        var_a1 = arg0->unk378 + arg0->unk37C;
    }
    temp_a2 = arg0->unk368;
    arg0->unk378 = var_a1;
    (arg0 + (temp_a2 * 4))->unk358 = var_a1;
    arg0->unk370 = (s32) (arg0->unk370 + 1);
    arg0->unk368 = (s32) ((s32) (temp_a2 + 1) % (s32) arg0->unk374);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
