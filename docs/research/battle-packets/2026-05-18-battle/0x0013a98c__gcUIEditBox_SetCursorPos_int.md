# Battle Packet: `gcUIEditBox::SetCursorPos(int)`

## Target

- Address: `0x0013a98c`
- Size: `148` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcUIEditBox`
- Method family: `SetCursorPos`
- Leaf: `False`

## Queue Metadata

- score: `167`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 13B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `13`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `13`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00001314` `untried` 84B `cStrLength(const wchar_t *)`
- `0x000d8708` `untried` 436B `gcUIWidget::SetSizeDirty(void)`

Callers:
- `0x0013a730` `untried` 328B `gcUIEditBox::InsertString(const wchar_t *, int)`
- `0x0013a878` `untried` 236B `gcUIEditBox::DeleteString(int, int)`
- `0x0013aa20` `matched` 32B `gcUIEditBox::DeltaCursorPos(int)`
- `0x0013aa40` `untried` 220B `gcUIEditBox::Update(cTimeValue, const gcEventStackData &)`
- `0x0013acc4` `untried` 320B `gcUIEditBox::OnTextChanged(void)`
- `0x0013ae04` `untried` 500B `gcUIEditBox::HandleKeyboard(void)`
- `0x0030943c` `untried` 1088B `gcDoUIEditBoxOp::Evaluate(void) const`

## Placement

- Canonical source: `src/gcUIEditBox.cpp`
- Header(s): `include/gcUIEditBox.h`
- Allowed source(s): `src/gcUIEditBox.cpp`
- Split-TU prefix: `src/gcUIEditBox_*.cpp`

## Class Header: `include/gcUIEditBox.h`

```cpp
#ifndef GCUIEDITBOX_H
#define GCUIEDITBOX_H

#include "gcUIWidget.h"
#include "gcCinematic.h"  // for cHandleT

class eSprite;
class cBase;
class cTimeValue;
class gcEventStackData;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct gcUIEditBoxDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcUITextControl {
public:
    char pad_text_control[0x110];

    gcUITextControl(cBase *);
    ~gcUITextControl();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class gcUIEditBox : public gcUITextControl {
public:
    unsigned int mEditFlags;           // 0x110, bit 0x80000000 = cursor visible
    cHandleT<eSprite> mCursorSprite;   // 0x114
    char pad_118[0x4];                 // 0x118 (unknown)
    int mCursorPos;                    // 0x11C

    gcUIEditBox(cBase *);
    ~gcUIEditBox();
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcUIEditBoxDeleteRecord *rec =
            (gcUIEditBoxDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }

    const cType *GetType(void) const;
    int GetCursor(cHandleT<eSprite> *) const;
    void DeltaCursorPos(int);
    void Backspace(void);
    void UpdateUI(cTimeValue, const gcEventStackData &);
    void Focus(bool, bool);
    void Write(cFile &) const;

    void SetCursorPos(int);
    void DeleteString(int, int);
    void HandleBlink(cTimeValue);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0013a644` 124B `gcUIEditBox::~gcUIEditBox(void)` — `src/gcUIEditBox.cpp`
- `0x0013a964` 40B `gcUIEditBox::Backspace(void)` — `src/gcUIEditBox.cpp`
- `0x0013aa20` 32B `gcUIEditBox::DeltaCursorPos(int)` — `src/gcUIEditBox.cpp`
- `0x0013ab1c` 60B `gcUIEditBox::UpdateUI(cTimeValue, const gcEventStackData &)` — `src/gcUIEditBox.cpp`
- `0x0013ab58` 68B `gcUIEditBox::Focus(bool, bool)` — `src/gcUIEditBox.cpp`

## Prior Failure Notes

### Note 1

- session `65a5a0de`; src `src/gcUIEditBox.cpp`; snapshot `logs/failure_snapshots/match_20260505_170107/65a5a0de/0013a98c__agent_self_reported_failure__src_gcUIEditBox.cpp`

gcUIEditBox::SetCursorPos(int) (148B). Implemented as: load len from cStrLength when mpString!=0, clamp pos into newPos with `newPos=0; if(pos>0){newPos=pos; if(newPos>=len) newPos=len;}`, then `if(newPos!=mCursorPos){ mCursorPos=newPos; mEditFlags|=0x80000000; SetSizeDirty(); copy 0x118->0x120; }`. SetSizeDirty called via extern "C" __0fKgcUIWidgetMSetSizeDirtyv(this) since I can't add the declaration to gcUIWidget.h (header is intentional/locked).

Result: 13/148 bytes diff. Three remaining diffs all stem from target keeping newPos in callee-saved s0 across the !=mCursorPos branch, while my build keeps it in a1: (1) `slt a2, s0, a0` (target) vs `slt a2, a1, a0` (mine), (2) `move s0, a1` in beq delay slot (target) vs `nop` (mine), (3) `sw s0, 0x11c(s1)` (target) vs `sw a1, 0x11c(s1)` (mine). The downstream code is byte-identical: `lw a0,0x110(s1); lui a1,0x8000; or; sw a0,0x110(s1); jal SetSizeDirty; struct copy 0x118->0x120`.

Tried: pos>=len vs newPos>=len bound check (worse: 22/148), permuter 180s/2359 candidates (zero improvement from 13). Root cause: SNC's register allocator doesn't promote newPos to s0 for my source structure even though s0 is otherwise free; target's compiler chose s0 because newPos must survive across the lui-a1 sequence (a1 is clobbered) — target uses delay-slot move-to-s0 pattern; mine reorders to do the sw before the lui so a1 survives just long enough. The choice is purely scheduler/regalloc determined and resists source perturbation. Trying volatile, reordering the assignment with mEditFlags first, and explicit local copies all produced the same a1-based codegen. Falls under the documented SNC scheduling/register-allocation divergence class, not a logical mismatch.


## Disassembly

```asm
13a98c:	27bdfff0 	addiu	sp,sp,-16
13a990:	afb00000 	sw	s0,0(sp)
13a994:	afb10004 	sw	s1,4(sp)
13a998:	00808825 	move	s1,a0
13a99c:	00a08025 	move	s0,a1
13a9a0:	8e2500d0 	lw	a1,208(s1)
13a9a4:	afbf0008 	sw	ra,8(sp)
13a9a8:	10a00004 	beqz	a1,0x13a9bc
13a9ac:	34040000 	li	a0,0x0
13a9b0:	0c0004c5 	jal	0x1314
13a9b4:	00a02025 	move	a0,a1
13a9b8:	00402025 	move	a0,v0
13a9bc:	1a000005 	blez	s0,0x13a9d4
13a9c0:	34050000 	li	a1,0x0
13a9c4:	02002825 	move	a1,s0
13a9c8:	0204302a 	slt	a2,s0,a0
13a9cc:	50c00001 	beqzl	a2,0x13a9d4
13a9d0:	00802825 	move	a1,a0
13a9d4:	8e24011c 	lw	a0,284(s1)
13a9d8:	10a4000c 	beq	a1,a0,0x13aa0c
13a9dc:	00a08025 	move	s0,a1
13a9e0:	8e240110 	lw	a0,272(s1)
13a9e4:	3c058000 	lui	a1,0x8000
13a9e8:	ae30011c 	sw	s0,284(s1)
13a9ec:	00852025 	or	a0,a0,a1
13a9f0:	ae240110 	sw	a0,272(s1)
13a9f4:	0c0361c2 	jal	0xd8708
13a9f8:	02202025 	move	a0,s1
13a9fc:	26240118 	addiu	a0,s1,280
13aa00:	8c840000 	lw	a0,0(a0)
13aa04:	26250120 	addiu	a1,s1,288
13aa08:	aca40000 	sw	a0,0(a1)
13aa0c:	8fb00000 	lw	s0,0(sp)
13aa10:	8fb10004 	lw	s1,4(sp)
13aa14:	8fbf0008 	lw	ra,8(sp)
13aa18:	03e00008 	jr	ra
13aa1c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcUIEditBox::SetCursorPos(int)
// Address: 0x0013a98c, Size: 148B
// Obj: gcAll_psp.obj

s32 cStrLength_constwchar_tptr(s32, s32);           /* extern */
? gcUIWidget__SetSizeDirty_void(void *, ?);         /* extern */

void gcUIEditBox__SetCursorPos_int(void *arg0, s32 arg1) {
    s32 temp_a1;
    s32 var_a0;
    s32 var_a1;

    temp_a1 = arg0->unkD0;
    var_a0 = 0;
    if (temp_a1 != 0) {
        var_a0 = cStrLength_constwchar_tptr(temp_a1, temp_a1);
    }
    var_a1 = 0;
    if (arg1 > 0) {
        var_a1 = arg1;
        if (arg1 >= var_a0) {
            var_a1 = var_a0;
        }
    }
    if (var_a1 != arg0->unk11C) {
        arg0->unk11C = var_a1;
        arg0->unk110 = (s32) (arg0->unk110 | 0x80000000);
        gcUIWidget__SetSizeDirty_void(arg0, 0x80000000);
        arg0->unk120 = (s32) arg0->unk118;
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
