# Battle Packet: `gcUIWidget::Reset(cMemPool *, bool)`

## Target

- Address: `0x000d8654`
- Size: `180` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcUIWidget`
- Method family: `Reset`
- Leaf: `False`

## Queue Metadata

- score: `158`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; manageable size`
- failure_action: `prep`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `8`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x000d8708` `untried` 436B `gcUIWidget::SetSizeDirty(void)`

Callers:
- `0x000ddbf4` `failed` 476B `gcUIWidgetGroup::Reset(cMemPool *, bool)`

## Placement

- Canonical source: `src/gcUIWidget.cpp`
- Header(s): `include/gcUIWidget.h`
- Allowed source(s): `src/gcUIWidget.cpp`, `src/gcUIWidget_Reset.cpp`
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

## Matched Method Exemplars

### Exemplar 1: `gcUI::Reset(void)`

- Address: `0x000e0520`
- Size: `48` bytes
- Source: `src/gcEntityCustomAnimationGroup.cpp`

```cpp
void gcUI::Reset() {
    mFader0.Reset();
    mFader1.Reset();
}
```

### Exemplar 2: `gcUIDialog::Reset(cMemPool *, bool)`

- Address: `0x000e38cc`
- Size: `32` bytes
- Source: `src/gcUIDialog.cpp`

```cpp
void gcUIDialog::Reset(cMemPool *pool, bool flag) {
    ((gcUIWidgetGroup *)((char *)this + 0x98))->Reset(pool, true);
}
```

### Exemplar 3: `gcUIGeomTemplate::Reset(cMemPool *, bool)`

- Address: `0x0013bcd0`
- Size: `8` bytes
- Source: `src/gcUIGeomTemplate.cpp`

```cpp
void gcUIGeomTemplate::Reset(cMemPool *, bool) {
}
```

### Exemplar 4: `gcRoomInstance::Reset(cMemPool *, bool)`

- Address: `0x0013c6e8`
- Size: `176` bytes
- Source: `src/gcRoomInstance.cpp`

```cpp
void gcRoomInstance::Reset(cMemPool *, bool) {
    int value = *(int *)((char *)this + 0x44);
    int valid;
    if (value == 0) {
        valid = 0;
    } else {
        HandleEntry *entry = D_00038890[value & 0xFFFF];
        HandleEntry *found = 0;
        if (entry != 0) {
            if (entry->handle == value) {
                found = entry;
            }
        }
        valid = found != 0;
    }

    if ((valid & 0xFF) != 0) {
        HandleEntry *entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        int *nextRoom = (int *)((char *)this + 0x48);
        int next = *nextRoom;
        __asm__ volatile(
            "addiu $a3, %1, 0x00e8\n"
            "sw %0, 0($a3)"
            :
            : "r"(next), "r"(entry)
            : "a3", "memory");

        value = *(volatile int *)((char *)this + 0x44);
        entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        entry->roomInstance = (int)this;
    }
}
```


## Prior Failure Notes

### Note 1

- session `7f3dc83c`; src `src/gcUIWidget_Reset.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/7f3dc83c/000d8654__agent_self_reported_failure__src_gcUIWidget_Reset.cpp`

Closest retained attempt is the split-TU Reset source, which compiles to 172B vs expected 180B. It matches the high-level loop and final SetSizeDirty/flag stores, but still omits the pair.first round-trip reload in the loop body: expected has sw t0,0(sp), lw t1,0(sp), later move t0,t1 before storing handle->first, while this source keeps the first value live and stores it directly. I also tested canonical gcUIWidget.cpp with temporary header declarations; it produced the same 172B shape, so the shared header edits were reverted per split-TU guardrails.

Tried volatile Pair, volatile pair.first pointer, cast-to-volatile field read, explicit handle-second pointer materialization, fixed register declarations, and declaration-order changes. Some variants force the missing lw but introduce extra post-loop reloads or reorder the loop into a 39-99 byte mismatch. permuter.py was run with --time 300 --save-best, but skipped because the candidate symbol size is 172B and the DB target is 180B. Root cause remains forcing pair.first to be re-read from stack without making SNC spill/reorder the rest of the loop.

### Note 2

- session `03a5259e`; src `src/gcUIWidget.cpp`; snapshot `logs/failure_snapshots/match_20260505_170107/03a5259e/000d8654__agent_self_reported_failure__src_gcUIWidget.cpp`

gcUIWidget::Reset(cMemPool *, bool) - 8 byte diff (172B vs 180B), missing 2 instructions in loop body: `lw t1, 0(sp)` (round-trip reload pair.first) and `move t0, t1`. Mine uses pair struct with `int *pairSecondPtr = &pair.second` and `__asm__ volatile("" ::: "memory")` barrier, which forces round-trip on pair.second but NOT pair.first. Expected has BOTH fields round-trip through memory. Tried: explicit pairFirstPtr (matched 180B but added `move t0, sp` extra and 62 bytes differed), constructor-style `*handle = pair` (eliminated pair entirely), volatile pair2. Root cause: SNC at -O2 optimizes through memory barrier for direct member access (handle->first = pair.first) when pair.first reg is still alive after the store. Need a structure that forces pair.first to be re-read from memory without adding extra pointer-setup instructions. The post-loop `pair2.first = pair.first; pair2.second = pair.first` works (with __asm__ "m"(pair2) barrier) — that part lines up. The trailing setup using live regs t1 and t0 from the last loop iteration also matches. The remaining issue is just the loop body's pair.first round-trip pattern.


## Disassembly

```asm
d8654:	27bdffe0 	addiu	sp,sp,-32
d8658:	afb00010 	sw	s0,16(sp)
d865c:	00808025 	move	s0,a0
d8660:	8e040024 	lw	a0,36(s0)
d8664:	2405fffb 	li	a1,-5
d8668:	00852024 	and	a0,a0,a1
d866c:	ae040024 	sw	a0,36(s0)
d8670:	34070000 	li	a3,0x0
d8674:	27a40004 	addiu	a0,sp,4
d8678:	2606002c 	addiu	a2,s0,44
d867c:	26050030 	addiu	a1,s0,48
d8680:	afbf0014 	sw	ra,20(sp)
d8684:	8e090020 	lw	t1,32(s0)
d8688:	34080000 	li	t0,0x0
d868c:	55200001 	bnezl	t1,0xd8694
d8690:	8d280030 	lw	t0,48(t1)
d8694:	afa80000 	sw	t0,0(sp)
d8698:	8fa90000 	lw	t1,0(sp)
d869c:	8ca80000 	lw	t0,0(a1)
d86a0:	24cb0004 	addiu	t3,a2,4
d86a4:	afa80004 	sw	t0,4(sp)
d86a8:	01204025 	move	t0,t1
d86ac:	acc80000 	sw	t0,0(a2)
d86b0:	8c8a0000 	lw	t2,0(a0)
d86b4:	24e70001 	addiu	a3,a3,1
d86b8:	ad6a0000 	sw	t2,0(t3)
d86bc:	24c60008 	addiu	a2,a2,8
d86c0:	28ea0004 	slti	t2,a3,4
d86c4:	1540ffef 	bnez	t2,0xd8684
d86c8:	24a50008 	addiu	a1,a1,8
d86cc:	afa90008 	sw	t1,8(sp)
d86d0:	afa8000c 	sw	t0,12(sp)
d86d4:	0c0361c2 	jal	0xd8708
d86d8:	02002025 	move	a0,s0
d86dc:	8e040024 	lw	a0,36(s0)
d86e0:	3c050100 	lui	a1,0x100
d86e4:	00852025 	or	a0,a0,a1
d86e8:	ae040024 	sw	a0,36(s0)
d86ec:	3c050080 	lui	a1,0x80
d86f0:	00852025 	or	a0,a0,a1
d86f4:	ae040024 	sw	a0,36(s0)
d86f8:	8fb00010 	lw	s0,16(sp)
d86fc:	8fbf0014 	lw	ra,20(sp)
d8700:	03e00008 	jr	ra
d8704:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcUIWidget::Reset(cMemPool *, bool)
// Address: 0x000d8654, Size: 180B
// Obj: gcAll_psp.obj

? gcUIWidget__SetSizeDirty_void(void *, s32 *, void *, s32); /* extern */

void gcUIWidget__Reset_cMemPoolptr_bool(void *arg0) {
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 *var_a1;
    s32 temp_a0;
    s32 var_a3;
    void *var_a2;

    arg0->unk24 = (s32) (arg0->unk24 & ~4);
    var_a3 = 0;
    var_a2 = arg0 + 0x2C;
    var_a1 = arg0 + 0x30;
    do {
        if (arg0->unk20 != NULL) {

        }
        sp4 = *var_a1;
        var_a2->unk0 = (s32) subroutine_arg0;
        var_a3 += 1;
        var_a2->unk4 = (s32) sp4;
        var_a2 += 8;
        var_a1 += 8;
    } while (var_a3 < 4);
    sp8 = subroutine_arg0;
    spC = subroutine_arg0;
    gcUIWidget__SetSizeDirty_void(arg0, var_a1, var_a2, var_a3);
    temp_a0 = arg0->unk24 | 0x01000000;
    arg0->unk24 = temp_a0;
    arg0->unk24 = (s32) (temp_a0 | 0x800000);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
