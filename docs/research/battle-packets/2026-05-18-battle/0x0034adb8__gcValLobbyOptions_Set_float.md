# Battle Packet: `gcValLobbyOptions::Set(float)`

## Target

- Address: `0x0034adb8`
- Size: `224` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValLobbyOptions`
- Method family: `Set`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 19B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `19`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `19`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x001a54fc` `matched` 8B `nwNetwork::GetLobby(void) static`

## Placement

- Canonical source: `src/gcValLobbyOptions.cpp`
- Header(s): `include/gcValLobbyOptions.h`
- Allowed source(s): `src/gcValLobbyOptions.cpp`, `src/gcValHeadset.cpp`
- Split-TU prefix: `src/gcValLobbyOptions_*.cpp`

## Class Header: `include/gcValLobbyOptions.h`

Not found.

## Matched Same-Class Neighbors

- `0x0034a9c4` 48B `gcValLobbyOptions::AssignCopy(const cBase *)` — `src/gcValHeadset.cpp`
- `0x0034a9f4` 136B `gcValLobbyOptions::New(cMemPool *, cBase *) static` — `src/gcValLobbyOptions.cpp`
- `0x0034aa7c` 340B `gcValLobbyOptions::GetType(void) const` — `src/gcValLobbyOptions.cpp`
- `0x0034abd0` 88B `gcValLobbyOptions::Write(cFile &) const` — `src/gcValLobbyOptions.cpp`
- `0x0034ac28` 208B `gcValLobbyOptions::Read(cFile &, cMemPool *)` — `src/gcValLobbyOptions.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValEventVar::Set(float)`

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

### Exemplar 2: `gcValUIEnabled::Set(float)`

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

- session `99ede13a`; src `src/gcValHeadset.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/99ede13a/0034adb8__agent_self_reported_failure__src_gcValHeadset.cpp`

Closest attempt is the permuter-saved source in src/gcValHeadset.cpp at 19/224 bytes differing after a full 300s run (10,824 candidates generated, 3,924 compiled). Manual attempts tried natural if/else dispatch, explicit goto layout to reproduce the bgtz/bltz/bnezl shape, shared vs duplicated return tails, int/bool/unsigned-char setter parameter types, explicit flag variables, separate object/function-pointer temporaries, register bindings for the dispatch entry and callee, empty asm constraints, and sched=1 (which was worse). The control-flow skeleton can be made exact, and the dispatch call registers can also be made exact, but not simultaneously with the original compare/load schedule.

The remaining mismatch is concentrated in the two virtual setter dispatch blocks and branch-delay slots. The original wants the selector-zero and selector-one paths to preload the lobby vtable in branch delay slots, then perform mtc1/li/c.eq before dispatch-entry loads and call via a0/a1 moves. The closest byte result shifts the mtc1/li setup into those branch-delay regions and changes bnezl delay-slot contents; other variants get the desired dispatch register sequence but leave a 38-byte scheduling mismatch. This looks like an SNC scheduler/register-allocation corner with little remaining C-level leverage.


## Disassembly

```asm
34adb8:	27bdfff0 	addiu	sp,sp,-16
34adbc:	e7b40000 	swc1	$f20,0(sp)
34adc0:	46006506 	mov.s	$f20,$f12
34adc4:	afb00004 	sw	s0,4(sp)
34adc8:	afbf0008 	sw	ra,8(sp)
34adcc:	0c06953f 	jal	0x1a54fc
34add0:	00808025 	move	s0,a0
34add4:	00402025 	move	a0,v0
34add8:	1080002a 	beqz	a0,0x34ae84
34addc:	00000000 	nop
34ade0:	8e100008 	lw	s0,8(s0)
34ade4:	1e000006 	bgtz	s0,0x34ae00
34ade8:	2a050002 	slti	a1,s0,2
34adec:	06000025 	bltz	s0,0x34ae84
34adf0:	00000000 	nop
34adf4:	10000007 	b	0x34ae14
34adf8:	8c850000 	lw	a1,0(a0)
34adfc:	2a050002 	slti	a1,s0,2
34ae00:	54a00013 	bnezl	a1,0x34ae50
34ae04:	8c850000 	lw	a1,0(a0)
34ae08:	1000001e 	b	0x34ae84
34ae0c:	00000000 	nop
34ae10:	8c850000 	lw	a1,0(a0)
34ae14:	44806000 	mtc1	zero,$f12
34ae18:	34060000 	li	a2,0x0
34ae1c:	460ca032 	c.eq.s	$f20,$f12
34ae20:	24a704a8 	addiu	a3,a1,1192
34ae24:	84e50000 	lh	a1,0(a3)
34ae28:	00852821 	addu	a1,a0,a1
34ae2c:	8ce40004 	lw	a0,4(a3)
34ae30:	45020001 	bc1fl	0x34ae38
34ae34:	34060001 	li	a2,0x1
34ae38:	00803825 	move	a3,a0
34ae3c:	00a02025 	move	a0,a1
34ae40:	00e0f809 	jalr	a3
34ae44:	30c500ff 	andi	a1,a2,0xff
34ae48:	1000000e 	b	0x34ae84
34ae4c:	00000000 	nop
34ae50:	44806000 	mtc1	zero,$f12
34ae54:	34060000 	li	a2,0x0
34ae58:	460ca032 	c.eq.s	$f20,$f12
34ae5c:	24a704b0 	addiu	a3,a1,1200
34ae60:	84e50000 	lh	a1,0(a3)
34ae64:	00852821 	addu	a1,a0,a1
34ae68:	8ce40004 	lw	a0,4(a3)
34ae6c:	45020001 	bc1fl	0x34ae74
34ae70:	34060001 	li	a2,0x1
34ae74:	00803825 	move	a3,a0
34ae78:	00a02025 	move	a0,a1
34ae7c:	00e0f809 	jalr	a3
34ae80:	30c500ff 	andi	a1,a2,0xff
34ae84:	c7b40000 	lwc1	$f20,0(sp)
34ae88:	8fb00004 	lw	s0,4(sp)
34ae8c:	8fbf0008 	lw	ra,8(sp)
34ae90:	03e00008 	jr	ra
34ae94:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcValLobbyOptions::Set(float)
// Address: 0x0034adb8, Size: 224B
// Obj: gcAll_psp.obj

void **nwNetwork__GetLobby_voidstatic();            /* extern */

void gcValLobbyOptions__Set_float(f32 arg0) {
    ? (*temp_a0)(?, void *, s32, s32, ?);
    ? (*temp_a0_2)(?, void *, s32, s32, ?);
    s32 temp_s0;
    s32 var_a2;
    s32 var_a2_2;
    void **temp_v0;
    void *temp_a1;
    void *temp_a1_2;

    temp_v0 = nwNetwork__GetLobby_voidstatic();
    if (temp_v0 != NULL) {
        temp_s0 = arg0->unk8;
        switch (temp_s0) {                          /* irregular */
        case 1:
            temp_a1 = *temp_v0;
            var_a2 = 0;
            temp_a0 = (temp_a1 + 0x4B0)->unk4;
            if (arg0 != 0.0f) {
                var_a2 = 1;
            }
            temp_a0(0, temp_v0 + temp_a1->unk4B0, var_a2 & 0xFF, var_a2, temp_a0);
            break;
        case 0:
            temp_a1_2 = *temp_v0;
            var_a2_2 = 0;
            temp_a0_2 = (temp_a1_2 + 0x4A8)->unk4;
            if (arg0 != 0.0f) {
                var_a2_2 = 1;
            }
            temp_a0_2(0, temp_v0 + temp_a1_2->unk4A8, var_a2_2 & 0xFF, var_a2_2, temp_a0_2);
            return;
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
