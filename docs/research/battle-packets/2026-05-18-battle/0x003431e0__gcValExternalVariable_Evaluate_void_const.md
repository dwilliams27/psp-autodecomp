# Battle Packet: `gcValExternalVariable::Evaluate(void) const`

## Target

- Address: `0x003431e0`
- Size: `128` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcValExternalVariable`
- Method family: `Evaluate`
- Leaf: `True`

## Queue Metadata

- score: `173`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; correct function size; manageable size`
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

### Exemplar 1: `gcValHeadset::Evaluate(void) const`

- Address: `0x003471cc`
- Size: `144` bytes
- Source: `src/gcValHeadset.cpp`

```cpp
//   gcValHeadset::Evaluate(void) const                   @ 0x003471cc
//   eSound::IsPlaying(void) const                        @ 0x00021460
//   gcValLobbyInfo::GetText(char *) const                @ 0x00349db8
//   gcValLobbyOptions::AssignCopy(const cBase *)         @ 0x0034a9c4
//   gcValLobbySessionStatus::GetText(char *) const       @ 0x0034e830

class cBase;
class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```

### Exemplar 2: `gcValLobbySessionStatus::Evaluate(void) const`

- Address: `0x0034e7c4`
- Size: `108` bytes
- Source: `src/gcValLobbySessionStatus.cpp`

```cpp
// ── gcValLobbySessionStatus::Evaluate(void) const @ 0x0034e7c4 ──
float gcValLobbySessionStatus::Evaluate(void) const {
    void *lobby = nwNetwork::GetLobby();
    if (lobby == 0) {
        return 0.0f;
    }
    DispatchEntry *e = (DispatchEntry *)(*(char **)lobby + 0x80);
    short off = e->offset;
    int (*fn)(void *) = e->fn;
    int status = fn((char *)lobby + off);
    return (float)(this->mField8 == status);
}
```

### Exemplar 3: `gcValObjectIsValid::Evaluate(void) const`

- Address: `0x0035698c`
- Size: `164` bytes
- Source: `src/gcValObjectIsValid.cpp`

```cpp
float gcValObjectIsValid::Evaluate(void) const {
    int obj = mObject;
    int flag = 0;
    int tag = obj & 1;
    if (tag != 0) {
        flag = 1;
    }
    int valid;
    if (flag == 0) goto eval_not_tagged;
    valid = 0;
    goto eval_valid_done;
eval_not_tagged:
    valid = obj != 0;
    valid &= 0xFF;
    valid = valid != 0;
eval_valid_done:
    if (valid != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }
        int base;
        char *type;
        if (flag2 == 0) goto eval_untagged_base;
        base = 0;
        type = *(char **)(base + 4);
        goto eval_type_done;
eval_untagged_base:
        base = obj;
        type = *(char **)(base + 4);
eval_type_done:
        cTypeMethod *entry = (cTypeMethod *)(type + 0x70);
        short off = entry->offset;
        int (*fn)(void *, int) = (int (*)(void *, int))entry->fn;
        if (fn((char *)base + off, 0) != 0) {
            return 1.0f;
        }
    }
    return 0.0f;
}
```

### Exemplar 4: `gcValEntityPrimaryController::Evaluate(void) const`

- Address: `0x0033b488`
- Size: `168` bytes
- Source: `src/gcValEntityPrimaryController.cpp`

```cpp
float gcValEntityPrimaryController::Evaluate(void) const {
    float neg_one = -1.0f;
    char *entity = (char *)((const gcDesiredEntity *)((const char *)this + 8))->Get(true);
    int selector;
    if (entity == 0) goto retNegOne;
    selector = *(const int *)((const char *)this + 52);
    if (selector > 0) goto ret64;
    if (selector < 0) goto retNegOne;
    return (float)entity[0x65];
retNegOne:
    return neg_one;
ret64:
    if (selector < 2) return (float)entity[0x64];
    goto retNegOne;
}
```


## Prior Failure Notes

### Note 1

- session `770e3501`; src `src/gcValExternalVariable.cpp`; snapshot `logs/failure_snapshots/match_20260506_155600/770e3501/003431e0__agent_self_reported_failure__src_gcValExternalVariable.cpp`

category=REG_ALLOC. Reconstructed gcValExternalVariable::Evaluate(void) const with the same validated handle lookup as Set(float), early zero return, redundant index==0 branch around the zero-return block, second D_00038890 reload, and lwc1 from +0x44. The early-return source form is required for the expected 128-byte layout; the if-body-return form compiles to a 120-byte function with the load block before the zero return. Current closest source has the correct 128-byte size and compare_func reports only 8/128 bytes different.

The byte differences mirror Set(float): the first handle-table lookup has a1/a2 swapped between the loaded candidate and validated object result, with no control-flow or offset differences. The same source perturbations were tried for both functions, and the 300s permuter pass on sibling Set(float) found no improvement from the shared lookup shape. This appears to be local SNC register-allocation drift from the reduced TU context rather than a semantic mismatch.


## Disassembly

```asm
3431e0:	8c840008 	lw	a0,8(a0)
3431e4:	14800003 	bnez	a0,0x3431f4
3431e8:	3085ffff 	andi	a1,a0,0xffff
3431ec:	1000000c 	b	0x343220
3431f0:	34050000 	li	a1,0x0
3431f4:	3c060004 	lui	a2,0x4
3431f8:	00052880 	sll	a1,a1,0x2
3431fc:	24c68890 	addiu	a2,a2,-30576
343200:	00a62821 	addu	a1,a1,a2
343204:	8ca60000 	lw	a2,0(a1)
343208:	10c00005 	beqz	a2,0x343220
34320c:	34050000 	li	a1,0x0
343210:	8cc70030 	lw	a3,48(a2)
343214:	14e40002 	bne	a3,a0,0x343220
343218:	00000000 	nop
34321c:	00c02825 	move	a1,a2
343220:	10a00005 	beqz	a1,0x343238
343224:	00000000 	nop
343228:	14800005 	bnez	a0,0x343240
34322c:	34050000 	li	a1,0x0
343230:	10000009 	b	0x343258
343234:	00000000 	nop
343238:	03e00008 	jr	ra
34323c:	44800000 	mtc1	zero,$f0
343240:	3084ffff 	andi	a0,a0,0xffff
343244:	3c050004 	lui	a1,0x4
343248:	00042080 	sll	a0,a0,0x2
34324c:	24a58890 	addiu	a1,a1,-30576
343250:	00852021 	addu	a0,a0,a1
343254:	8c850000 	lw	a1,0(a0)
343258:	03e00008 	jr	ra
34325c:	c4a00044 	lwc1	$f0,68(a1)
```

## m2c Starting Point

```c
// Decompiled: gcValExternalVariable::Evaluate(void) const
// Address: 0x003431e0, Size: 128B
// Obj: gcAll_psp.obj

f32 gcValExternalVariable__Evaluate_voidconst(void *arg0) {
    s32 temp_a0;
    void *temp_a2;
    void *var_a1;
    void *var_a1_2;

    temp_a0 = arg0->unk8;
    if (temp_a0 == 0) {
        var_a1_2 = NULL;
    } else {
        temp_a2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        var_a1_2 = NULL;
        if ((temp_a2 != NULL) && (temp_a2->unk30 == temp_a0)) {
            var_a1_2 = temp_a2;
        }
    }
    if (var_a1_2 != NULL) {
        var_a1 = NULL;
        if (temp_a0 == 0) {

        } else {
            var_a1 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        }
        return var_a1->unk44;
    }
    return 0.0f;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
