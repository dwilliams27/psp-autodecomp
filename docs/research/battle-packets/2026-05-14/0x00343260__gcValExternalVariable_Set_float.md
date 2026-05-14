# Battle Packet: `gcValExternalVariable::Set(float)`

## Target

- Address: `0x00343260`
- Size: `116` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValExternalVariable`
- Method family: `Set`
- Leaf: `True`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; manageable size`
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

## Placement

- Canonical source: `src/gcValExternalVariable.cpp`
- Header(s): `include/gcValExternalVariable.h`
- Allowed source(s): `src/gcValExternalVariable.cpp`
- Split-TU prefix: `src/gcValExternalVariable_*.cpp`

## Class Header: `include/gcValExternalVariable.h`

Not found.

## Matched Same-Class Neighbors

- `0x00342ed0` 136B `gcValExternalVariable::New(cMemPool *, cBase *) static` — `src/gcValExternalVariable.cpp`
- `0x00342f58` 340B `gcValExternalVariable::GetType(void) const` — `src/gcValExternalVariable.cpp`
- `0x003430ac` 88B `gcValExternalVariable::Write(cFile &) const` — `src/gcValExternalVariable.cpp`
- `0x00343104` 220B `gcValExternalVariable::Read(cFile &, cMemPool *)` — `src/gcValExternalVariable.cpp`
- `0x00343698` 100B `gcValExternalVariable::~gcValExternalVariable(void)` — `src/gcValExternalVariable.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcValHeadset::Set(float)`

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

### Exemplar 2: `gcValStateTimer::Set(float)`

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

### Exemplar 3: `gcValEventVar::Set(float)`

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

### Exemplar 4: `gcValCameraFollowEntity3rdVariable::Set(float)`

- Address: `0x00322318`
- Size: `8` bytes
- Source: `src/gcValCameraFollowEntity3rdVariable.cpp`

```cpp
// Function: gcValCameraFollowEntity3rdVariable::Set(float)
// -----------------------------------------------------------------------------
void gcValCameraFollowEntity3rdVariable::Set(float) {
}
```


## Prior Failure Notes

### Note 1

- session `770e3501`; src `src/gcValExternalVariable.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/770e3501/00343260__agent_self_reported_failure__src_gcValExternalVariable.cpp`

category=REG_ALLOC. Reconstructed gcValExternalVariable::Set(float) as canonical C++ in src/gcValExternalVariable.cpp using the same two-stage cHandle table validation visible in the disassembly: load mHandle.mIndex, validate D_00038890[index & 0xffff] against object+0x30, then reload the table entry and store the float at +0x44. Control flow, size, immediates, memory offsets, branches, delay slots, and the final swc1 all line up; compare_func reports only 8/116 bytes different.

The remaining difference is the first lookup's candidate/result register allocation: original uses a1 for the validated result and a2 for the loaded candidate (for example li a1,0; lw a2,0(a1); beqz a2; move a1,a2), while SNC in this TU consistently emits the same instructions with a1/a2 swapped for those values. Tried void* casts, raw char-table access, typed gcExternalVariableValue fields, early-return layout, obj/cand declaration reorderings, and local register hints. Ran one 300s permuter pass on 0x00343260 with --save-best: 4,294 candidates compiled, best stayed at 8 bytes with zero improvements.


## Disassembly

```asm
343260:	8c840008 	lw	a0,8(a0)
343264:	14800003 	bnez	a0,0x343274
343268:	3085ffff 	andi	a1,a0,0xffff
34326c:	1000000c 	b	0x3432a0
343270:	34050000 	li	a1,0x0
343274:	3c060004 	lui	a2,0x4
343278:	00052880 	sll	a1,a1,0x2
34327c:	24c68890 	addiu	a2,a2,-30576
343280:	00a62821 	addu	a1,a1,a2
343284:	8ca60000 	lw	a2,0(a1)
343288:	10c00005 	beqz	a2,0x3432a0
34328c:	34050000 	li	a1,0x0
343290:	8cc70030 	lw	a3,48(a2)
343294:	14e40002 	bne	a3,a0,0x3432a0
343298:	00000000 	nop
34329c:	00c02825 	move	a1,a2
3432a0:	10a0000a 	beqz	a1,0x3432cc
3432a4:	00000000 	nop
3432a8:	10800007 	beqz	a0,0x3432c8
3432ac:	34050000 	li	a1,0x0
3432b0:	3084ffff 	andi	a0,a0,0xffff
3432b4:	3c050004 	lui	a1,0x4
3432b8:	00042080 	sll	a0,a0,0x2
3432bc:	24a58890 	addiu	a1,a1,-30576
3432c0:	00852021 	addu	a0,a0,a1
3432c4:	8c850000 	lw	a1,0(a0)
3432c8:	e4ac0044 	swc1	$f12,68(a1)
3432cc:	03e00008 	jr	ra
3432d0:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcValExternalVariable::Set(float)
// Address: 0x00343260, Size: 116B
// Obj: gcAll_psp.obj

void gcValExternalVariable__Set_float(void *arg0) {
    s32 temp_a0;
    void *temp_a2;
    void *var_a1;
    void *var_a1_2;

    temp_a0 = arg0->unk8;
    if (temp_a0 == 0) {
        var_a1 = NULL;
    } else {
        temp_a2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        var_a1 = NULL;
        if ((temp_a2 != NULL) && (temp_a2->unk30 == temp_a0)) {
            var_a1 = temp_a2;
        }
    }
    if (var_a1 != NULL) {
        var_a1_2 = NULL;
        if (temp_a0 != 0) {
            var_a1_2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        }
        var_a1_2->unk44 = arg0;
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
