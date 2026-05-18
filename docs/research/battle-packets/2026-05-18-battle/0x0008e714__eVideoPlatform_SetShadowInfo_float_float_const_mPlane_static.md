# Battle Packet: `eVideoPlatform::SetShadowInfo(float, float, const mPlane &) static`

## Target

- Address: `0x0008e714`
- Size: `44` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eVideoPlatform`
- Method family: `SetShadowInfo`
- Leaf: `True`

## Queue Metadata

- score: `158`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 6B; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `6`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `6`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x0007d7b8` `untried` 1300B `eProjector::Draw(const eDrawInfo &) const`
- `0x000b80c8` `untried` 1780B `eShadow::Draw(const eDrawInfo &) const`

## Placement

- Canonical source: `src/eVideoPlatform.cpp`
- Header(s): `include/eVideoPlatform.h`
- Allowed source(s): `src/eVideoPlatform.cpp`
- Split-TU prefix: `src/eVideoPlatform_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eVideoPlatform.h`

Not found.

## Prior Failure Notes

### Note 1

- session `567ba94e`; src `src/eVideoPlatform.cpp`; snapshot `logs/failure_snapshots/match_20260505_162336/567ba94e/0008e714__agent_self_reported_failure__src_eVideoPlatform.cpp`

SetShadowInfo: 44-byte leaf. Confirmed prior agent's stable 6-byte diff. Best source: `float wv = plane.w; mPlane *dst=(mPlane*)0x98388; *(v4sf_t*)dst=*(const v4sf_t*)&plane; dst->w=wv;` -> exactly 6 bytes diff at offsets 0x1c/0x20. Expected emits `lui a0,0xa; sv.q C120,-0x7C78(a0); addiu a0,a0,-31864; jr ra; swc1 f12,12(a0)(delay)`. My codegen emits `lui a0,0xa; addiu a0,a0,-31864; sv.q C120,0(a0); jr ra; swc1 f12,12(a0)(delay)` -- addiu and sv.q are swapped. Also tried (1) `char *base=(char*)0xA0000; *(v4sf_t*)(base-0x7C78)=...; *(float*)(base-0x7C78+12)=wv;` -> compiler CSE'd to lui+sv.q+swc1 (3 insns, 40B total, smaller than expected); (2) absolute addresses `*(v4sf_t*)0x98388=...; *(float*)0x98394=wv;` -> 6B diff but with extra lui rather than addiu; (3) `v4sf_t v=*(const v4sf_t*)&plane; ... *(v4sf_t*)dst=v; dst->w=wv;` -> 12B diff (lv.q/lwc1 reordered). Root cause: SNC scheduler treats `dst = 0x98388` as an atomic lui+addiu materialization, then dereferences offset 0/12. Expected pattern requires SNC to split the materialization, using lui-relative for sv.q and emitting addiu only when needed for the swc1 offset 12 — an optimization SNC at -O2 -Xsched=2 does not produce from any source we've found. Permuter (1308 candidates from prior agent + my variants) found zero improvement. May require -Xmopt or -Xxopt flag tweak (cannot modify Makefile per agent guardrails). Recommend human review the Makefile to test -Xxopt=5 or -Xmopt=0 for src/eVideoPlatform.cpp. NOT a bnel/beql divergence so not classifiable as unmatchable per ADR-006. Current src/eVideoPlatform.cpp has the best 6-byte-diff version.

### Note 2

- session `526d3a23`

SetShadowInfo: 44-byte leaf. Best diff achieved is 6 bytes, and it is stable — permuter ran 1308 candidates at 21.6/sec with zero improvement, and manually recompiling with -Xsched=1 produces the SAME output as -Xsched=2 (so this is NOT a sched override issue despite the hint). Function structure: writes scalar args to 0x37D35C/0x37D360, copies plane quad to 0x98388 via lv.q/sv.q, then writes plane.w to 0x98394. Expected codegen uses `lui a0, 0xa` → `sv.q C120, -0x7C78(a0)` (lui-relative) → `addiu a0, a0, -31864` → `swc1 f12, 12(a0)` (base+offset). My codegen emits addiu BEFORE sv.q (`addiu a0, a0, -31864` → `sv.q C120, 0(a0)` → `swc1 f12, 12(a0)`) which is 6 bytes different in instruction encoding but semantically identical. Source tried: `mPlane *dst = (mPlane*)0x98388; *(v4sf_t*)dst = *(const v4sf_t*)&plane; dst->w = w;` (6B diff), absolute addresses with `((float*)0x98388)[3] = w` (35B, separate lui), `*(float*)0x98394 = w` (6B), char*base+offset variants. Root cause appears to be SNC's address lowering heuristic: when writing `*(v4sf_t*)absolute_addr_1` followed by `*addr_2->field_at_12`, SNC picks addiu-first pattern; expected wants lui-relative-first. May require a specific source idiom I haven't found — perhaps a macro/typedef used elsewhere in the original codebase, or `#pragma` that influences address lowering. Current src file has best 6-byte-diff version; next agent can continue from there.


## Disassembly

```asm
8e714:	3c050038 	lui	a1,0x38
8e718:	e4acd35c 	swc1	$f12,-11428(a1)
8e71c:	3c050038 	lui	a1,0x38
8e720:	e4add360 	swc1	$f13,-11424(a1)
8e724:	c48c000c 	lwc1	$f12,12(a0)
8e728:	d8860000 	lv.q        C120, 0x0($a0)
8e72c:	3c04000a 	lui	a0,0xa
8e730:	f8868388 	sv.q        C120, -0x7C78($a0)
8e734:	24848388 	addiu	a0,a0,-31864
8e738:	03e00008 	jr	ra
8e73c:	e48c000c 	swc1	$f12,12(a0)
```

## m2c Starting Point

```c
// Decompiled: eVideoPlatform::SetShadowInfo(float, float, const mPlane &) static
// Address: 0x0008e714, Size: 44B
// Obj: eAll_psp.obj

void eVideoPlatform__SetShadowInfo_float_float_constmPlanerefstatic(f32 arg0, f32 arg1) {
    *(f32 *)0x37D35C = arg0;
    *(f32 *)0x37D360 = arg1;
    __asm__ volatile("lv.q C120, ($a0)");
    __asm__ volatile("sv.q C120, -0x7c78($a0)");
    (void *)0x98388->unkC = (f32) arg0->unkC;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
