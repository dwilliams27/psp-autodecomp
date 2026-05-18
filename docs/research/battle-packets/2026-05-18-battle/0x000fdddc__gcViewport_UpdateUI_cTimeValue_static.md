# Battle Packet: `gcViewport::UpdateUI(cTimeValue) static`

## Target

- Address: `0x000fdddc`
- Size: `156` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcViewport`
- Method family: `UpdateUI`
- Leaf: `False`

## Queue Metadata

- score: `159`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 12B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000e0550` `untried` 324B `gcUI::Update(cTimeValue)`

Callers:
- `0x00222078` `untried` 388B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcBackgroundLoader> >(gcRenderLoopT *)`
- `0x00225cc8` `untried` 456B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<eMoviePauseExitCheck> >(gcRenderLoopT *)`
- `0x00225e90` `untried` 400B `void gcRenderLoopT<fileSystemErrorLoop>(gcRenderLoopT *)`
- `0x00226020` `untried` 412B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcUIOpenDialogExitCheck> >(gcRenderLoopT *)`
- `0x00226b00` `untried` 516B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcMapServerReadyExitCheck> >(gcRenderLoopT *)`
- `0x00226ee8` `untried` 524B `void gcRenderLoopT<gcUIFullscreenRenderLoopT<gcRegionLoaderExitCheck> >(gcRenderLoopT *)`

## Placement

- Canonical source: `src/gcViewport.cpp`
- Header(s): `include/gcViewport.h`
- Allowed source(s): `src/gcViewport.cpp`
- Split-TU prefix: `src/gcViewport_*.cpp`

## Class Header: `include/gcViewport.h`

Not found.

## Matched Same-Class Neighbors

- `0x000ccd94` 148B `gcViewport::~gcViewport(void)` — `src/gcViewport.cpp`
- `0x000fdadc` 40B `gcViewport::Write(cFile &) const` — `src/eRoom.cpp`
- `0x000fdb04` 120B `gcViewport::Read(cFile &, cMemPool *)` — `src/gcViewport.cpp`
- `0x000fdb7c` 76B `gcViewport::gcViewport(cBase *)` — `src/gcViewport.cpp`
- `0x000fdbc8` 108B `gcViewport::Initialize(void) static` — `src/gcViewport.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcUIWidgetList::UpdateUI(cTimeValue, const gcEventStackData &)`

- Address: `0x0013185c`
- Size: `100` bytes
- Source: `src/gcUIWidgetList.cpp`

```cpp
// ── gcUIWidgetList::UpdateUI(cTimeValue, const gcEventStackData &) @ 0x0013185c ──
void gcUIWidgetList::UpdateUI(cTimeValue t, const gcEventStackData &d) {
    this->FillCurVisibleItems();
    gcUIWidget_UpdateUI(this, t, d);
    gcUIWidgetGroup_UpdateChildEffects(this, t);
    this->FocusCurCell(true);
}
```

### Exemplar 2: `gcUIEditBox::UpdateUI(cTimeValue, const gcEventStackData &)`

- Address: `0x0013ab1c`
- Size: `60` bytes
- Source: `src/gcUIEditBox.cpp`

```cpp
void gcUIEditBox::UpdateUI(cTimeValue time, const gcEventStackData &evt) {
    ((gcUIWidget *)this)->UpdateUI(time, evt);
    HandleBlink(time);
}
```


## Prior Failure Notes

### Note 1

- session `cf7a051d`; src `src/gcViewport.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/cf7a051d/000fdddc__agent_self_reported_failure__src_gcViewport.cpp`

gcViewport::UpdateUI(cTimeValue) static — 12/156 bytes differ. SAME compiler quirk as the previously failed UpdateAll at 0x000fdd44 (sibling function, identical structure documented in functions.json session 78471c89). Function logic is fully correct: do-while loop (5 iters) testing viewport[offset+0x1380], calling gcUI::Update(t) at viewport+offset+0x10+0x11E4, then post-loop call to gcUI::Update at 0x99928 if D_0037D840[0xED0]==0.

The 12-byte diff is purely register allocation: SNC chose s1=global_high, s2=offset; expected has s2=global_high, s1=offset. The s1/s2 swap propagates to: prologue saves (sw s1/s2 at swapped stack offsets), the lui of global high register, two `lw a0,...(sN)` reloads, two `addu a1,a0,sN` (one with operand order also flipped: expected `addu a1,a0,s1` vs mine `addu a1,s2,a0`), and the offset-increment `addiu sN,sN,5008`.

UpdateAll attempted: declaring offset before i, intermediate `char *vp = D_0037D840 + offset`, array-index form `((unsigned char*)D_0037D840)[offset+0x1380]`, plus 90s/874-candidate permuter run — all zero improvement. SNC's interference-graph coloring is loop-shape-dependent and not source-controllable. Sibling matched methods in this same file (CloseAllDialogs at 0xfe024, OnMapEnded at 0xfe134, OnMemPoolResetAll at 0xfe1a4, MemoryCardReplicateDynamic at 0xfe484) allocate global_high to varying registers, so there's no single allocation rule. Likely needs SNC pspcor.exe binary patching to fix the allocator preference, similar to docs/decisions/006-bnel-compiler-divergence.md but for register coloring rather than branch-likely heuristics. Diff exceeds the 8-byte unmatchable threshold (and isn't bnel/beql), so stays 'failed'.


## Disassembly

```asm
fdddc:	27bdffe0 	addiu	sp,sp,-32
fdde0:	afa40000 	sw	a0,0(sp)
fdde4:	afb00004 	sw	s0,4(sp)
fdde8:	afb2000c 	sw	s2,12(sp)
fddec:	8fb00000 	lw	s0,0(sp)
fddf0:	3c120038 	lui	s2,0x38
fddf4:	afb10008 	sw	s1,8(sp)
fddf8:	afb30010 	sw	s3,16(sp)
fddfc:	34130000 	li	s3,0x0
fde00:	34110000 	li	s1,0x0
fde04:	8e44d840 	lw	a0,-10176(s2)
fde08:	afbf0014 	sw	ra,20(sp)
fde0c:	00912821 	addu	a1,a0,s1
fde10:	90a51380 	lbu	a1,4992(a1)
fde14:	50a00008 	beqzl	a1,0xfde38
fde18:	26730001 	addiu	s3,s3,1
fde1c:	00912021 	addu	a0,a0,s1
fde20:	24840010 	addiu	a0,a0,16
fde24:	248411e4 	addiu	a0,a0,4580
fde28:	0c038154 	jal	0xe0550
fde2c:	02002825 	move	a1,s0
fde30:	8e44d840 	lw	a0,-10176(s2)
fde34:	26730001 	addiu	s3,s3,1
fde38:	2a650005 	slti	a1,s3,5
fde3c:	14a0fff3 	bnez	a1,0xfde0c
fde40:	26311390 	addiu	s1,s1,5008
fde44:	90840ed0 	lbu	a0,3792(a0)
fde48:	14800004 	bnez	a0,0xfde5c
fde4c:	3c04000a 	lui	a0,0xa
fde50:	02002825 	move	a1,s0
fde54:	0c038154 	jal	0xe0550
fde58:	24849928 	addiu	a0,a0,-26328
fde5c:	8fb00004 	lw	s0,4(sp)
fde60:	8fb10008 	lw	s1,8(sp)
fde64:	8fb2000c 	lw	s2,12(sp)
fde68:	8fb30010 	lw	s3,16(sp)
fde6c:	8fbf0014 	lw	ra,20(sp)
fde70:	03e00008 	jr	ra
fde74:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcViewport::UpdateUI(cTimeValue) static
// Address: 0x000fdddc, Size: 156B
// Obj: gcAll_psp.obj

? gcUI__Update_cTimeValue(s32, s32);                /* extern */

void gcViewport__UpdateUI_cTimeValuestatic(s32 arg0) {
    s32 var_s1;
    s32 var_s3;

    var_s3 = 0;
    var_s1 = 0;
    do {
        if ((*(void **)0x37D840 + var_s1)->unk1380 != 0) {
            gcUI__Update_cTimeValue(*(void **)0x37D840 + var_s1 + 0x10 + 0x11E4, subroutine_arg0);
        }
        var_s3 += 1;
        var_s1 += 0x1390;
    } while (var_s3 < 5);
    if ((*(void **)0x37D840)->unkED0 == 0) {
        gcUI__Update_cTimeValue(0x99928, subroutine_arg0);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
