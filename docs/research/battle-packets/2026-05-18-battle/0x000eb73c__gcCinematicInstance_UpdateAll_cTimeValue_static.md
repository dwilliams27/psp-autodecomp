# Battle Packet: `gcCinematicInstance::UpdateAll(cTimeValue) static`

## Target

- Address: `0x000eb73c`
- Size: `160` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcCinematicInstance`
- Method family: `UpdateAll`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 19B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `19`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `19`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000eb7dc` `failed` 92B `gcCinematicInstance::FreeDynamicInstances(void) static`
- `0x000eb840` `untried` 1060B `gcCinematicInstance::Update(cTimeValue)`

Callers:
- `0x000f364c` `untried` 688B `gcMap::Update(cTimeValue)`

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

## Matched Method Exemplars

### Exemplar 1: `nwSocket::UpdateAll(cTimeValue) static`

- Address: `0x001a8260`
- Size: `100` bytes
- Source: `src/nwSocket.cpp`

```cpp
void nwSocket::UpdateAll(cTimeValue dt) {
    int i = 0;
    nwSocket **socket = D_00034958;

    do {
        nwSocket *s = *socket;
        if (s != 0) {
            s->Update(dt);
        }
        i += 1;
        socket += 1;
    } while (i < 1);
}
```


## Prior Failure Notes

### Note 1

- session `930ddec5`; src `src/gcCinematicInstance.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/930ddec5/000eb73c__agent_self_reported_failure__src_gcCinematicInstance.cpp`

Best result is 19/160 bytes differ. Function semantically matches: snapshot the circular intrusive list at D_0037D7C4 (linked via offset 0x54) into a 128-entry stack array, then iterate and call Update(dt) on each, then call FreeDynamicInstances. Loop2 + prologue + epilogue match expected exactly (the trick that fixes loop2 is `gcCinematicInstance **p = (gcCinematicInstance **)((char *)snapshot - 4); ... p[1]->Update(dt); ...` which lets the compiler emit `move s2, sp; lw a0, 4(s2)` instead of `addiu s2, sp, 4; lw a0, 0(s2)`; the current source uses the equivalent `snapshot[i]` form which compiles to identical code).

The remaining 19-byte diff is entirely in loop1: expected uses INDEX-based addressing (`sll a2, s0, 2; addu a2, sp, a2; sw a1, 4(a2)` recomputed each iter, with `addiu s0, s0, 1` in the bne delay slot), while our SNC strength-reduces to a running POINTER (`move a1, sp` once, `sw a2, 4(a1)`, with `addiu a1, a1, 4` in the delay slot). Both encode `snapshot[i] = cur` with identical semantics.

Approaches tried: (1) plain `snapshot[count] = cur; count++` — pointer-based, 19B diff. (2) `snapshot[count++] = cur` — same. (3) count++ moved before/after the cur-advance — same. (4) `__asm__ volatile("":::"memory")` between the array store and the next-link load — no effect on strength reduction. (5) `*(gcCinematicInstance * volatile *)&snapshot[count] = cur` — no effect. (6) `for (count=0;;count++) { ...; if (cur==head) break; }` style — 93/160 diff (much worse). (7) Declaring cur and count up front instead of inline — 26/160 diff (worse). (8) `#pragma control mopt=0` over the whole function — applies to whole function only, fixes loop1 (index-based) but converts loop2 to index-based too AND drops s3 saved register, gives 156B size mismatch. -Xmopt=0 reproduces this. The pragma cannot scope to a single loop.

Permuter ran 120s but picked the wrong same-size function (gcCinematicInstance::GetType is also exactly 160B and comes first in the .o; permuter at tools/permuter.py picks size_matches[0] and has no --symbol arg despite the warning text mentioning one). Did not attempt putting UpdateAll in a temp solo file because the diff is structural (full loop1 reorder, not 2-3 instruction permutation).

This is a compiler version divergence: the original SNC chose loop1=index AND loop2=pointer, a hybrid that our SNC 1.2.7503.0 cannot reproduce with any single -Xmopt setting. NOT a bnel/beql divergence (no branch-likely involved). Loop1 strength reduction is decided per-function by SNC and isn't overridable in source. Source preserved at src/gcCinematicInstance.cpp:265-285 as the closest semantic+structural match (loop2 + prologue + epilogue exact). Next agent could try: (a) -Xxopt=N variants (N=0..5), (b) put UpdateAll alone in a temp file for the permuter, (c) experiment with making count an unsigned/size_t/long type, or (d) recompile pspcor.exe with strength-reduction tuning.


## Disassembly

```asm
eb73c:	27bdfde0 	addiu	sp,sp,-544
eb740:	afa40000 	sw	a0,0(sp)
eb744:	3c040038 	lui	a0,0x38
eb748:	8c84d7c4 	lw	a0,-10300(a0)
eb74c:	afb00204 	sw	s0,516(sp)
eb750:	afb10208 	sw	s1,520(sp)
eb754:	afb2020c 	sw	s2,524(sp)
eb758:	afb30210 	sw	s3,528(sp)
eb75c:	afbf0214 	sw	ra,532(sp)
eb760:	10800015 	beqz	a0,0xeb7b8
eb764:	34100000 	li	s0,0x0
eb768:	00802825 	move	a1,a0
eb76c:	00103080 	sll	a2,s0,0x2
eb770:	03a63021 	addu	a2,sp,a2
eb774:	acc50004 	sw	a1,4(a2)
eb778:	8ca50054 	lw	a1,84(a1)
eb77c:	14a4fffb 	bne	a1,a0,0xeb76c
eb780:	26100001 	addiu	s0,s0,1
eb784:	34130000 	li	s3,0x0
eb788:	0270202a 	slt	a0,s3,s0
eb78c:	1080000a 	beqz	a0,0xeb7b8
eb790:	00000000 	nop
eb794:	8fb10000 	lw	s1,0(sp)
eb798:	03a09025 	move	s2,sp
eb79c:	8e440004 	lw	a0,4(s2)
eb7a0:	0c03ae10 	jal	0xeb840
eb7a4:	02202825 	move	a1,s1
eb7a8:	26730001 	addiu	s3,s3,1
eb7ac:	0270202a 	slt	a0,s3,s0
eb7b0:	1480fffa 	bnez	a0,0xeb79c
eb7b4:	26520004 	addiu	s2,s2,4
eb7b8:	0c03adf7 	jal	0xeb7dc
eb7bc:	00000000 	nop
eb7c0:	8fb00204 	lw	s0,516(sp)
eb7c4:	8fb10208 	lw	s1,520(sp)
eb7c8:	8fb2020c 	lw	s2,524(sp)
eb7cc:	8fb30210 	lw	s3,528(sp)
eb7d0:	8fbf0214 	lw	ra,532(sp)
eb7d4:	03e00008 	jr	ra
eb7d8:	27bd0220 	addiu	sp,sp,544
```

## m2c Starting Point

```c
// Decompiled: gcCinematicInstance::UpdateAll(cTimeValue) static
// Address: 0x000eb73c, Size: 160B
// Obj: gcAll_psp.obj

? gcCinematicInstance__FreeDynamicInstances_voidstatic(s32); /* extern */
? gcCinematicInstance__Update_cTimeValue(s32, s32); /* extern */

void gcCinematicInstance__UpdateAll_cTimeValuestatic(s32 arg0) {
    s32 var_s0;
    s32 var_s3;
    void *var_a0;
    void *var_a1;
    void *var_s2;

    var_a0 = *(void **)0x37D7C4;
    var_s0 = 0;
    if (var_a0 != NULL) {
        var_a1 = var_a0;
        do {
            (sp + (var_s0 * 4))->unk4 = var_a1;
            var_a1 = var_a1->unk54;
            var_s0 += 1;
        } while (var_a1 != var_a0);
        var_s3 = 0;
        var_a0 = (void *) (var_s0 > 0);
        if (var_a0 != NULL) {
            var_s2 = sp;
            do {
                gcCinematicInstance__Update_cTimeValue(var_s2->unk4, subroutine_arg0);
                var_s3 += 1;
                var_a0 = (void *) (var_s3 < var_s0);
                var_s2 += 4;
            } while (var_a0 != NULL);
        }
    }
    gcCinematicInstance__FreeDynamicInstances_voidstatic((s32) var_a0);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
