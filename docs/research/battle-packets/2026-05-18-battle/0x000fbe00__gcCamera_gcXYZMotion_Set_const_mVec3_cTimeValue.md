# Battle Packet: `gcCamera::gcXYZMotion::Set(const mVec3 &, cTimeValue)`

## Target

- Address: `0x000fbe00`
- Size: `136` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcCamera::gcXYZMotion`
- Method family: `Set`
- Leaf: `True`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 4B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `4`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x000f88f8` `untried` 392B `gcCamera::Reset(void)`
- `0x000fae7c` `untried` 468B `gcCamera::SnapEnvironment(void)`
- `0x0016a888` `untried` 988B `gcCamera::UpdateEnvironment(cTimeValue, bool)`
- `0x00298c84` `untried` 2952B `gcDoCameraOp::Evaluate(void) const`

## Placement

- Canonical source: `src/gcCamera_gcXYZMotion.cpp`
- Header(s): `include/gcCamera_gcXYZMotion.h`
- Allowed source(s): `src/gcCamera_gcXYZMotion.cpp`
- Split-TU prefix: `src/gcCamera_gcXYZMotion_*.cpp`

## Class Header: `include/gcCamera::gcXYZMotion.h`

Not found.

## Matched Method Exemplars

### Exemplar 1: `gcLobbyConfigStrings::Set(const wchar_t *) const`

- Address: `0x00280728`
- Size: `128` bytes
- Source: `src/gcLobbyConfigStrings.cpp`

```cpp
// ── gcLobbyConfigStrings::Set(const wchar_t *) const  @ 0x00280728, 128B ──
void gcLobbyConfigStrings::Set(const wchar_t *src) const {
    char buf[256];
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0) {
        buf[0] = 0;
        cStrCopy(buf, src, 0xff);
        if (this->mText == 0) {
            DispatchEntry *e = (DispatchEntry *)(*(char **)lobby + 0x20);
            e->fn((char *)lobby + e->offset, buf, e->offset);
        }
    }
}
```

### Exemplar 2: `gcCamera::gcScalarMotion::Set(float, cTimeValue)`

- Address: `0x000fbcbc`
- Size: `116` bytes
- Source: `src/gcCamera_gcScalarMotion.cpp`

```cpp
void gcCamera::gcScalarMotion::Set(float value, cTimeValue time) {
    int ticks = time.mTime;
    if (ticks == 0) {
        mF4 = value;
        mF0 = value;
        mF8 = 0.0f;
    } else {
        float start = mF0;
        mF4 = value;
        *(float *)((char *)this + 0x14) =
            fabsf(value - start) / (*(float *)0x0036C800 * (float)ticks);
    }
}
```

### Exemplar 3: `gcStringLValue::Set(const gcStringValue *) const`

- Address: `0x0026a100`
- Size: `96` bytes
- Source: `src/gcStringLValue.cpp`

```cpp
// ── gcStringLValue::Set(const gcStringValue *) const @ 0x0026A100 ──
void gcStringLValue::Set(const gcStringValue *src) const {
    unsigned short buf[4096];
    char *src_classdesc = *(char **)((char *)src + 4);
    buf[0] = 0;
    DispatchEntry *get_entry = (DispatchEntry *)(src_classdesc + 0x70);
    get_entry->fn((char *)src + get_entry->offset, buf, 0x1000);

    char *this_classdesc = *(char **)((char *)this + 4);
    DispatchEntry *set_entry = (DispatchEntry *)(this_classdesc + 0x80);
    set_entry->fn((char *)this + set_entry->offset, buf, set_entry->offset);
}
```

### Exemplar 4: `gcTableColumnGUID::Set(int, const wchar_t *, bool)`

- Address: `0x002737c8`
- Size: `88` bytes
- Source: `src/gcTableColumnGUID.cpp`

```cpp
void gcTableColumnGUID::Set(int row, const wchar_t *text, bool flag) {
    char tmp[32];
    cStrCopy(tmp, text);
    cGUID *p = &mValues.mData[row];
    sscanf(tmp, "%08X-%08X", &p->mA, &p->mB);
}
```


## Prior Failure Notes

### Note 1

- session `9ba4fbb1`; src `src/gcCamera_gcXYZMotion.cpp`; snapshot `logs/failure_snapshots/match_20260510_120905/9ba4fbb1/000fbe00__agent_self_reported_failure__src_gcCamera_gcXYZMotion.cpp`

Matched 132/136 bytes (4 bytes diff). Final diff is a 2-instruction swap at offsets 0x60/0x64 in the ticks!=0 divide path: SNC emits `mtc1 $a1, $f12` (dist) before `mtc1 $a2, $f13` (ticks bits), but the EBOOT has them in the opposite order. The cvt.s.w/lui/lwc1/mul/div sequence and final swc1 all match. Iterations performed: (1) inline `*&v` struct copy + simple if/else - 30 bytes diff. (2) Hoisted SceULong128 vv temp + `if (ticks==0)` then-branch first - 30 bytes. (3) Swapped path order to `if (ticks!=0)` - got beqzl + duplicate sv.q at fall-through (132B function, 1 instruction short). (4) Added matching `if (ticks == 0)` with stores in BOTH branches plus SceULong128 vv temp - got bnezl + duplicate sv.q matching expected size. (5) Inserted `__asm__ volatile("" : : "f"(z))` between *(this+0x00)=vv and the first mfc1 to anchor mtc1 zero materialization - this fixed the mtc1/sv.q interleave at 0x1c/0x20. (6) Inserted `__asm__ volatile("" : : "r"(ticks))` after `int ticks = time.mTime;` and before SceULong128 vv load - this fixed the lw/lv.q swap at 0x08/0x0c in the prologue. (7) Tried various declaration orders for `dist` and `fticks` named locals: dist-first kept reg alloc ($f12=dist, $f13=fticks) but mtc1 $a1 issued first; fticks-first put mtc1 $a2 first but flipped reg alloc to ($f12=fticks, $f13=dist) which broke mul/div register fields downstream (21 bytes diff). (8) Tried embedding mtc1 inside the inline asm via `=f`(dist) constraint - 20 bytes diff (different lui/lwc1/mtc1 sequence). Permuter ran 300s with 4094 candidates compiled, found 0 improvements over the 4-byte baseline. Root cause appears to be SNC-side scheduler choice between two valid orderings of independent mtc1 instructions after mfv: SNC consumes `$a1` (the mfv output) immediately to free the GPR before `lui $a1` reuses it, while the EBOOT's compiler version inserted mtc1 $a2 (independent of $a1) into that single-cycle latency slot first. This is REG_ALLOC drift (mtc1 destination register field), not bnel/beql divergence, so does not qualify for unmatchable classification. Source has no leverage over the scheduler's choice between these two register-allocation orderings here - it's a 4-byte register-field difference.


## Disassembly

```asm
fbe00:	27bdfff0 	addiu	sp,sp,-16
fbe04:	afa60000 	sw	a2,0(sp)
fbe08:	8fa60000 	lw	a2,0(sp)
fbe0c:	d8a60000 	lv.q        C120, 0x0($a1)
fbe10:	54c0000d 	bnezl	a2,0xfbe48
fbe14:	f8860010 	sv.q        C120, 0x10($a0)
fbe18:	f8860010 	sv.q        C120, 0x10($a0)
fbe1c:	44806000 	mtc1	zero,$f12
fbe20:	f8860000 	sv.q        C120, 0x0($a0)
fbe24:	44056000 	mfc1	a1,$f12
fbe28:	44066000 	mfc1	a2,$f12
fbe2c:	44076000 	mfc1	a3,$f12
fbe30:	48e50006 	mtv         $a1, S120
fbe34:	48e60026 	mtv         $a2, S121
fbe38:	48e70046 	mtv         $a3, S122
fbe3c:	f8860020 	sv.q        C120, 0x20($a0)
fbe40:	03e00008 	jr	ra
fbe44:	27bd0010 	addiu	sp,sp,16
fbe48:	d8860000 	lv.q        C120, 0x0($a0)
fbe4c:	d8a70000 	lv.q        C130, 0x0($a1)
fbe50:	60868706 	vsub.t      C120, C130, C120
fbe54:	64868604 	vdot.t      S100, C120, C120
fbe58:	d0160404 	vsqrt.s     S100, S100
fbe5c:	48650004 	mfv         $a1, S100
fbe60:	44866800 	mtc1	a2,$f13
fbe64:	44856000 	mtc1	a1,$f12
fbe68:	46806b60 	cvt.s.w	$f13,$f13
fbe6c:	3c050037 	lui	a1,0x37
fbe70:	c4aec800 	lwc1	$f14,-14336(a1)
fbe74:	460d7342 	mul.s	$f13,$f14,$f13
fbe78:	460d6303 	div.s	$f12,$f12,$f13
fbe7c:	e48c0038 	swc1	$f12,56(a0)
fbe80:	03e00008 	jr	ra
fbe84:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcCamera::gcXYZMotion::Set(const mVec3 &, cTimeValue)
// Address: 0x000fbe00, Size: 136B
// Obj: gcAll_psp.obj

void gcCamera__gcXYZMotion__Set_constmVec3ref_cTimeValue(void *arg0, f32 arg1, s32 arg2) {
    s32 sp0;

    sp0 = arg2;
    __asm__ volatile("lv.q C120, ($a1)");
    if (arg2 != 0) {
        __asm__ volatile("sv.q C120, 0x10($a0)");
        __asm__ volatile("lv.q C120, ($a0)");
        __asm__ volatile("lv.q C130, ($a1)");
        __asm__ volatile("vsub.t C120, C130, C120");
        __asm__ volatile("vdot.t S100, C120, C120");
        __asm__ volatile("vsqrt.s S100, S100");
        __asm__ volatile("mfv $a1, S100");
        arg0->unk38 = (f32) (arg1 / (*(f32 *)0x36C800 * (f32) arg2));
        return;
    }
    __asm__ volatile("sv.q C120, 0x10($a0)");
    __asm__ volatile("sv.q C120, ($a0)");
    __asm__ volatile("mtv $a1, S120");
    __asm__ volatile("mtv $a2, S121");
    __asm__ volatile("mtv $a3, S122");
    __asm__ volatile("sv.q C120, 0x20($a0)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
