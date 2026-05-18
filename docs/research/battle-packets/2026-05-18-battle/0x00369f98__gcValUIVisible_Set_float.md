# Battle Packet: `gcValUIVisible::Set(float)`

## Target

- Address: `0x00369f98`
- Size: `316` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValUIVisible`
- Method family: `Set`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 2B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `2`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0012dfdc` `untried` 728B `gcDesiredUIWidgetHelper::GetWidget(const cType *, bool) const`

## Placement

- Canonical source: `src/gcValUIVisible.cpp`
- Header(s): `include/gcValUIVisible.h`
- Allowed source(s): `src/gcValUIVisible.cpp`
- Split-TU prefix: `src/gcValUIVisible_*.cpp`

## Class Header: `include/gcValUIVisible.h`

Not found.

## Matched Same-Class Neighbors

- `0x00369af4` 80B `gcValUIVisible::AssignCopy(const cBase *)` — `src/gcValUIVisible.cpp`
- `0x00369b44` 152B `gcValUIVisible::New(cMemPool *, cBase *) static` — `src/gcValUIVisible.cpp`
- `0x00369bdc` 340B `gcValUIVisible::GetType(void) const` — `src/gcValUIVisible.cpp`
- `0x00369d30` 88B `gcValUIVisible::Write(cFile &) const` — `src/gcValUIVisible.cpp`
- `0x0036a0d4` 80B `gcValUIVisible::GetText(char *) const` — `src/gcValUIVisible.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValUIEnabled::Set(float)`

- Address: `0x003623a4`
- Size: `316` bytes
- Source: `src/gcValUIEnabled.cpp`

```cpp
// ── gcValUIEnabled::Set(float) @ 0x003623a4 ──
void gcValUIEnabled::Set(float value) {
    gcDesiredUIWidgetHelper *helper =
        (gcDesiredUIWidgetHelper *)((char *)this + 8);

    if (type_gcUIWidget == 0) {
        if (type_named == 0) {
            if (type_base == 0) {
                type_base = cType::InitializeType((const char *)0x36D894,
                                                  (const char *)0x36D89C,
                                                  1, 0, 0, 0, 0, 0);
            }
            type_named = cType::InitializeType(
                0, 0, 2, type_base,
                (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
        }
        type_gcUIWidget = cType::InitializeType(0, 0, 0x84, type_named,
                                                0, 0, 0, 0);
    }

    gcUIWidget *widget = helper->GetWidget(type_gcUIWidget, true);
    if (widget != 0) {
        gcValUIEnabled_VSlot *slot =
            (gcValUIEnabled_VSlot *)(*(char **)((char *)widget + 4) + 0xC8);
        short offset = slot->offset;
        void *adjusted = (char *)widget + offset;
        int enabled = 0;
        if (value != 0.0f) {
            enabled = 1;
        }
        slot->fn(adjusted, (unsigned char)enabled);
    }
}
```

### Exemplar 2: `gcValEventVar::Set(float)`

- Address: `0x00342d20`
- Size: `184` bytes
- Source: `src/gcValEventVar.cpp`

```cpp
void gcValEventVar::Set(float value) {
    int selector = mField08;
    int index;
    int base;
    int *ctx;
    unsigned char *event;

    switch (selector) {
    case 0:
        index = mFieldC;
        ctx = (int *)g_expressionEvalStack;
        base = 0;
        if (ctx != 0) {
            base = ctx[4];
        }
        *(float *)((index << 2) + base + 8) = value;
        return;
    case 1:
        index = mFieldC;
        ctx = (int *)g_expressionEvalStack;
        base = 0;
        if (ctx != 0) {
            base = ctx[4];
        }
        *(float *)((index << 2) + base + 0x18) = value;
        return;
    case 2:
        {
            int flag = 0;
            float zero = 0.0f;
            ctx = (int *)g_expressionEvalStack;
            if (value != zero) {
                flag = 1;
            }
            event = 0;
            if (ctx != 0) {
                event = (unsigned char *)ctx[3];
            }
            event[0x18] = flag;
        }
        return;
    }
}
```

### Exemplar 3: `gcValHeadset::Set(float)`

- Address: `0x0034725c`
- Size: `108` bytes
- Source: `src/gcValHeadset.cpp`

```cpp
// gcValHeadset::Set(float) @ 0x0034725c
// ─────────────────────────────────────────────────────────────────────────

void gcValHeadset::Set(float value) {
    if (mField8 < 2) {
        if (mField8 > 0) {
            nwHeadset::SetChannel((int)value);
        }
    } else if (mField8 < 3) {
        nwHeadset::AllowVoice(value != 0.0f);
    }
}
```

### Exemplar 4: `gcValStateTimer::Set(float)`

- Address: `0x0035bb20`
- Size: `100` bytes
- Source: `src/gcValStateTimer.cpp`

```cpp
//   gcValStateTimer::Set(float)                          @ 0x0035bb20 (100B)
//   gcValStateTimer::GetText(char *) const               @ 0x0035bb84 (40B)
//
// gcValStateTimer is a leaf cBase-derived class with no extra payload —
// just the inherited (mParent, mClassdesc) pair at offsets 0/4. New
// allocates 8 bytes from the mempool and placement-news a gcValStateTimer
// (parent ctor stores the cBase classdesc into mClassdesc; the derived
// ctor immediately overwrites it with gcValStateTimer's own vtable).
// The destructor restores the cBase classdesc and dispatches operator
// delete on the deleting-tail flag.

class cBase;
class cFile;
class cMemPool;
class cType;

extern char cBaseclassdesc[];                   // @ 0x37E6A8
extern char gcValStateTimervirtualtable[];      // @ 0x003978E0

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};
```


## Prior Failure Notes

### Note 1

- session `4122d3c6`; src `src/gcValUIVisible.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/4122d3c6/00369f98__agent_self_reported_failure__src_gcValUIVisible.cpp`

Implemented gcValUIVisible::Set(float) in canonical C++ form with the gcUIWidget lazy type chain (D_0009990C/D_000385E0/D_000385DC), gcDesiredUIWidgetHelper::GetWidget, and the classdesc+0xE8 bool setter dispatch. After restructuring the helper pointer lifetime and slot/flag ordering, compare_func is down to 2/316 bytes different. The remaining diff is only the final virtual-call function-pointer register: current emits lw/jalr through a2, while the target uses a3, with all surrounding instructions and control flow matching.

Tried explicit function-pointer locals, register binding to $a3, a barrier on the function pointer, and a short inline-asm call. The inline asm disrupted register allocation and shrank the symbol, so it was reverted to the closest C form. A 120s permuter run generated 2832 candidates / 1013 compiled and found no improvement. Root cause appears to be a last-mile register allocation choice for the indirect call, not a structural mismatch.


## Disassembly

```asm
369f98:	27bdffe0 	addiu	sp,sp,-32
369f9c:	afb00004 	sw	s0,4(sp)
369fa0:	afb10008 	sw	s1,8(sp)
369fa4:	24910008 	addiu	s1,a0,8
369fa8:	3c10000a 	lui	s0,0xa
369fac:	8e04990c 	lw	a0,-26356(s0)
369fb0:	e7b40000 	swc1	$f20,0(sp)
369fb4:	afb2000c 	sw	s2,12(sp)
369fb8:	afb30010 	sw	s3,16(sp)
369fbc:	afbf0014 	sw	ra,20(sp)
369fc0:	14800029 	bnez	a0,0x36a068
369fc4:	46006506 	mov.s	$f20,$f12
369fc8:	3c130004 	lui	s3,0x4
369fcc:	8e6485e0 	lw	a0,-31264(s3)
369fd0:	1480001b 	bnez	a0,0x36a040
369fd4:	3c120004 	lui	s2,0x4
369fd8:	8e4485dc 	lw	a0,-31268(s2)
369fdc:	1480000e 	bnez	a0,0x36a018
369fe0:	3c08001c 	lui	t0,0x1c
369fe4:	3c040037 	lui	a0,0x37
369fe8:	3c050037 	lui	a1,0x37
369fec:	34060001 	li	a2,0x1
369ff0:	00003825 	move	a3,zero
369ff4:	00004025 	move	t0,zero
369ff8:	00004825 	move	t1,zero
369ffc:	00005025 	move	t2,zero
36a000:	00005825 	move	t3,zero
36a004:	2484d894 	addiu	a0,a0,-10092
36a008:	0c001d4d 	jal	0x7534
36a00c:	24a5d89c 	addiu	a1,a1,-10084
36a010:	ae4285dc 	sw	v0,-31268(s2)
36a014:	3c08001c 	lui	t0,0x1c
36a018:	8e4785dc 	lw	a3,-31268(s2)
36a01c:	00002025 	move	a0,zero
36a020:	00002825 	move	a1,zero
36a024:	34060002 	li	a2,0x2
36a028:	00004825 	move	t1,zero
36a02c:	00005025 	move	t2,zero
36a030:	00005825 	move	t3,zero
36a034:	0c001d4d 	jal	0x7534
36a038:	25083c58 	addiu	t0,t0,15448
36a03c:	ae6285e0 	sw	v0,-31264(s3)
36a040:	8e6785e0 	lw	a3,-31264(s3)
36a044:	00002025 	move	a0,zero
36a048:	00002825 	move	a1,zero
36a04c:	34060084 	li	a2,0x84
36a050:	00004025 	move	t0,zero
36a054:	00004825 	move	t1,zero
36a058:	00005025 	move	t2,zero
36a05c:	0c001d4d 	jal	0x7534
36a060:	00005825 	move	t3,zero
36a064:	ae02990c 	sw	v0,-26356(s0)
36a068:	8e05990c 	lw	a1,-26356(s0)
36a06c:	02202025 	move	a0,s1
36a070:	0c04b7f7 	jal	0x12dfdc
36a074:	34060001 	li	a2,0x1
36a078:	00408825 	move	s1,v0
36a07c:	1220000d 	beqz	s1,0x36a0b4
36a080:	00000000 	nop
36a084:	8e250004 	lw	a1,4(s1)
36a088:	44806000 	mtc1	zero,$f12
36a08c:	460ca032 	c.eq.s	$f20,$f12
36a090:	24a500e8 	addiu	a1,a1,232
36a094:	84a40000 	lh	a0,0(a1)
36a098:	02242021 	addu	a0,s1,a0
36a09c:	34110000 	li	s1,0x0
36a0a0:	45020001 	bc1fl	0x36a0a8
36a0a4:	34110001 	li	s1,0x1
36a0a8:	8ca70004 	lw	a3,4(a1)
36a0ac:	00e0f809 	jalr	a3
36a0b0:	322500ff 	andi	a1,s1,0xff
36a0b4:	c7b40000 	lwc1	$f20,0(sp)
36a0b8:	8fb00004 	lw	s0,4(sp)
36a0bc:	8fb10008 	lw	s1,8(sp)
36a0c0:	8fb2000c 	lw	s2,12(sp)
36a0c4:	8fb30010 	lw	s3,16(sp)
36a0c8:	8fbf0014 	lw	ra,20(sp)
36a0cc:	03e00008 	jr	ra
36a0d0:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcValUIVisible::Set(float)
// Address: 0x00369f98, Size: 316B
// Obj: gcAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
void *gcDesiredUIWidgetHelper__GetWidget_constcTypeptr_boolconst(s32, s32, ?); /* extern */

void gcValUIVisible__Set_float(s32 arg0) {
    s32 var_s1;
    void *temp_a1;
    void *temp_v0;

    if (*(s32 *)0x9990C == 0) {
        if (*(s32 *)0x385E0 == 0) {
            if (*(s32 *)0x385DC == 0) {
                *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
            }
            *(s32 *)0x385E0 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 2, *(s32 *)0x385DC);
        }
        *(s32 *)0x9990C = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x84, *(s32 *)0x385E0);
    }
    temp_v0 = gcDesiredUIWidgetHelper__GetWidget_constcTypeptr_boolconst(arg0 + 8, *(s32 *)0x9990C, 1);
    if (temp_v0 != NULL) {
        temp_a1 = temp_v0->unk4;
        var_s1 = 0;
        if (arg0 != 0.0f) {
            var_s1 = 1;
        }
        (temp_a1 + 0xE8)->unk4(0, temp_v0 + temp_a1->unkE8, var_s1 & 0xFF);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
