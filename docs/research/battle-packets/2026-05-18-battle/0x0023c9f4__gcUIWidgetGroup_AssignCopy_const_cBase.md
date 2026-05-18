# Battle Packet: `gcUIWidgetGroup::AssignCopy(const cBase *)`

## Target

- Address: `0x0023c9f4`
- Size: `428` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcUIWidgetGroup`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `129`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 47B; correct function size; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `47`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `47`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x000dd7cc` `failed` 436B `gcUIWidgetGroup::operator=(const gcUIWidgetGroup &)`

## Placement

- Canonical source: `src/gcUIWidgetGroup.cpp`
- Header(s): `include/gcUIWidgetGroup.h`
- Allowed source(s): `src/gcUIWidgetGroup.cpp`
- Split-TU prefix: `src/gcUIWidgetGroup_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcUIWidgetGroup.h`

```cpp
#ifndef GCUIWIDGETGROUP_H
#define GCUIWIDGETGROUP_H

#include "gcUIWidget.h"
#include "gcUIWidgetList.h"
#include "mVec2.h"

class cBase;
class cMemPool;
class cType;

class gcUIWidgetGroup : public gcUIWidget {
public:
    gcUIWidgetGroup(cBase *);
    static cBase *New(cMemPool *pool, cBase *parent);
    gcUIWidgetGroup &operator=(const gcUIWidgetGroup &);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    int NeedsEffectUpdate(void) const;
    void *GetResizableSprites(void);
    void Reset(cMemPool *pool, bool flag);
    void SetDimensions(int, int);
    void DeleteAllWidgets(bool);
    void InsertInDrawOrder(int, gcUIWidget *, gcUIWidget *);
    mVec2 GetCellPos(gcUICell) const;
    float GetCellPos(gcUICell, int) const;
};

#endif
```

## Matched Same-Class Neighbors

- `0x000dd548` 544B `gcUIWidgetGroup::Write(cFile &) const` — `src/gcUIWidgetGroup_Write.cpp`
- `0x000dd768` 100B `gcUIWidgetGroup::gcUIWidgetGroup(cBase *)` — `src/gcUIWidgetGroup.cpp`
- `0x0023cba0` 124B `gcUIWidgetGroup::New(cMemPool *, cBase *) static` — `src/gcUIWidgetGroup.cpp`
- `0x0023cc1c` 288B `gcUIWidgetGroup::GetType(void) const` — `src/gcUIWidgetGroup.cpp`
- `0x0023cf3c` 68B `gcUIWidgetGroup::NeedsEffectUpdate(void) const` — `src/gcUIWidgetGroup.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcUITextControl::AssignCopy(const cBase *)`

- Address: `0x00263934`
- Size: `368` bytes
- Source: `src/gcUITextControl.cpp`

```cpp
void gcUITextControl::AssignCopy(const cBase *base) {
    gcUITextControl *other = dcast<gcUITextControl *>(base);
    ((gcUIWidget *)this)->operator=(*(const gcUIWidget *)other);
    *(int *)((char *)this + 0xB0) = *(const int *)((const char *)other + 0xB0);

    int i = 0;
    int *dst = (int *)((char *)this + 0xB4);
    const int *src = (const int *)((const char *)other + 0xB4);
    do {
        i += 1;
        *dst = *src;
        dst += 1;
        src += 1;
    } while (i < 3);

    *(float *)((char *)this + 0xC0) = *(const float *)((const char *)other + 0xC0);
    const int *srcC8 = (const int *)((const char *)other + 0xC8);
    *(float *)((char *)this + 0xC4) = *(const float *)((const char *)other + 0xC4);
    int *dstC8 = (int *)((char *)this + 0xC8);
    unsigned int valueC8 = *srcC8;
    int *dstCC = (int *)((char *)this + 0xCC);
    *dstC8 = valueC8;
    const int *srcCC = (const int *)((const char *)other + 0xCC);
    *dstCC = *srcCC;

    ((cArrayBase<wchar_t> *)((char *)this + 0xD0))->operator=(
        *(const cArrayBase<wchar_t> *)((const char *)other + 0xD0));
    ((cArrayBase<float> *)((char *)this + 0xD4))->operator=(
        *(const cArrayBase<float> *)((const char *)other + 0xD4));
    ((cArrayBase<cArray<wchar_t> > *)((char *)this + 0xD8))->operator=(
        *(const cArrayBase<cArray<wchar_t> > *)((const char *)other + 0xD8));
    ((cArrayBase<cHandleT<eSprite> > *)((char *)this + 0xDC))->operator=(
        *(const cArrayBase<cHandleT<eSprite> > *)((const char *)other + 0xDC));

    int valueE0 = *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE0) = valueE0;
    const int *srcE4 = (const int *)((const char *)other + 0xE4);
    int *dstE4 = (int *)((char *)this + 0xE4);
    const int *srcE8 = (const int *)((const char *)other + 0xE8);
    *dstE4 = *srcE4;
    int *dstE8 = (int *)((char *)this + 0xE8);
    const int *srcEC = (const int *)((const char *)other + 0xEC);
    *dstE8 = *srcE8;
    int *dstEC = (int *)((char *)this + 0xEC);
    const int *srcF0 = (const int *)((const char *)other + 0xF0);
    *dstEC = *srcEC;
    int *dstF0 = (int *)((char *)this + 0xF0);
    const int *srcF4 = (const int *)((const char *)other + 0xF4);
    int *dstF4 = (int *)((char *)this + 0xF4);
    *dstF0 = *srcF0;
    int valueF4 = *srcF4;
    *dstF4 = valueF4;
    *(short *)((char *)this + 0xF8) = *(const short *)((const char *)other + 0xF8);
    *(unsigned short *)((char *)this + 0xFA) =
        *(const unsigned short *)((const char *)other + 0xFA);
    *(float *)((char *)this + 0xFC) = *(const float *)((const char *)other + 0xFC);
    *(float *)((char *)this + 0x100) = *(const float *)((const char *)other + 0x100);
    *(int *)((char *)this + 0x104) = *(const int *)((const char *)other + 0x104);

    int j = 0;
    TextControlShortCursor *dstShorts = (TextControlShortCursor *)this;
    const TextControlShortCursor *srcShorts =
        (const TextControlShortCursor *)other;
    do {
        dstShorts->value = *(const unsigned short *)&srcShorts->value;
        j += 1;
        srcShorts = (const TextControlShortCursor *)((const char *)srcShorts + 2);
        dstShorts = (TextControlShortCursor *)((char *)dstShorts + 2);
    } while (j < 4);
}
```

### Exemplar 2: `gcUIControl::AssignCopy(const cBase *)`

- Address: `0x00262f80`
- Size: `128` bytes
- Source: `src/gcUIControl.cpp`

```cpp
void gcUIControl::AssignCopy(const cBase *base) {
    gcUIControl *other = dcast<gcUIControl *>(base);
    ((gcUIWidget *)this)->operator=(*(const gcUIWidget *)other);
    *(int *)((char *)this + 0xB0) = *(const int *)((const char *)other + 0xB0);

    int i = 0;
    int *dst = (int *)((char *)this + 0xB4);
    const int *src = (const int *)((const char *)other + 0xB4);
    do {
        i += 1;
        *dst = *src;
        dst += 1;
        src += 1;
    } while (i < 3);

    *(float *)((char *)this + 0xC0) = *(const float *)((const char *)other + 0xC0);
    *(float *)((char *)this + 0xC4) = *(const float *)((const char *)other + 0xC4);
}
```

### Exemplar 3: `gcUIGeomTemplate::AssignCopy(const cBase *)`

- Address: `0x00290e54`
- Size: `108` bytes
- Source: `src/gcUIGeomTemplate.cpp`

```cpp
// ── gcUIGeomTemplate::AssignCopy(const cBase *) @ 0x00290e54 ──
void gcUIGeomTemplate::AssignCopy(const cBase *src) {
    gcUIGeomTemplate *other = dcast<gcUIGeomTemplate>(src);
    cObject::operator=(*other);
    mField44 = other->mField44;
    mField48 = other->mField48;
    mField4C = other->mField4C;
    mField50 = other->mField50;
    mEvent = other->mEvent;
}
```

### Exemplar 4: `gcUIControlString::AssignCopy(const cBase *)`

- Address: `0x0028f3d0`
- Size: `96` bytes
- Source: `src/gcUIControlString.cpp`

```cpp
//   gcUIControlString::AssignCopy(const cBase *)        @ 0x0028f3d0  ( 96B)
//   gcUIControlString::Write(cFile &) const             @ 0x0028f5e8  (112B)
//   gcUIControlString::~gcUIControlString(void)         @ 0x00290188  (100B)

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;
class gcUITextControl;

inline void *operator new(unsigned int, void *p) { return p; }
```


## Prior Failure Notes

### Note 1

- session `e897c9ba`; src `src/gcUIWidgetGroup.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/e897c9ba/0023c9f4__agent_self_reported_failure__src_gcUIWidgetGroup.cpp`

Kept the prior best C++ AssignCopy body and tested two different parent-type walk shapes: a do/while with match initialized up front, and an explicit label/fallthrough version meant to remove the final extra branch. Both variants were worse: the do/while shortened the function to 420B and moved the boolean into a1, while the label version increased the diff to 70/428 bytes. The current source is restored to the prior best 47/428-byte mismatch.

The remaining drift is still localized to the dynamic-cast success/failure lattice after the virtual GetType call. The target keeps the match flag in a0 and has the null parent case fall directly through to `li a0,0`; SNC keeps emitting an extra unconditional branch in the closest C shape. I also attempted one bounded permuter run, but tools/permuter.py could not isolate the target symbol from this multi-function translation unit and exited before searching.

### Note 2

- session `4bb9b048`

Matched the full local `GetType` singleton chain and the downcast walk in C++, then iterated several source shapes for the tail copy call. I tried a direct member `operator=` call, an explicit symbol-aliased helper call to `__0oPgcUIWidgetGroupasRC6PgcUIWidgetGroup`, versions with and without a copied `src` local, and multiple loop forms for the type-parent walk (`while`, `for(;;)`, and label/goto variants). The best version compiles to the correct 428-byte size and keeps the body very close to target, but `compare_func.py` still reports `47/428` differing bytes.

The useful checkpoint is that the mismatch is now localized. `GetType` is exact, and the `AssignCopy` body now reproduces the three-level `InitializeType` chain plus the virtual type getter and parent walk. Comparing against `expected/build/func/0023c9f4.o`, the remaining drift is in the prologue/register assignment and the final control-flow shape around the success/fail exit from the parent-type loop. The target wants `s0=this`, `s1=base`, `s2=other`, with the `beqz a1` / `move s1,a1` prologue and a specific `bnel a0,s3` parent-walk sequence. Current best is close but SNC still chooses a slightly different prologue and loop exit lattice. Retry from the current source in `src/gcUIWidgetGroup.cpp`; the most promising direction is last-mile shaping of the top-of-function local layout and the loop success/failure labels, not the type-init chain.


## Disassembly

```asm
23c9f4:	27bdffe0 	addiu	sp,sp,-32
23c9f8:	afb00000 	sw	s0,0(sp)
23c9fc:	afb20008 	sw	s2,8(sp)
23ca00:	34120000 	li	s2,0x0
23ca04:	00808025 	move	s0,a0
23ca08:	afb10004 	sw	s1,4(sp)
23ca0c:	afb3000c 	sw	s3,12(sp)
23ca10:	afb40010 	sw	s4,16(sp)
23ca14:	afb50014 	sw	s5,20(sp)
23ca18:	afb60018 	sw	s6,24(sp)
23ca1c:	afbf001c 	sw	ra,28(sp)
23ca20:	10a00052 	beqz	a1,0x23cb6c
23ca24:	00a08825 	move	s1,a1
23ca28:	3c13000a 	lui	s3,0xa
23ca2c:	8e649920 	lw	a0,-26336(s3)
23ca30:	14800037 	bnez	a0,0x23cb10
23ca34:	3c14000a 	lui	s4,0xa
23ca38:	8e84990c 	lw	a0,-26356(s4)
23ca3c:	1480002a 	bnez	a0,0x23cae8
23ca40:	3c080024 	lui	t0,0x24
23ca44:	3c150004 	lui	s5,0x4
23ca48:	8ea485e0 	lw	a0,-31264(s5)
23ca4c:	1480001b 	bnez	a0,0x23cabc
23ca50:	3c160004 	lui	s6,0x4
23ca54:	8ec485dc 	lw	a0,-31268(s6)
23ca58:	1480000e 	bnez	a0,0x23ca94
23ca5c:	3c08001c 	lui	t0,0x1c
23ca60:	3c040037 	lui	a0,0x37
23ca64:	3c050037 	lui	a1,0x37
23ca68:	34060001 	li	a2,0x1
23ca6c:	00003825 	move	a3,zero
23ca70:	00004025 	move	t0,zero
23ca74:	00004825 	move	t1,zero
23ca78:	00005025 	move	t2,zero
23ca7c:	00005825 	move	t3,zero
23ca80:	2484d894 	addiu	a0,a0,-10092
23ca84:	0c001d4d 	jal	0x7534
23ca88:	24a5d89c 	addiu	a1,a1,-10084
23ca8c:	aec285dc 	sw	v0,-31268(s6)
23ca90:	3c08001c 	lui	t0,0x1c
23ca94:	8ec785dc 	lw	a3,-31268(s6)
23ca98:	00002025 	move	a0,zero
23ca9c:	00002825 	move	a1,zero
23caa0:	34060002 	li	a2,0x2
23caa4:	00004825 	move	t1,zero
23caa8:	00005025 	move	t2,zero
23caac:	00005825 	move	t3,zero
23cab0:	0c001d4d 	jal	0x7534
23cab4:	25083c58 	addiu	t0,t0,15448
23cab8:	aea285e0 	sw	v0,-31264(s5)
23cabc:	8ea785e0 	lw	a3,-31264(s5)
23cac0:	00002025 	move	a0,zero
23cac4:	00002825 	move	a1,zero
23cac8:	34060084 	li	a2,0x84
23cacc:	00004025 	move	t0,zero
23cad0:	00004825 	move	t1,zero
23cad4:	00005025 	move	t2,zero
23cad8:	0c001d4d 	jal	0x7534
23cadc:	00005825 	move	t3,zero
23cae0:	ae82990c 	sw	v0,-26356(s4)
23cae4:	3c080024 	lui	t0,0x24
23cae8:	8e87990c 	lw	a3,-26356(s4)
23caec:	00002025 	move	a0,zero
23caf0:	00002825 	move	a1,zero
23caf4:	34060085 	li	a2,0x85
23caf8:	00004825 	move	t1,zero
23cafc:	00005025 	move	t2,zero
23cb00:	00005825 	move	t3,zero
23cb04:	0c001d4d 	jal	0x7534
23cb08:	2508cba0 	addiu	t0,t0,-13408
23cb0c:	ae629920 	sw	v0,-26336(s3)
23cb10:	8e240004 	lw	a0,4(s1)
23cb14:	8e739920 	lw	s3,-26336(s3)
23cb18:	24840008 	addiu	a0,a0,8
23cb1c:	84850000 	lh	a1,0(a0)
23cb20:	8c860004 	lw	a2,4(a0)
23cb24:	00c0f809 	jalr	a2
23cb28:	02252021 	addu	a0,s1,a1
23cb2c:	16600003 	bnez	s3,0x23cb3c
23cb30:	00402025 	move	a0,v0
23cb34:	1000000a 	b	0x23cb60
23cb38:	34040000 	li	a0,0x0
23cb3c:	50800008 	beqzl	a0,0x23cb60
23cb40:	34040000 	li	a0,0x0
23cb44:	54930003 	bnel	a0,s3,0x23cb54
23cb48:	8c84001c 	lw	a0,28(a0)
23cb4c:	10000004 	b	0x23cb60
23cb50:	34040001 	li	a0,0x1
23cb54:	1480fffb 	bnez	a0,0x23cb44
23cb58:	00000000 	nop
23cb5c:	34040000 	li	a0,0x0
23cb60:	10800003 	beqz	a0,0x23cb70
23cb64:	02002025 	move	a0,s0
23cb68:	02209025 	move	s2,s1
23cb6c:	02002025 	move	a0,s0
23cb70:	0c0375f3 	jal	0xdd7cc
23cb74:	02402825 	move	a1,s2
23cb78:	8fb00000 	lw	s0,0(sp)
23cb7c:	8fb10004 	lw	s1,4(sp)
23cb80:	8fb20008 	lw	s2,8(sp)
23cb84:	8fb3000c 	lw	s3,12(sp)
23cb88:	8fb40010 	lw	s4,16(sp)
23cb8c:	8fb50014 	lw	s5,20(sp)
23cb90:	8fb60018 	lw	s6,24(sp)
23cb94:	8fbf001c 	lw	ra,28(sp)
23cb98:	03e00008 	jr	ra
23cb9c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcUIWidgetGroup::AssignCopy(const cBase *)
// Address: 0x0023c9f4, Size: 428B
// Obj: gcAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? gcUIWidgetGroup__operator_eq_constgcUIWidgetGroupref(s32, void *); /* extern */

void gcUIWidgetGroup__AssignCopy_constcBaseptr(s32 arg0, void *arg1) {
    s16 temp_a1;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x99920 == 0) {
            if (*(s32 *)0x9990C == 0) {
                if (*(s32 *)0x385E0 == 0) {
                    if (*(s32 *)0x385DC == 0) {
                        *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
                    }
                    *(s32 *)0x385E0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 2, *(s32 *)0x385DC);
                }
                *(s32 *)0x9990C = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x84, *(s32 *)0x385E0);
            }
            *(s32 *)0x99920 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x85, *(s32 *)0x9990C);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x99920;
        temp_a1 = temp_a0->unk8;
        temp_a2 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a2(arg1 + temp_a1, temp_a1, temp_a2);
        if (temp_s3 == 0) {
            goto block_17;
        }
        if (var_a0 != NULL) {
loop_12:
            if (var_a0 != temp_s3) {
                var_a0 = var_a0->unk1C;
                if (var_a0 == NULL) {
                    goto block_17;
                }
                goto loop_12;
            }
            var_a0_2 = 1;
        } else {
block_17:
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            var_s2 = arg1;
        }
    }
    gcUIWidgetGroup__operator_eq_constgcUIWidgetGroupref(arg0, var_s2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
