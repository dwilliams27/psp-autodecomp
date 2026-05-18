# Battle Packet: `gcUIGeom::AssignCopy(const cBase *)`

## Target

- Address: `0x00290b1c`
- Size: `420` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcUIGeom`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `126`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 29B; correct function size; AssignCopy retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `29`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `needs_layout`
- Near miss: `29`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0013aff8` `untried` 624B `gcUIGeom::operator=(const gcUIGeom &)`

## Placement

- Canonical source: `src/gcUIGeom.cpp`
- Header(s): `include/gcUIGeom.h`
- Allowed source(s): `src/gcUIGeom.cpp`
- Split-TU prefix: `src/gcUIGeom_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcUIGeom.h`

Not found.

## Matched Same-Class Neighbors

- `0x0013b268` 76B `gcUIGeom::Write(cFile &) const` — `src/gcUIGeom.cpp`
- `0x0013b2b4` 188B `gcUIGeom::Read(cFile &, cMemPool *)` — `src/gcUIGeom.cpp`
- `0x0013b370` 68B `gcUIGeom::gcUIGeom(cBase *)` — `src/gcUIGeom.cpp`
- `0x0013b3b4` 164B `gcUIGeom::~gcUIGeom(void)` — `src/gcUIGeom.cpp`
- `0x0013b8c0` 88B `gcUIGeom::Update(cTimeValue)` — `src/gcUIGeom.cpp`

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

- session `cb0ce03c`

Implemented gcUIGeom::AssignCopy as real C++: inline type initialization matching GetType, virtual GetType dispatch through the vtable entry at +8, cType parent-chain walk, and final gcUIGeom::operator= call. The best result is the correct 420-byte size with 29/420 bytes differing; all existing matched functions in src/gcUIGeom.cpp still compare as before. The remaining diff is localized to the ancestry-test branch shape after the jalr: expected uses the bnez/beqzl/bnel pattern shown in the target disassembly, while the best C form still differs in the target/type null checks and loop branch layout. I tried structured if/else, do/while, while, explicit goto forms, boolean-vs-pointer result variables, and inverted equality tests. A permuter run was attempted, but tools/permuter.py aborted because it could not locate this target inside the multi-function compiled .o (found whole .text instead of the 420B function), so no automated mutations ran.


## Disassembly

```asm
290b1c:	27bdffe0 	addiu	sp,sp,-32
290b20:	afb00000 	sw	s0,0(sp)
290b24:	afb20008 	sw	s2,8(sp)
290b28:	34120000 	li	s2,0x0
290b2c:	00808025 	move	s0,a0
290b30:	afb10004 	sw	s1,4(sp)
290b34:	afb3000c 	sw	s3,12(sp)
290b38:	afb40010 	sw	s4,16(sp)
290b3c:	afb50014 	sw	s5,20(sp)
290b40:	afb60018 	sw	s6,24(sp)
290b44:	afbf001c 	sw	ra,28(sp)
290b48:	10a00050 	beqz	a1,0x290c8c
290b4c:	00a08825 	move	s1,a1
290b50:	3c13000a 	lui	s3,0xa
290b54:	8e64f58c 	lw	a0,-2676(s3)
290b58:	14800035 	bnez	a0,0x290c30
290b5c:	3c140004 	lui	s4,0x4
290b60:	8e8469c0 	lw	a0,27072(s4)
290b64:	14800028 	bnez	a0,0x290c08
290b68:	3c080029 	lui	t0,0x29
290b6c:	3c150004 	lui	s5,0x4
290b70:	8ea40ff4 	lw	a0,4084(s5)
290b74:	14800019 	bnez	a0,0x290bdc
290b78:	3c160004 	lui	s6,0x4
290b7c:	8ec485dc 	lw	a0,-31268(s6)
290b80:	1480000c 	bnez	a0,0x290bb4
290b84:	3c040037 	lui	a0,0x37
290b88:	3c050037 	lui	a1,0x37
290b8c:	34060001 	li	a2,0x1
290b90:	00003825 	move	a3,zero
290b94:	00004025 	move	t0,zero
290b98:	00004825 	move	t1,zero
290b9c:	00005025 	move	t2,zero
290ba0:	00005825 	move	t3,zero
290ba4:	2484d894 	addiu	a0,a0,-10092
290ba8:	0c001d4d 	jal	0x7534
290bac:	24a5d89c 	addiu	a1,a1,-10084
290bb0:	aec285dc 	sw	v0,-31268(s6)
290bb4:	8ec785dc 	lw	a3,-31268(s6)
290bb8:	00002025 	move	a0,zero
290bbc:	00002825 	move	a1,zero
290bc0:	34060016 	li	a2,0x16
290bc4:	00004025 	move	t0,zero
290bc8:	00004825 	move	t1,zero
290bcc:	00005025 	move	t2,zero
290bd0:	0c001d4d 	jal	0x7534
290bd4:	00005825 	move	t3,zero
290bd8:	aea20ff4 	sw	v0,4084(s5)
290bdc:	8ea70ff4 	lw	a3,4084(s5)
290be0:	00002025 	move	a0,zero
290be4:	00002825 	move	a1,zero
290be8:	34060017 	li	a2,0x17
290bec:	00004025 	move	t0,zero
290bf0:	00004825 	move	t1,zero
290bf4:	00005025 	move	t2,zero
290bf8:	0c001d4d 	jal	0x7534
290bfc:	00005825 	move	t3,zero
290c00:	ae8269c0 	sw	v0,27072(s4)
290c04:	3c080029 	lui	t0,0x29
290c08:	8e8769c0 	lw	a3,27072(s4)
290c0c:	00002025 	move	a0,zero
290c10:	00002825 	move	a1,zero
290c14:	34060082 	li	a2,0x82
290c18:	00004825 	move	t1,zero
290c1c:	00005025 	move	t2,zero
290c20:	00005825 	move	t3,zero
290c24:	0c001d4d 	jal	0x7534
290c28:	25080cc0 	addiu	t0,t0,3264
290c2c:	ae62f58c 	sw	v0,-2676(s3)
290c30:	8e240004 	lw	a0,4(s1)
290c34:	8e73f58c 	lw	s3,-2676(s3)
290c38:	24840008 	addiu	a0,a0,8
290c3c:	84850000 	lh	a1,0(a0)
290c40:	8c860004 	lw	a2,4(a0)
290c44:	00c0f809 	jalr	a2
290c48:	02252021 	addu	a0,s1,a1
290c4c:	16600003 	bnez	s3,0x290c5c
290c50:	00402025 	move	a0,v0
290c54:	1000000a 	b	0x290c80
290c58:	34040000 	li	a0,0x0
290c5c:	50800008 	beqzl	a0,0x290c80
290c60:	34040000 	li	a0,0x0
290c64:	54930003 	bnel	a0,s3,0x290c74
290c68:	8c84001c 	lw	a0,28(a0)
290c6c:	10000004 	b	0x290c80
290c70:	34040001 	li	a0,0x1
290c74:	1480fffb 	bnez	a0,0x290c64
290c78:	00000000 	nop
290c7c:	34040000 	li	a0,0x0
290c80:	10800003 	beqz	a0,0x290c90
290c84:	02002025 	move	a0,s0
290c88:	02209025 	move	s2,s1
290c8c:	02002025 	move	a0,s0
290c90:	0c04ebfe 	jal	0x13aff8
290c94:	02402825 	move	a1,s2
290c98:	8fb00000 	lw	s0,0(sp)
290c9c:	8fb10004 	lw	s1,4(sp)
290ca0:	8fb20008 	lw	s2,8(sp)
290ca4:	8fb3000c 	lw	s3,12(sp)
290ca8:	8fb40010 	lw	s4,16(sp)
290cac:	8fb50014 	lw	s5,20(sp)
290cb0:	8fb60018 	lw	s6,24(sp)
290cb4:	8fbf001c 	lw	ra,28(sp)
290cb8:	03e00008 	jr	ra
290cbc:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcUIGeom::AssignCopy(const cBase *)
// Address: 0x00290b1c, Size: 420B
// Obj: gcAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? gcUIGeom__operator_eq_constgcUIGeomref(s32, void *); /* extern */

void gcUIGeom__AssignCopy_constcBaseptr(s32 arg0, void *arg1) {
    s16 temp_a1;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x9F58C == 0) {
            if (*(s32 *)0x469C0 == 0) {
                if (*(s32 *)0x40FF4 == 0) {
                    if (*(s32 *)0x385DC == 0) {
                        *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
                    }
                    *(s32 *)0x40FF4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x16, *(s32 *)0x385DC);
                }
                *(s32 *)0x469C0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x17, *(s32 *)0x40FF4);
            }
            *(s32 *)0x9F58C = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x82, *(s32 *)0x469C0);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x9F58C;
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
    gcUIGeom__operator_eq_constgcUIGeomref(arg0, var_s2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
