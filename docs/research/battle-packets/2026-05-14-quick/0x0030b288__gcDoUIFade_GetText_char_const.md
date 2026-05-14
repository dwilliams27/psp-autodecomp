# Battle Packet: `gcDoUIFade::GetText(char *) const`

## Target

- Address: `0x0030b288`
- Size: `568` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDoUIFade`
- Method family: `GetText`
- Leaf: `False`

## Queue Metadata

- score: `189`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 1B; correct function size; codegen-specific diagnosis`
- failure_action: `prep`
- near_miss_bytes: `1`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `1`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x000011c0` `untried` 124B `cStrAppend(char *, const char *, ...)`
- `0x00001368` `untried` 96B `cStrCat(char *, const char *)`

## Placement

- Canonical source: `src/gcDoUIFade.cpp`
- Header(s): `include/gcDoUIFade.h`
- Allowed source(s): `src/gcDoUIFade.cpp`, `src/gcDoUIFade_GetText.cpp`
- Split-TU prefix: `src/gcDoUIFade_*.cpp`

## Class Header: `include/gcDoUIFade.h`

```cpp
#ifndef GCDOUIFADE_H
#define GCDOUIFADE_H

class cMemPool;
class cBase;
class cFile;
class cType;
class gcExpression;

class gcDoUIFade {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    gcDoUIFade &operator=(const gcDoUIFade &);
};

class gcDoUISendMessage {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    gcDoUISendMessage &operator=(const gcDoUISendMessage &);
};

class gcExpression;

class gcDoUIWidgetForEach {
public:
    char _pad[0x18];
    gcExpression *branch;

    ~gcDoUIWidgetForEach(void);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    void GetText(char *) const;
    void Write(cFile &) const;
    int  Read(cFile &, cMemPool *);
    float Evaluate(void) const;
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcDoWhile {
public:
    char _pad[0x10];
    gcExpression *branch;

    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
};

struct gcValBinaryOpData {
    char _pad[0x08];
    int mOp;
    gcExpression **mChildren;
};

class gcValBinaryOp {
public:
    gcValBinaryOp(cBase *);
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    int GetExprFlags(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

class gcValCameraFollowEntity3rdVariable {
public:
    float Evaluate(void) const;
    void Set(float);
};

class gcValCameraValue {
public:
    float Evaluate(void) const;
};

#endif
```

## Matched Same-Class Neighbors

- `0x0030a7b8` 420B `gcDoUIFade::AssignCopy(const cBase *)` — `src/gcDoUIFade.cpp`
- `0x0030a95c` 220B `gcDoUIFade::New(cMemPool *, cBase *) static` — `src/gcDoUIFade_New.cpp`
- `0x0030aa38` 280B `gcDoUIFade::GetType(void) const` — `src/gcDoUIFade_GetType.cpp`
- `0x0030ab50` 260B `gcDoUIFade::Write(cFile &) const` — `src/gcDoUIFade_Write.cpp`
- `0x0030b4c0` 8B `gcDoUIFade::GetExprFlags(void) const` — `src/gcDoUIFade.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDoEntityAttack::GetText(char *) const`

- Address: `0x002a6a3c`
- Size: `572` bytes
- Source: `src/gcDoEntityAttack.cpp`

```cpp
void gcDoEntityAttack::GetText(char *buf) const {
    register const gcDoEntityAttack *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char targetText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    text[0] = '\0';
    targetText[0] = '\0';

    if (*(int *)((const char *)self + 0x38) == 0) {
        char *textStart = text;
        *textStart = '\0';
        bool helperFlag = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x44))
            ->GetText(text, helperFlag);

        int branchOwned = 0;
        int branch = *(int *)((const char *)self + 0x50);
        int branchTagged = branch & 1;
        if (branchTagged != 0) {
            branchOwned = 1;
        }
        int branchValid;
        if (branchOwned != 0) {
            branchValid = 0;
        } else {
            branchValid = branch != 0;
            branchValid &= 0xFF;
            branchValid = branchValid != 0;
        }

        if (branchValid != 0) {
            cStrAppend(text, (const char *)0x36DCBC);

            branch = *(int *)((const char *)self + 0x50);
            int owned2 = 0;
            if (branch & 1) {
                owned2 = 1;
            }
            if (owned2 != 0) {
                branch = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = branch;
            if (check != 0) {
                GetTextSlot *slot =
                    (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
                slot->fn((char *)branch + slot->offset, text);
            } else {
                cStrCat(text, (const char *)0x36DB24);
            }
        }
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x58);
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x54;
        slot1->fn(sub1 + slot1->offset, text);
    }

    int target = *(int *)((const char *)self + 0x68);
    int owned3 = 0;
    if (target & 1) {
        owned3 = 1;
    }
    if (owned3 != 0) {
        target = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int targetCheck = target;
    if (targetCheck != 0) {
        GetTextSlot *slot2 = (GetTextSlot *)(*(char **)(targetCheck + 4) + 0xD0);
        slot2->fn((char *)target + slot2->offset, targetText);
    } else {
        cStrCat(targetText, (const char *)0x36DB24);
    }

    int mode = *(int *)((const char *)self + 0x6C);
    if (mode < 2) {
        if (mode < 0) {
            goto done;
        }
        if (mode > 0) {
            goto mode_one;
        }
        cStrAppend(out, (const char *)0x36E528, text);
        goto done;
    } else {
        if (mode < 3) {
            goto mode_two;
        }
        if (mode >= 4) {
            goto done;
        }
        cStrAppend(out, (const char *)0x36E53C, text);
        goto done;
    }

mode_one:
    cStrAppend(out, (const char *)0x36E554, text, targetText);
    goto done;

mode_two:
    cStrAppend(out, (const char *)0x36E578, targetText);

done:
    ;
}
```

### Exemplar 2: `gcDoEntityPartialBodyAttack::GetText(char *) const`

- Address: `0x002b94d0`
- Size: `572` bytes
- Source: `src/gcDoEntityPartialBodyAttack_GetText.cpp`

```cpp
void gcDoEntityPartialBodyAttack::GetText(char *buf) const {
    register const gcDoEntityPartialBodyAttack *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char targetText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    text[0] = '\0';
    targetText[0] = '\0';

    if (*(int *)((const char *)self + 0x38) == 0) {
        char *textStart = text;
        *textStart = '\0';
        bool helperFlag = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x44))
            ->GetText(text, helperFlag);

        int branchOwned = 0;
        int branch = *(int *)((const char *)self + 0x50);
        int branchTagged = branch & 1;
        if (branchTagged != 0) {
            branchOwned = 1;
        }
        int branchValid;
        if (branchOwned != 0) {
            branchValid = 0;
        } else {
            branchValid = branch != 0;
            branchValid &= 0xFF;
            branchValid = branchValid != 0;
        }

        if (branchValid != 0) {
            cStrAppend(text, (const char *)0x36DCBC);

            branch = *(int *)((const char *)self + 0x50);
            int owned2 = 0;
            if (branch & 1) {
                owned2 = 1;
            }
            if (owned2 != 0) {
                branch = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = branch;
            if (check != 0) {
                GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
                slot->fn((char *)branch + slot->offset, text);
            } else {
                cStrCat(text, (const char *)0x36DB24);
            }
        }
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x58);
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x54;
        slot1->fn(sub1 + slot1->offset, text);
    }

    int target = *(int *)((const char *)self + 0x68);
    int owned3 = 0;
    if (target & 1) {
        owned3 = 1;
    }
    if (owned3 != 0) {
        target = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int targetCheck = target;
    if (targetCheck != 0) {
        GetTextSlot *slot2 = (GetTextSlot *)(*(char **)(targetCheck + 4) + 0xD0);
        slot2->fn((char *)target + slot2->offset, targetText);
    } else {
        cStrCat(targetText, (const char *)0x36DB24);
    }

    int mode = *(int *)((const char *)self + 0x6C);
    if (mode < 2) {
        if (mode < 0) {
            goto done;
        }
        if (mode > 0) {
            goto mode_one;
        }
        cStrAppend(out, (const char *)0x36E78C, text);
        goto done;
    } else {
        if (mode < 3) {
            goto mode_two;
        }
        if (mode >= 4) {
            goto done;
        }
        cStrAppend(out, (const char *)0x36E7AC, text);
        goto done;
    }

mode_one:
    cStrAppend(out, (const char *)0x36E7D0, text, targetText);
    goto done;

mode_two:
    cStrAppend(out, (const char *)0x36E800, targetText);

done:
    ;
}
```

### Exemplar 3: `gcDoEntityPlayAnimation::GetText(char *) const`

- Address: `0x002bb198`
- Size: `572` bytes
- Source: `src/gcDoEntityPlayAnimation.cpp`

```cpp
void gcDoEntityPlayAnimation::GetText(char *buf) const {
    register const gcDoEntityPlayAnimation *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char objectText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    int object;
    if (*(int *)((const char *)self + 0x38) == 0) {
        text[0] = '\0';
        char *textStart = text;
        *textStart = '\0';
        bool helperFlag = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x44))
            ->GetText(text, helperFlag);

        int branchOwned = 0;
        int branch = *(int *)((const char *)self + 0x50);
        int branchTagged = branch & 1;
        if (branchTagged != 0) {
            branchOwned = 1;
        }
        int branchValid;
        if (branchOwned != 0) {
            branchValid = 0;
        } else {
            branchValid = branch != 0;
            branchValid &= 0xFF;
            branchValid = branchValid != 0;
        }

        if (branchValid != 0) {
            cStrAppend(text, (const char *)0x36DCBC);

            branch = *(int *)((const char *)self + 0x50);
            int owned2 = 0;
            if (branch & 1) {
                owned2 = 1;
            }
            if (owned2 != 0) {
                branch = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check = branch;
            if (check != 0) {
                GetTextSlot *slot =
                    (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
                slot->fn((char *)branch + slot->offset, text);
            } else {
                cStrCat(text, (const char *)0x36DB24);
            }
        }

        cStrAppend(out, (const char *)0x36E82C, text);
        object = *(int *)((const char *)self + 0x68);
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x58);
        objectText[0] = '\0';
        register char *objectOut __asm__("$18") = objectText;
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x54;
        slot1->fn(sub1 + slot1->offset, objectOut);
        cStrAppend(out, (const char *)0x36E840, objectOut);
        object = *(int *)((const char *)self + 0x68);
    }

    int objectTagged = object & 1;
    int objectOwned = 0;
    if (objectTagged != 0) {
        objectOwned = 1;
    }
    if (objectOwned != 0) {
        object = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int objectCheck = object;
    if (objectCheck != 0) {
        cStrCat(out, (const char *)0x36E85C);

        object = *(int *)((const char *)self + 0x68);
        int owned2 = 0;
        if (object & 1) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            object = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = object;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)object + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrCat(out, (const char *)0x36E2E8);
    }

    cStrCat(out, (const char *)0x36DCEC);
}
```

### Exemplar 4: `gcDoEntityPlayPartialBodyAnimation::GetText(char *) const`

- Address: `0x002bccbc`
- Size: `572` bytes
- Source: `src/gcDoEntityPlayPartialBodyAnimation.cpp`

```cpp
void gcDoEntityPlayPartialBodyAnimation::GetText(char *buf) const {
    register const gcDoEntityPlayPartialBodyAnimation *self __asm__("$16") =
        this;
    register char *out __asm__("$17") = buf;
    char text[256];
    char branchText[256];
    char objectText[256];

    char *typeInfo0 = *(char **)((const char *)self + 0x10);
    GetTextSlot *slot0 = (GetTextSlot *)(typeInfo0 + 0x78);
    char *sub0 = (char *)self + 0x0C;
    slot0->fn(sub0 + slot0->offset, out);

    text[0] = '\0';
    char *textStart = text;
    *textStart = '\0';
    bool helperFlag = false;
    ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x40))
        ->GetText(text, helperFlag);

    int branchOwned = 0;
    int branch = *(int *)((const char *)self + 0x4C);
    int branchTagged = branch & 1;
    if (branchTagged != 0) {
        branchOwned = 1;
    }
    int branchValid;
    if (branchOwned != 0) {
        branchValid = 0;
    } else {
        branchValid = branch != 0;
        branchValid &= 0xFF;
        branchValid = branchValid != 0;
    }

    if (branchValid != 0) {
        cStrAppend(text, (const char *)0x36DCBC);

        branch = *(int *)((const char *)self + 0x4C);
        int owned2 = 0;
        if (branch & 1) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            branch = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = branch;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)branch + slot->offset, text);
        } else {
            cStrCat(text, (const char *)0x36DB24);
        }
    }

    if (*(int *)((const char *)self + 0x50) == 0) {
        branchText[0] = '\0';
        char *branchStart = branchText;
        *branchStart = '\0';
        bool helperFlag2 = false;
        ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x5C))
            ->GetText(branchText, helperFlag2);

        int branchOwned2 = 0;
        int branch2 = *(int *)((const char *)self + 0x68);
        int branchTagged2 = branch2 & 1;
        if (branchTagged2 != 0) {
            branchOwned2 = 1;
        }
        int branchValid2;
        if (branchOwned2 != 0) {
            branchValid2 = 0;
        } else {
            branchValid2 = branch2 != 0;
            branchValid2 &= 0xFF;
            branchValid2 = branchValid2 != 0;
        }

        if (branchValid2 != 0) {
            cStrAppend(branchText, (const char *)0x36DCBC);

            branch2 = *(int *)((const char *)self + 0x68);
            int owned3 = 0;
            if (branch2 & 1) {
                owned3 = 1;
            }
            if (owned3 != 0) {
                branch2 = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int check2 = branch2;
            if (check2 != 0) {
                GetTextSlot *slot =
                    (GetTextSlot *)(*(char **)(check2 + 4) + 0xD0);
                slot->fn((char *)branch2 + slot->offset, branchText);
            } else {
                cStrCat(branchText, (const char *)0x36DB24);
            }
        }

        cStrAppend(out, (const char *)0x36E868, text, branchText);
    } else {
        char *typeInfo1 = *(char **)((const char *)self + 0x70);
        objectText[0] = '\0';
        register char *objectOut __asm__("$18") = objectText;
        GetTextSlot *slot1 = (GetTextSlot *)(typeInfo1 + 0x40);
        char *sub1 = (char *)self + 0x6C;
        slot1->fn(sub1 + slot1->offset, objectOut);
        cStrAppend(out, (const char *)0x36E88C, text, objectOut);
    }
}
```


## Prior Failure Notes

### Note 1

- session `2287c69a`; src `src/gcDoUIFade_GetText.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/2287c69a/0030b288__agent_self_reported_failure__src_gcDoUIFade_GetText.cpp`

Closest attempt is a 1-byte mismatch with the correct 568-byte size. The function matches structurally after reconstructing the first handle-name lookup with fixed argument registers ($a0 format, $a2 mode, $a3 separator, $t0 name) and the rest of the child/flag/suffix text building aligns. The remaining byte is the immediate on the branch at the empty-name path: our code emits the branch directly to the shared call setup, while the original branches to the intermediate entry-path branch first. This changes only the branch target immediate; all surrounding instructions, calls, delay slots, and later code are aligned.

Tried multiple C restructurings for the entry-present/name-empty block: structured if/else, explicit goto labels, preincrementing entry for the delay slot, struct field loads to force lh/lw offsets, fixed register variables for fmt/sep/entry, duplicated goto blocks to force the shared branch, and a 300s permuter run (12,024 candidates, 5,430 compiled). The permuter found no improvement from the 1-byte baseline. Root cause appears to be an SNC control-flow layout/tail-merge choice for the tiny shared branch block, not register allocation or scheduling elsewhere.


## Disassembly

```asm
30b288:	27bdfff0 	addiu	sp,sp,-16
30b28c:	afb00000 	sw	s0,0(sp)
30b290:	00808025 	move	s0,a0
30b294:	8e040010 	lw	a0,16(s0)
30b298:	30840002 	andi	a0,a0,0x2
30b29c:	0004202b 	sltu	a0,zero,a0
30b2a0:	308600ff 	andi	a2,a0,0xff
30b2a4:	3c040037 	lui	a0,0x37
30b2a8:	afb10004 	sw	s1,4(sp)
30b2ac:	00a08825 	move	s1,a1
30b2b0:	2484f020 	addiu	a0,a0,-4064
30b2b4:	afbf0008 	sw	ra,8(sp)
30b2b8:	10c00004 	beqz	a2,0x30b2cc
30b2bc:	8e05004c 	lw	a1,76(s0)
30b2c0:	3c060037 	lui	a2,0x37
30b2c4:	10000003 	b	0x30b2d4
30b2c8:	24c6f030 	addiu	a2,a2,-4048
30b2cc:	3c060037 	lui	a2,0x37
30b2d0:	24c6f038 	addiu	a2,a2,-4040
30b2d4:	3c070037 	lui	a3,0x37
30b2d8:	14a00003 	bnez	a1,0x30b2e8
30b2dc:	24e7daf0 	addiu	a3,a3,-9488
30b2e0:	1000000d 	b	0x30b318
30b2e4:	34080000 	li	t0,0x0
30b2e8:	30a8ffff 	andi	t0,a1,0xffff
30b2ec:	3c090004 	lui	t1,0x4
30b2f0:	00084080 	sll	t0,t0,0x2
30b2f4:	25298890 	addiu	t1,t1,-30576
30b2f8:	01094021 	addu	t0,t0,t1
30b2fc:	8d090000 	lw	t1,0(t0)
30b300:	11200005 	beqz	t1,0x30b318
30b304:	34080000 	li	t0,0x0
30b308:	8d2a0030 	lw	t2,48(t1)
30b30c:	15450002 	bne	t2,a1,0x30b318
30b310:	00000000 	nop
30b314:	01204025 	move	t0,t1
30b318:	1100000b 	beqz	t0,0x30b348
30b31c:	00000000 	nop
30b320:	8505001c 	lh	a1,28(t0)
30b324:	2ca50001 	sltiu	a1,a1,1
30b328:	30a500ff 	andi	a1,a1,0xff
30b32c:	10a00004 	beqz	a1,0x30b340
30b330:	25080008 	addiu	t0,t0,8
30b334:	3c080037 	lui	t0,0x37
30b338:	10000001 	b	0x30b340
30b33c:	2508dab8 	addiu	t0,t0,-9544
30b340:	10000008 	b	0x30b364
30b344:	00802825 	move	a1,a0
30b348:	10a00004 	beqz	a1,0x30b35c
30b34c:	3c080037 	lui	t0,0x37
30b350:	3c080037 	lui	t0,0x37
30b354:	10000002 	b	0x30b360
30b358:	2508dac4 	addiu	t0,t0,-9532
30b35c:	2508dacc 	addiu	t0,t0,-9524
30b360:	00802825 	move	a1,a0
30b364:	0c000470 	jal	0x11c0
30b368:	02202025 	move	a0,s1
30b36c:	8e050050 	lw	a1,80(s0)
30b370:	34040000 	li	a0,0x0
30b374:	30a60001 	andi	a2,a1,0x1
30b378:	54c00001 	bnezl	a2,0x30b380
30b37c:	34040001 	li	a0,0x1
30b380:	10800003 	beqz	a0,0x30b390
30b384:	00000000 	nop
30b388:	10000001 	b	0x30b390
30b38c:	34050000 	li	a1,0x0
30b390:	10a00021 	beqz	a1,0x30b418
30b394:	3c050037 	lui	a1,0x37
30b398:	02202025 	move	a0,s1
30b39c:	0c0004da 	jal	0x1368
30b3a0:	24a5eaf0 	addiu	a1,a1,-5392
30b3a4:	8e040050 	lw	a0,80(s0)
30b3a8:	34050000 	li	a1,0x0
30b3ac:	30860001 	andi	a2,a0,0x1
30b3b0:	54c00001 	bnezl	a2,0x30b3b8
30b3b4:	34050001 	li	a1,0x1
30b3b8:	10a00003 	beqz	a1,0x30b3c8
30b3bc:	00000000 	nop
30b3c0:	10000001 	b	0x30b3c8
30b3c4:	34040000 	li	a0,0x0
30b3c8:	00802825 	move	a1,a0
30b3cc:	50a0000b 	beqzl	a1,0x30b3fc
30b3d0:	3c050037 	lui	a1,0x37
30b3d4:	8ca60004 	lw	a2,4(a1)
30b3d8:	02202825 	move	a1,s1
30b3dc:	24c600d0 	addiu	a2,a2,208
30b3e0:	84c70000 	lh	a3,0(a2)
30b3e4:	8cc60004 	lw	a2,4(a2)
30b3e8:	00c0f809 	jalr	a2
30b3ec:	00872021 	addu	a0,a0,a3
30b3f0:	10000006 	b	0x30b40c
30b3f4:	3c050037 	lui	a1,0x37
30b3f8:	3c050037 	lui	a1,0x37
30b3fc:	02202025 	move	a0,s1
30b400:	0c0004da 	jal	0x1368
30b404:	24a5db24 	addiu	a1,a1,-9436
30b408:	3c050037 	lui	a1,0x37
30b40c:	02202025 	move	a0,s1
30b410:	0c0004da 	jal	0x1368
30b414:	24a5e48c 	addiu	a1,a1,-7028
30b418:	8e040010 	lw	a0,16(s0)
30b41c:	30840004 	andi	a0,a0,0x4
30b420:	0004202b 	sltu	a0,zero,a0
30b424:	308400ff 	andi	a0,a0,0xff
30b428:	10800004 	beqz	a0,0x30b43c
30b42c:	3c050037 	lui	a1,0x37
30b430:	02202025 	move	a0,s1
30b434:	0c0004da 	jal	0x1368
30b438:	24a5f03c 	addiu	a1,a1,-4036
30b43c:	8e040010 	lw	a0,16(s0)
30b440:	30840008 	andi	a0,a0,0x8
30b444:	0004202b 	sltu	a0,zero,a0
30b448:	308400ff 	andi	a0,a0,0xff
30b44c:	10800004 	beqz	a0,0x30b460
30b450:	3c050037 	lui	a1,0x37
30b454:	02202025 	move	a0,s1
30b458:	0c0004da 	jal	0x1368
30b45c:	24a5f04c 	addiu	a1,a1,-4020
30b460:	8e040060 	lw	a0,96(s0)
30b464:	2c840001 	sltiu	a0,a0,1
30b468:	308400ff 	andi	a0,a0,0xff
30b46c:	14800004 	bnez	a0,0x30b480
30b470:	3c050037 	lui	a1,0x37
30b474:	02202025 	move	a0,s1
30b478:	0c000470 	jal	0x11c0
30b47c:	24a5e6b8 	addiu	a1,a1,-6472
30b480:	8e040078 	lw	a0,120(s0)
30b484:	8c840000 	lw	a0,0(a0)
30b488:	10800004 	beqz	a0,0x30b49c
30b48c:	3c040037 	lui	a0,0x37
30b490:	3c040037 	lui	a0,0x37
30b494:	10000002 	b	0x30b4a0
30b498:	2484dad8 	addiu	a0,a0,-9512
30b49c:	2484e440 	addiu	a0,a0,-7104
30b4a0:	00802825 	move	a1,a0
30b4a4:	0c000470 	jal	0x11c0
30b4a8:	02202025 	move	a0,s1
30b4ac:	8fb00000 	lw	s0,0(sp)
30b4b0:	8fb10004 	lw	s1,4(sp)
30b4b4:	8fbf0008 	lw	ra,8(sp)
30b4b8:	03e00008 	jr	ra
30b4bc:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcDoUIFade::GetText(char *) const
// Address: 0x0030b288, Size: 568B
// Obj: gcAll_psp.obj

? cStrAppend_charptr_constcharptr_...(?, ?, ?, ?);  /* extern */
? cStrCat_charptr_constcharptr(?, ?, s32);          /* extern */

void gcDoUIFade__GetText_charptrconst(void *arg0, ? arg1) {
    ? (*temp_a2_4)(s32, ?, ?, s16);
    ? var_a0_3;
    ? var_a2;
    s16 temp_a3;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a1_2;
    void *temp_a2_3;
    void *temp_t1;
    void *var_t0;

    temp_a1 = arg0->unk4C;
    if (arg0->unk10 & 2 & 0xFF) {
        var_a2 = 0x36F030;
    } else {
        var_a2 = 0x36F038;
    }
    if (temp_a1 == 0) {
        var_t0 = NULL;
    } else {
        temp_t1 = ((temp_a1 & 0xFFFF) * 4)->unk38890;
        var_t0 = NULL;
        if ((temp_t1 != NULL) && (temp_t1->unk30 == temp_a1)) {
            var_t0 = temp_t1;
        }
    }
    if (var_t0 != NULL) {
        if ((var_t0->unk1C == 0) & 0xFF) {

        }
    } else if (temp_a1 != 0) {

    }
    cStrAppend_charptr_constcharptr_...(arg1, 0x36F020, var_a2, 0x36DAF0);
    var_a1 = arg0->unk50;
    var_a0 = 0;
    temp_a2 = var_a1 & 1;
    if (temp_a2 != 0) {
        var_a0 = 1;
    }
    if (var_a0 != 0) {
        var_a1 = 0;
    }
    if (var_a1 != 0) {
        cStrCat_charptr_constcharptr(arg1, 0x36EAF0, temp_a2);
        var_a0_2 = arg0->unk50;
        var_a1_2 = 0;
        temp_a2_2 = var_a0_2 & 1;
        if (temp_a2_2 != 0) {
            var_a1_2 = 1;
        }
        if (var_a1_2 != 0) {
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            temp_a2_3 = var_a0_2->unk4;
            temp_a3 = temp_a2_3->unkD0;
            temp_a2_4 = (temp_a2_3 + 0xD0)->unk4;
            temp_a2_4(var_a0_2 + temp_a3, arg1, temp_a2_4, temp_a3);
        } else {
            cStrCat_charptr_constcharptr(arg1, 0x36DB24, temp_a2_2);
        }
        cStrCat_charptr_constcharptr(arg1, 0x36E48C);
    }
    if (arg0->unk10 & 4 & 0xFF) {
        cStrCat_charptr_constcharptr(arg1, 0x36F03C);
    }
    if (arg0->unk10 & 8 & 0xFF) {
        cStrCat_charptr_constcharptr(arg1, 0x36F04C);
    }
    if (!((arg0->unk60 == 0) & 0xFF)) {
        cStrAppend_charptr_constcharptr_...(arg1, 0x36E6B8);
    }
    if (*arg0->unk78 != 0) {
        var_a0_3 = 0x36DAD8;
    } else {
        var_a0_3 = 0x36E440;
    }
    cStrAppend_charptr_constcharptr_...(arg1, var_a0_3);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
