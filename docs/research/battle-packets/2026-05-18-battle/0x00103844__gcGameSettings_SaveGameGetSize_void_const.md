# Battle Packet: `gcGameSettings::SaveGameGetSize(void) const`

## Target

- Address: `0x00103844`
- Size: `148` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcGameSettings`
- Method family: `SaveGameGetSize`
- Leaf: `False`

## Queue Metadata

- score: `123`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 42B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `42`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `42`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00056990` `matched` 16B `eMemCard::GetSaveSize(int, int, int) static`

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

- session `a96922c7`; src `src/gcGameSettings.cpp`; snapshot `logs/failure_snapshots/match_20260505_191203/a96922c7/00103844__agent_self_reported_failure__src_gcGameSettings.cpp`

gcGameSettings::SaveGameGetSize(void) const — 42/148 bytes mismatch (~28%). Logic structure matches target: load global gcConfig at 0x37D7DC -> mField74 (offset 0x74), compute mProfileNames size and (when nonempty) mField68[0] size, call eMemCard::GetSaveSize(sysArg, nameSize, itemSize). Used `((int *)*(void **)0x37D7DC)[29]` to coax the lui/lw -> lw with offset 0x74 codegen pattern (matches target). Three remaining divergences: (1) Target uses register a2 for the global pointer (8ca6d7dc + 8cc60074), mine uses a1 (8ca5d7dc + 8ca60074) — different scratch register pick. (2) Target schedules `lw a0, 104(a0)` (mField68 load) into the delay slot of `beqz mProfileNames`; mine puts `li a1,0` in the delay slot and emits the lw a0 BEFORE sw ra. (3) Target uses beqzl (branch-likely) at the hasItems test with delay slot `move a0, a2` (annulled if !taken); mine emits regular beqz with extra `move t0, a3` because the slt result lands in a3 instead of t0. Tried multiple variable-declaration orderings (nameSize/itemSize/hasItems decl order, items declared early vs late, hasItems init in delay vs explicit). Permuter ran 2 minutes (1488 candidates compiled) — zero improvement, baseline 42 unchanged. The branch-likely divergence may be the documented SNC beqzl/bnel heuristic difference per docs/decisions/006-bnel-compiler-divergence.md. Pattern aligns with sched=2 but the scheduler picks different register/delay-slot allocation than original SNC.


## Disassembly

```asm
103844:	27bdfff0 	addiu	sp,sp,-16
103848:	3c050038 	lui	a1,0x38
10384c:	8ca6d7dc 	lw	a2,-10276(a1)
103850:	8c87005c 	lw	a3,92(a0)
103854:	8cc60074 	lw	a2,116(a2)
103858:	34050000 	li	a1,0x0
10385c:	afbf0000 	sw	ra,0(sp)
103860:	10e00005 	beqz	a3,0x103878
103864:	8c840068 	lw	a0,104(a0)
103868:	8ce5fffc 	lw	a1,-4(a3)
10386c:	3c074000 	lui	a3,0x4000
103870:	24e7ffff 	addiu	a3,a3,-1
103874:	00a72824 	and	a1,a1,a3
103878:	34070000 	li	a3,0x0
10387c:	10800006 	beqz	a0,0x103898
103880:	34080000 	li	t0,0x0
103884:	8c88fffc 	lw	t0,-4(a0)
103888:	3c094000 	lui	t1,0x4000
10388c:	2529ffff 	addiu	t1,t1,-1
103890:	01094024 	and	t0,t0,t1
103894:	0008402a 	slt	t0,zero,t0
103898:	5100000a 	beqzl	t0,0x1038c4
10389c:	00c02025 	move	a0,a2
1038a0:	8c870000 	lw	a3,0(a0)
1038a4:	10e00005 	beqz	a3,0x1038bc
1038a8:	34040000 	li	a0,0x0
1038ac:	8ce4fffc 	lw	a0,-4(a3)
1038b0:	3c074000 	lui	a3,0x4000
1038b4:	24e7ffff 	addiu	a3,a3,-1
1038b8:	00872024 	and	a0,a0,a3
1038bc:	00803825 	move	a3,a0
1038c0:	00c02025 	move	a0,a2
1038c4:	0c015a64 	jal	0x56990
1038c8:	00e03025 	move	a2,a3
1038cc:	8fbf0000 	lw	ra,0(sp)
1038d0:	03e00008 	jr	ra
1038d4:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcGameSettings::SaveGameGetSize(void) const
// Address: 0x00103844, Size: 148B
// Obj: gcAll_psp.obj

? eMemCard__GetSaveSize_int_int_intstatic(s32, s32, s32, s32); /* extern */

void gcGameSettings__SaveGameGetSize_voidconst(void *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 var_a3;
    s32 var_t0;
    void *temp_a0;
    void *temp_a3;
    void *temp_a3_2;

    temp_a3 = arg0->unk5C;
    var_a1 = 0;
    temp_a0 = arg0->unk68;
    if (temp_a3 != NULL) {
        var_a1 = temp_a3->unk-4 & 0x3FFFFFFF;
    }
    var_a3 = 0;
    var_t0 = 0;
    if (temp_a0 != NULL) {
        var_t0 = (temp_a0->unk-4 & 0x3FFFFFFF) > 0;
    }
    if (var_t0 != 0) {
        temp_a3_2 = temp_a0->unk0;
        var_a0 = 0;
        if (temp_a3_2 != NULL) {
            var_a0 = temp_a3_2->unk-4 & 0x3FFFFFFF;
        }
        var_a3 = var_a0;
    }
    eMemCard__GetSaveSize_int_int_intstatic((*(void **)0x37D7DC)->unk74, var_a1, var_a3, var_a3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
