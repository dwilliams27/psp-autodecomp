# Battle Packet: `gcUIWidget::Enable(bool)`

## Target

- Address: `0x000db2f0`
- Size: `132` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcUIWidget`
- Method family: `Enable`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000d8708` `untried` 436B `gcUIWidget::SetSizeDirty(void)`

## Placement

- Canonical source: `src/gcUIWidget.cpp`
- Header(s): `include/gcUIWidget.h`
- Allowed source(s): `src/gcUIWidget.cpp`
- Split-TU prefix: `src/gcUIWidget_*.cpp`

## Class Header: `include/gcUIWidget.h`

```cpp
#ifndef GCUIWIDGET_H
#define GCUIWIDGET_H

class mVec2;
class gcUIDialog;
class cTimeValue;
class gcEventStackData;
class cType;
class cBase;
class cFile;

class gcUIWidget {
public:
    char pad_000[0x20];
    gcUIDialog *mpDialog;       // 0x20
    unsigned int mFlags;        // 0x24

    enum gcUIEffect {};
    gcUIWidget(cBase *);
    ~gcUIWidget(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcUIDialog *GetParentDialog(void) const;
    void OnGetSize(mVec2 *, mVec2 *, unsigned int) const;
    void *GetResizableSprites(void);
    bool NeedsEffectUpdate(void) const;
    void InsertIntoDialog(gcUIDialog *);
    void InitialUpdate(void);
    void InitialUpdateUI(void);
    void CaptureFocus(bool);
    int IsUpdateEmpty(bool, bool) const;
    void UpdateUI(cTimeValue, const gcEventStackData &);
    void Focus(bool, bool);
};

#endif
```

## Matched Same-Class Neighbors

- `0x000d7c68` 260B `gcUIWidget::Write(cFile &) const` — `src/gcUIWidget.cpp`
- `0x000d8aa8` 36B `gcUIWidget::InsertIntoDialog(gcUIDialog *)` — `src/gcUIWidget_InsertIntoDialog.cpp`
- `0x000da350` 12B `gcUIWidget::GetResizableSprites(void)` — `src/gcUIWidget_GetResizableSprites.cpp`
- `0x000da724` 88B `gcUIWidget::InitialUpdate(void)` — `src/gcUIWidget_InitialUpdate.cpp`
- `0x000da77c` 88B `gcUIWidget::InitialUpdateUI(void)` — `src/gcUIWidget_InitialUpdateUI.cpp`

## Prior Failure Notes

### Note 1

- session `03a5259e`; src `src/gcUIWidget.cpp`; snapshot `logs/failure_snapshots/match_20260505_170107/03a5259e/000db2f0__agent_self_reported_failure__src_gcUIWidget.cpp`

gcUIWidget::Enable(bool) - 8 byte diff (124B vs 132B), 2 missing instructions: redundant `andi a1, a0, 0xff` (re-mask of enable byte) and `lw a0, 36(s0)` (mFlags reload). Expected register allocation puts enable byte in a0, mFlags in a2 with mFlags-mask in-place destroying original (`andi a2, a2, 1`); inside if-block compiler must reload mFlags via `lw a0, 36(s0)` in delay slot of `beqz a1`. Mine puts mFlags in a0 and mask in fresh a3, preserving original — no reload needed. Tried: bool-typed wasEnabled, pointer access via fp=&mFlags (matched size 132B but 51 bytes still differed due to addiu a1,s0,36 prelude), unsigned int f=mFlags reload variant — all CSE'd by SNC. Root cause: SNC's register allocator chooses different layout based on heuristics; my source structure produces a 'cleaner' allocation that doesn't require reload. Likely needs permuter to find a structure that triggers expected allocation, or this is compiler-divergent.


## Disassembly

```asm
db2f0:	27bdfff0 	addiu	sp,sp,-16
db2f4:	afb00000 	sw	s0,0(sp)
db2f8:	00808025 	move	s0,a0
db2fc:	8e060024 	lw	a2,36(s0)
db300:	30a400ff 	andi	a0,a1,0xff
db304:	34050000 	li	a1,0x0
db308:	30c60001 	andi	a2,a2,0x1
db30c:	afbf0004 	sw	ra,4(sp)
db310:	54c00001 	bnezl	a2,0xdb318
db314:	34050001 	li	a1,0x1
db318:	10850012 	beq	a0,a1,0xdb364
db31c:	308400ff 	andi	a0,a0,0xff
db320:	308500ff 	andi	a1,a0,0xff
db324:	10a00003 	beqz	a1,0xdb334
db328:	8e040024 	lw	a0,36(s0)
db32c:	10000003 	b	0xdb33c
db330:	34840001 	ori	a0,a0,0x1
db334:	2405fffe 	li	a1,-2
db338:	00852024 	and	a0,a0,a1
db33c:	ae040024 	sw	a0,36(s0)
db340:	0c0361c2 	jal	0xd8708
db344:	02002025 	move	a0,s0
db348:	8e040004 	lw	a0,4(s0)
db34c:	34050001 	li	a1,0x1
db350:	248400d8 	addiu	a0,a0,216
db354:	84860000 	lh	a2,0(a0)
db358:	8c870004 	lw	a3,4(a0)
db35c:	00e0f809 	jalr	a3
db360:	02062021 	addu	a0,s0,a2
db364:	8fb00000 	lw	s0,0(sp)
db368:	8fbf0004 	lw	ra,4(sp)
db36c:	03e00008 	jr	ra
db370:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcUIWidget::Enable(bool)
// Address: 0x000db2f0, Size: 132B
// Obj: gcAll_psp.obj

? gcUIWidget__SetSizeDirty_void(void *, s32, s32);  /* extern */

void gcUIWidget__Enable_bool(void *arg0, s32 arg1) {
    ? (*temp_a3)(void *, ?, s16, ?);
    s16 temp_a2_2;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a1_2;
    void *temp_a0_3;

    temp_a0 = arg1 & 0xFF;
    var_a1 = 0;
    temp_a2 = arg0->unk24 & 1;
    if (temp_a2 != 0) {
        var_a1 = 1;
    }
    if (temp_a0 != var_a1) {
        var_a1_2 = temp_a0 & 0xFF & 0xFF;
        temp_a0_2 = arg0->unk24;
        if (var_a1_2 != 0) {
            var_a0 = temp_a0_2 | 1;
        } else {
            var_a1_2 = -2;
            var_a0 = temp_a0_2 & ~1;
        }
        arg0->unk24 = var_a0;
        gcUIWidget__SetSizeDirty_void(arg0, var_a1_2, temp_a2);
        temp_a0_3 = arg0->unk4;
        temp_a2_2 = temp_a0_3->unkD8;
        temp_a3 = (temp_a0_3 + 0xD8)->unk4;
        temp_a3(arg0 + temp_a2_2, 1, temp_a2_2, temp_a3);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
