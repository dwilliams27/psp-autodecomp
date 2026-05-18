# Battle Packet: `nwConfigBase::AssignCopy(const cBase *)`

## Target

- Address: `0x0036b340`
- Size: `164` bytes
- Status: `failed`
- Object: `nwAll_psp.obj`
- Class: `nwConfigBase`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `155`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 19B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `19`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `19`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0036b228` `untried` 280B `dcast dcast<nwConfigBase *>(const cBase *)`

## Placement

- Canonical source: `src/nwConfigBase.cpp`
- Header(s): `include/nwConfigBase.h`
- Allowed source(s): `src/nwConfigBase.cpp`
- Split-TU prefix: `src/nwConfigBase_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/nwConfigBase.h`

Not found.

## Matched Same-Class Neighbors

- `0x001a4d00` 156B `nwConfigBase::Write(cFile &) const` — `src/nwConfigBase.cpp`
- `0x001a4eec` 144B `nwConfigBase::nwConfigBase(cBase *)` — `src/nwConfigBase.cpp`
- `0x001a4f7c` 120B `nwConfigBase::~nwConfigBase(void)` — `src/nwConfigBase.cpp`
- `0x00243f14` 124B `nwConfigBase::New(cMemPool *, cBase *) static` — `src/nwConfigBase.cpp`
- `0x0036b3e4` 160B `nwConfigBase::GetType(void) const` — `src/nwConfigBase.cpp`

## Matched Method Exemplars

### Exemplar 1: `nwConfigPSP::AssignCopy(const cBase *)`

- Address: `0x0036b484`
- Size: `548` bytes
- Source: `src/nwConfigPSP.cpp`

```cpp
// ── nwConfigPSP::AssignCopy(const cBase *) @ 0x0036B484 ──
void nwConfigPSP::AssignCopy(const cBase *base) {
    const nwConfigPSP *other = 0;

    if (base != 0) {
        if (D_0009F924 == 0) {
            if (D_0009F928 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x371158,
                                                       (const char *)0x371160,
                                                       1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) =
                    (cBase *(*)(cMemPool *, cBase *))&nwConfigBase::New;
                D_0009F928 = cType::InitializeType(0, 0, 0x28C, parentType,
                                                   factory, 0, 0, 0);
            }
            const cType *parentType = D_0009F928;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))&nwConfigPSP::New;
            D_0009F924 = cType::InitializeType(0, 0, 0x298, parentType,
                                               factory, 0, 0, 0);
        }

        cType *target = D_0009F924;
        void *classDesc = *(void **)((const char *)base + 4);
        TypeDispatchSlot *slot = (TypeDispatchSlot *)((char *)classDesc + 8);
        short offset = slot->offset;
        cType *type = slot->getType((void *)((const char *)base + offset));
        int ok;

        if (target != 0) {
            goto have_target;
        }
        ok = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop:
            if (type == target) {
                ok = 1;
            } else {
                type = ((TypeNode *)type)->parent;
                if (type != 0) {
                    goto loop;
                }
                goto invalid;
            }
        } else {
invalid:
            ok = 0;
        }

cast_done:
        if (ok != 0) {
            other = (const nwConfigPSP *)base;
        }
    }

    *(int *)((char *)this + 0x08) =
        *(const int *)((const char *)other + 0x08);
    *(unsigned short *)((char *)this + 0x0C) =
        *(const unsigned short *)((const char *)other + 0x0C);
    *(int *)((char *)this + 0x10) =
        *(const int *)((const char *)other + 0x10);
    *(unsigned char *)((char *)this + 0x14) =
        *(const unsigned char *)((const char *)other + 0x14);
    *(int *)((char *)this + 0x18) =
        *(const int *)((const char *)other + 0x18);
    *(int *)((char *)this + 0x1C) =
        *(const int *)((const char *)other + 0x1C);

    int srcBase = (int)other;
    int i = 0;
    unsigned int dstBase = (int)this;
    do {
        int dstAddr = i + dstBase;
        int srcAddr = i + srcBase;
        *(char *)(dstAddr + 0x20) = *(const char *)(srcAddr + 0x20);
        i += 1;
    } while (i < 0x20);

    i = 0;
    do {
        int dstAddr = i + dstBase;
        int srcAddr = i + srcBase;
        *(char *)(dstAddr + 0x40) = *(const char *)(srcAddr + 0x40);
        i += 1;
    } while (i < 0x20);

    i = 0;
    do {
        int dstAddr = i + dstBase;
        int srcAddr = i + srcBase;
        *(char *)(dstAddr + 0x60) = *(const char *)(srcAddr + 0x60);
        i += 1;
    } while (i < 0x20);

    i = 0;
    do {
        int dstAddr = i + dstBase;
        int srcAddr = i + srcBase;
        *(char *)(dstAddr + 0x80) = *(const char *)(srcAddr + 0x80);
        i += 1;
    } while (i < 0x40);
}
```

### Exemplar 2: `gcDoEntityForEachAttached::AssignCopy(const cBase *)`

- Address: `0x002b3e50`
- Size: `164` bytes
- Source: `src/gcDoEntityForEachAttached.cpp`

```cpp
void gcDoEntityForEachAttached::AssignCopy(const cBase *src) {
    gcDoEntityForEachAttached *other =
        (gcDoEntityForEachAttached *)dcast_gcDoEntityForEachAttached_cBase_ptr(src);

    int *self_i = (int *)this;
    int *other_i = (int *)other;

    int cleared = self_i[2] & ~3;
    self_i[2] = cleared;
    self_i[2] = cleared | (other_i[2] & 3);

    self_i[3] = other_i[3];

    ((gcDesiredEntity *)((char *)this + 0x10))
        ->operator=(*(const gcDesiredEntity *)((char *)other + 0x10));

    self_i[15] = other_i[15];
    self_i[16] = other_i[16];
    self_i[17] = other_i[17];

    ((gcDesiredEntityTemplate *)((char *)this + 0x48))
        ->operator=(*(const gcDesiredEntityTemplate *)((char *)other + 0x48));

    ((cArrayBase<cHandlePairT<cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandlePairT<cSubHandleT<gcEnumerationEntry> > > *)((char *)other + 0x60));

    ((gcExpressionList *)((char *)this + 0x64))
        ->operator=(*(const gcExpressionList *)((char *)other + 0x64));
}
```

### Exemplar 3: `gcPCBipedControllerTemplate::AssignCopy(const cBase *)`

- Address: `0x00319fdc`
- Size: `164` bytes
- Source: `src/gcPCBipedControllerTemplate.cpp`

```cpp
//   gcPCBipedControllerTemplate::AssignCopy(const cBase *)     @ 0x00319fdc (164B)
//   gcPCBipedControllerTemplate::New(cMemPool *, cBase *)      @ 0x0031a080 (124B)

#include "cBase.h"

class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```

### Exemplar 4: `gcNPCFlyingControllerTemplate::AssignCopy(const cBase *)`

- Address: `0x0031d4ec`
- Size: `164` bytes
- Source: `src/gcNPCFlyingControllerTemplate.cpp`

```cpp
//   gcNPCFlyingControllerTemplate::AssignCopy(const cBase *)      @ 0x0031D4EC  (164B)
//   gcNPCFlyingControllerTemplate::New(cMemPool *, cBase *)       @ 0x0031D590  (124B)
//
// Sibling pattern of gcNPCBipedControllerTemplate. The NPC variant inherits
// gcFlyingControllerTemplate (same overall layout as gcBipedControllerTemplate
// minus a few trailing fields), and adds one int member at offset +0x50 that
// is serialised by Write() and copied by AssignCopy().

#include "cBase.h"

class cFile;
class cMemPool;
class cType;

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

- session `f8bb1648`; src `src/nwConfigBase.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/f8bb1648/0036b340__agent_self_reported_failure__src_nwConfigBase.cpp`

19/164 byte mismatch — register allocation issue. Target uses a0 for `other` (dcast result) and a1 for the byte-loop counter; my output uses a1 for `other` and a0 for the counter. The structural shape (control flow, instruction selection, scheduling, two unrolled byte loops with addu in delay slot) is otherwise identical.

Tried this session: (a) source variants without int* aliases (worse, 21B), (b) using char* base only (much worse, 110B), (c) for-loop instead of do-while (worse, 23B), (d) `register __asm__("a0")` GCC-style register hint — SNC ignored it (still 19B), (e) extended permuter run for 540 seconds, 14544 candidates generated and 7096 compiled, ZERO improvement (still 19B).

Combined with prior session's 5-minute permuter (10176 candidates, also 19B), this confirms ~12000+ source-level permutations cannot move SNC off this register allocation. The 19 differing bytes are the immediate-field/register-encoding diffs across the 5+ instructions where a0/a1 are swapped (move a0/a1,v0; lw a1/a0,8(a0/a1); …; li a1/a0,0; addu a2,a1/a0,a0/a1; …).

Root cause: SNC's register allocator chooses a1 instead of a0 to receive the dcast result. The decision is determined by some internal heuristic insensitive to source-level permutations. Likely needs either a permuter improvement (more aggressive scheduling perturbations) or a compiler-level patch. All other nwConfigBase methods in the file (ctor, dtor, Write, New, GetType) are MATCHED.

### Note 2

- session `c034cef3`

Implemented the logical C copy: dcast to nwConfigBase, copy scalar fields at 0x08/0x0C/0x10/0x14/0x18/0x1C, then copy the two 0x20-byte strings with byte loops. Tried direct char-offset copies, a layout struct, reassigning the src parameter to the dcast result, declaration-order changes, sched=1, and register-hint/empty-asm variants. The best normal C form compiles to the right size and control flow but keeps the dcast result in a1 and uses a0 as the byte-loop counter; the target moves the dcast result into a0 and uses a1 as the counter. Manual variants that tried to force the register either did not affect allocation or made aliasing/scheduling much worse.

Ran tools/permuter.py for 300 seconds with --save-best. It generated 10176 candidates, compiled 5884, improved the diff from 21 bytes to 19 bytes, but did not match. The remaining mismatch is register allocation/scheduling around AssignCopy, not a semantic or structural mismatch.


## Disassembly

```asm
36b340:	27bdfff0 	addiu	sp,sp,-16
36b344:	afb00000 	sw	s0,0(sp)
36b348:	00808025 	move	s0,a0
36b34c:	afbf0004 	sw	ra,4(sp)
36b350:	0c0dac8a 	jal	0x36b228
36b354:	00a02025 	move	a0,a1
36b358:	00402025 	move	a0,v0
36b35c:	8c850008 	lw	a1,8(a0)
36b360:	ae050008 	sw	a1,8(s0)
36b364:	9485000c 	lhu	a1,12(a0)
36b368:	a605000c 	sh	a1,12(s0)
36b36c:	8c850010 	lw	a1,16(a0)
36b370:	ae050010 	sw	a1,16(s0)
36b374:	90850014 	lbu	a1,20(a0)
36b378:	a2050014 	sb	a1,20(s0)
36b37c:	8c860018 	lw	a2,24(a0)
36b380:	ae060018 	sw	a2,24(s0)
36b384:	8c86001c 	lw	a2,28(a0)
36b388:	34050000 	li	a1,0x0
36b38c:	ae06001c 	sw	a2,28(s0)
36b390:	00a43021 	addu	a2,a1,a0
36b394:	80c60020 	lb	a2,32(a2)
36b398:	00b03821 	addu	a3,a1,s0
36b39c:	24a50001 	addiu	a1,a1,1
36b3a0:	a0e60020 	sb	a2,32(a3)
36b3a4:	28a60020 	slti	a2,a1,32
36b3a8:	14c0fffa 	bnez	a2,0x36b394
36b3ac:	00a43021 	addu	a2,a1,a0
36b3b0:	34050000 	li	a1,0x0
36b3b4:	00a43021 	addu	a2,a1,a0
36b3b8:	80c60040 	lb	a2,64(a2)
36b3bc:	00b03821 	addu	a3,a1,s0
36b3c0:	24a50001 	addiu	a1,a1,1
36b3c4:	a0e60040 	sb	a2,64(a3)
36b3c8:	28a60020 	slti	a2,a1,32
36b3cc:	14c0fffa 	bnez	a2,0x36b3b8
36b3d0:	00a43021 	addu	a2,a1,a0
36b3d4:	8fb00000 	lw	s0,0(sp)
36b3d8:	8fbf0004 	lw	ra,4(sp)
36b3dc:	03e00008 	jr	ra
36b3e0:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: nwConfigBase::AssignCopy(const cBase *)
// Address: 0x0036b340, Size: 164B
// Obj: nwAll_psp.obj

void *dcastdcast_nwConfigBaseptr__constcBaseptr(?); /* extern */

void nwConfigBase__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 var_a1;
    s32 var_a1_2;
    void *temp_a3;
    void *temp_a3_2;
    void *temp_v0;
    void *var_a2;
    void *var_a2_2;

    temp_v0 = dcastdcast_nwConfigBaseptr__constcBaseptr(arg1);
    arg0->unk8 = (s32) temp_v0->unk8;
    arg0->unkC = (u16) temp_v0->unkC;
    arg0->unk10 = (s32) temp_v0->unk10;
    arg0->unk14 = (u8) temp_v0->unk14;
    arg0->unk18 = (s32) temp_v0->unk18;
    var_a1 = 0;
    arg0->unk1C = (s32) temp_v0->unk1C;
    var_a2 = temp_v0;
    do {
        temp_a3 = var_a1 + arg0;
        var_a1 += 1;
        temp_a3->unk20 = (s8) var_a2->unk20;
        var_a2 = var_a1 + temp_v0;
    } while (var_a1 < 0x20);
    var_a1_2 = 0;
    var_a2_2 = temp_v0;
    do {
        temp_a3_2 = var_a1_2 + arg0;
        var_a1_2 += 1;
        temp_a3_2->unk40 = (s8) var_a2_2->unk40;
        var_a2_2 = var_a1_2 + temp_v0;
    } while (var_a1_2 < 0x20);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
