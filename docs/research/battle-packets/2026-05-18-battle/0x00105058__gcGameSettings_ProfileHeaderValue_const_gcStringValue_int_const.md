# Battle Packet: `gcGameSettings::ProfileHeaderValue(const gcStringValue *, int) const`

## Target

- Address: `0x00105058`
- Size: `148` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcGameSettings`
- Method family: `ProfileHeaderValue`
- Leaf: `False`

## Queue Metadata

- score: `106`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 42B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 0 retry/prep target`
- failure_action: `prep`
- near_miss_bytes: `42`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `42`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x00104ef4` `untried` 248B `gcGameSettings::ProfileFind(const gcStringValue *) const`

Callers:
- `0x0035a31c` `untried` 2292B `gcValSaveGame::Evaluate(void) const`

## Placement

- Canonical source: `src/gcGameSettings.cpp`
- Header(s): `include/gcGameSettings.h`
- Allowed source(s): `src/gcGameSettings.cpp`
- Split-TU prefix: `src/gcGameSettings_*.cpp`

## Class Header: `include/gcGameSettings.h`

Not found.

## Matched Same-Class Neighbors

- `0x0010260c` 208B `gcGameSettings::gcGameSettings(cBase *)` — `src/gcGameSettings.cpp`
- `0x001030c8` 28B `gcGameSettings::Get(void) static` — `src/eHeightmapData.cpp`
- `0x001032d8` 72B `gcGameSettings::HandleSaveGame(void)` — `src/gcGameSettings.cpp`
- `0x00103320` 84B `gcGameSettings::SaveGameClear(int, int)` — `src/gcGameSettings.cpp`
- `0x00103788` 188B `gcGameSettings::SaveGameRestore(int)` — `src/gcGameSettings.cpp`

## Prior Failure Notes

### Note 1

- session `a96922c7`; src `src/gcGameSettings.cpp`; snapshot `logs/failure_snapshots/match_20260505_191203/a96922c7/00105058__agent_self_reported_failure__src_gcGameSettings.cpp`

gcGameSettings::ProfileHeaderValue(const gcStringValue *, int) const — 42/148 bytes mismatch (~28%). Logic structure correct: bounds-check arg2 ([0,2)), call ProfileFind, return *(float*)(mProfiles + p*24 + 12 + arg2*4) on success, 0.0f otherwise. Used inline-asm barrier on header pointer to force `addiu a0, a0, 12` separation — that part matches target. Core remaining divergence is epilogue layout: target emits success path with `b shared_tail; lwc1 $f0, 0(a0)` (delay slot) and has TWO failure epilogues — arg2-OOB has its own full mtc1+lw...jr block, while ProfileFind<0 has `mtc1 zero` then falls through to a tail SHARED with the success branch. My output emits FOUR distinct return blocks (success + 3 failure paths) all with full epilogues. Tried structures: (a) outer combined `if (idx >= 0 && idx < 2)` with inner `if (p >= 0) { return ...; } return 0.0f; ... return 0.0f;` (best, 42 mismatch); (b) goto-label single-zero-return (140B too short, all merged); (c) `if (idx<0||idx>=2)` combined first-check (60 mismatch); (d) `if (p<0) return 0;` early-return form (76 mismatch, generated bgez instead of bltz); (e) result-variable single-return with else block (60 mismatch). The asymmetric shared-tail with arg2-OOB kept distinct is a specific SNC layout choice I could not trigger via source structure. Permuter not run on this one (covered by sibling permute on SaveGameGetSize). Likely needs permuter or compiler-pattern research on shared-tail-with-distinct-prologue heuristic.


## Disassembly

```asm
105058:	27bdfff0 	addiu	sp,sp,-16
10505c:	afb10004 	sw	s1,4(sp)
105060:	00808825 	move	s1,a0
105064:	00a02025 	move	a0,a1
105068:	afb00000 	sw	s0,0(sp)
10506c:	afbf0008 	sw	ra,8(sp)
105070:	04c00012 	bltz	a2,0x1050bc
105074:	00c08025 	move	s0,a2
105078:	2a050002 	slti	a1,s0,2
10507c:	10a0000f 	beqz	a1,0x1050bc
105080:	00802825 	move	a1,a0
105084:	0c0413bd 	jal	0x104ef4
105088:	02202025 	move	a0,s1
10508c:	00402025 	move	a0,v0
105090:	04800010 	bltz	a0,0x1050d4
105094:	000420c0 	sll	a0,a0,0x3
105098:	8e250040 	lw	a1,64(s1)
10509c:	00843021 	addu	a2,a0,a0
1050a0:	00862021 	addu	a0,a0,a2
1050a4:	00a42021 	addu	a0,a1,a0
1050a8:	2484000c 	addiu	a0,a0,12
1050ac:	00102880 	sll	a1,s0,0x2
1050b0:	00852021 	addu	a0,a0,a1
1050b4:	10000008 	b	0x1050d8
1050b8:	c4800000 	lwc1	$f0,0(a0)
1050bc:	44800000 	mtc1	zero,$f0
1050c0:	8fb00000 	lw	s0,0(sp)
1050c4:	8fb10004 	lw	s1,4(sp)
1050c8:	8fbf0008 	lw	ra,8(sp)
1050cc:	03e00008 	jr	ra
1050d0:	27bd0010 	addiu	sp,sp,16
1050d4:	44800000 	mtc1	zero,$f0
1050d8:	8fb00000 	lw	s0,0(sp)
1050dc:	8fb10004 	lw	s1,4(sp)
1050e0:	8fbf0008 	lw	ra,8(sp)
1050e4:	03e00008 	jr	ra
1050e8:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcGameSettings::ProfileHeaderValue(const gcStringValue *, int) const
// Address: 0x00105058, Size: 148B
// Obj: gcAll_psp.obj

s32 gcGameSettings__ProfileFind_constgcStringValueptrconst(void *, ?); /* extern */

f32 gcGameSettings__ProfileHeaderValue_constgcStringValueptr_intconst(void *arg0, ? arg1, s32 arg2) {
    s32 temp_v0;

    if ((arg2 >= 0) && (arg2 < 2)) {
        temp_v0 = gcGameSettings__ProfileFind_constgcStringValueptrconst(arg0, arg1);
        if (temp_v0 >= 0) {
            return *(arg0->unk40 + (temp_v0 * 0x18) + 0xC + (arg2 * 4));
        }
        return 0.0f;
    }
    return 0.0f;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
