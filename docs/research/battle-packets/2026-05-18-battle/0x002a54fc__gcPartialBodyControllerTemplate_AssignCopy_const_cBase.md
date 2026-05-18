# Battle Packet: `gcPartialBodyControllerTemplate::AssignCopy(const cBase *)`

## Target

- Address: `0x002a54fc`
- Size: `132` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcPartialBodyControllerTemplate`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 6B; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `6`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `6`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00006e7c` `matched` 228B `cBaseArray::operator=(const cBaseArray &)`
- `0x002292ec` `untried` 340B `dcast dcast<gcPartialBodyControllerTemplate *>(const cBase *)`

## Placement

- Canonical source: `src/gcPartialBodyControllerTemplate.cpp`
- Header(s): `include/gcPartialBodyControllerTemplate.h`
- Allowed source(s): `src/gcPartialBodyControllerTemplate.cpp`
- Split-TU prefix: `src/gcPartialBodyControllerTemplate_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcPartialBodyControllerTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x0013fbe8` 112B `gcPartialBodyControllerTemplate::Write(cFile &) const` — `src/gcPartialBodyControllerTemplate.cpp`
- `0x0013fd40` 72B `gcPartialBodyControllerTemplate::gcPartialBodyControllerTemplate(cBase *)` — `src/gcPartialBodyControllerTemplate.cpp`
- `0x0013fe38` 64B `gcPartialBodyControllerTemplate::Reset(cMemPool *, bool)` — `src/gcPartialBodyControllerTemplate_Reset.cpp`
- `0x0013fe78` 228B `gcPartialBodyControllerTemplate::GetInstanceType(void) const` — `src/gcPartialBodyControllerTemplate.cpp`
- `0x002a5580` 124B `gcPartialBodyControllerTemplate::New(cMemPool *, cBase *) static` — `src/gcPartialBodyControllerTemplate.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcGeomTrailController::AssignCopy(const cBase *)`

- Address: `0x0031c85c`
- Size: `132` bytes
- Source: `src/eRoomSet.cpp`

```cpp
// -- gcGeomTrailController::AssignCopy(const cBase *) @ 0x0031c85c --
void gcGeomTrailController::AssignCopy(const cBase *base) {
    gcGeomTrailController *other = dcast<gcGeomTrailController>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(const int *)((char *)other + 12);
    *(float *)((char *)this + 16) = *(const float *)((char *)other + 16);
    *(float *)((char *)this + 20) = *(const float *)((char *)other + 20);
    *(float *)((char *)this + 24) = *(const float *)((char *)other + 24);

    int i = 0;
    ShortTriple *dst = (ShortTriple *)((char *)this + 0x1C);
    ShortTriple *src = (ShortTriple *)((char *)other + 0x1C);
    do {
        *dst = *src;
        i++;
        dst++;
        src++;
    } while (i <= 0);
}
```

### Exemplar 2: `gcShadowController::AssignCopy(const cBase *)`

- Address: `0x0031f83c`
- Size: `132` bytes
- Source: `src/gcShadowController.cpp`

```cpp
// ── gcShadowController::AssignCopy(const cBase *) @ 0x0031f83c ──
void gcShadowController::AssignCopy(const cBase *base) {
    gcShadowController *other = dcast<gcShadowController>(base);

    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(int *)((char *)other + 12);
    *(float *)((char *)this + 16) = *(float *)((char *)other + 16);
    *(float *)((char *)this + 20) = *(float *)((char *)other + 20);
    *(float *)((char *)this + 24) = *(float *)((char *)other + 24);

    int i = 0;
    short *dst = (short *)((char *)this + 28);
    short *src = (short *)((char *)other + 28);
    do {
        short x = src[0];
        short y = src[1];
        short z = src[2];
        dst[0] = x;
        dst[1] = y;
        dst[2] = z;
        i++;
        dst += 3;
        src += 3;
    } while (i <= 0);
}
```

### Exemplar 3: `gcRegion::AssignCopy(const cBase *)`

- Address: `0x00245588`
- Size: `128` bytes
- Source: `src/gcRegion.cpp`

```cpp
void gcRegion::AssignCopy(const cBase *base) {
    gcRegion *other = dcast(base);
    gcRegionBase_assignop(this, other);
    int i = 0;
    const int *src = (const int *)other;
    int *dst = (int *)this;
    do {
        i++;
        *(int *)((char *)dst + 0x78) = *(const int *)((const char *)src + 0x78);
        src = (const int *)((const char *)src + 4);
        dst = (int *)((char *)dst + 4);
    } while (i < 12);
    gcEvent_assignop((char *)this + 0xA8, (const char *)other + 0xA8);
    gcEvent_assignop((char *)this + 0xC4, (const char *)other + 0xC4);
}
```

### Exemplar 4: `gcRoomInstance::AssignCopy(const cBase *)`

- Address: `0x00292354`
- Size: `128` bytes
- Source: `src/gcRoomInstance.cpp`

```cpp
void gcRoomInstance::AssignCopy(const cBase *base) {
    gcRoomInstance *other = dcast<gcRoomInstance>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(cHandle *)((char *)this + 0x44) = *(const cHandle *)((const char *)other + 0x44);
    *(cHandle *)((char *)this + 0x48) = *(const cHandle *)((const char *)other + 0x48);
    ((gcEvent *)((char *)this + 0x4C))->operator=(*(const gcEvent *)((const char *)other + 0x4C));
    ((gcEvent *)((char *)this + 0x68))->operator=(*(const gcEvent *)((const char *)other + 0x68));
    *(int *)((char *)this + 0x84) = *(const int *)((const char *)other + 0x84);
}
```


## Prior Failure Notes

### Note 1

- session `b3807681`; src `src/gcPartialBodyControllerTemplate.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/b3807681/002a54fc__agent_self_reported_failure__src_gcPartialBodyControllerTemplate.cpp`

Best result is 6/132 bytes different, identical to the prior failed attempt's permuter-improved best. The remaining diff is a fixed register-allocation divergence in the middle 4-byte copy (offset 0x0C) and the third copy's source-address register: expected uses a1 for copy-2's dst-address and a2 for copy-3's source-address; SNC instead picks a2 for copy-2's dst and a1 for copy-3's source. All other bytes match.

Approaches tried this session: (1) direct *(int*)((char*)this+8) field copies — collapses to lw/sw with offset, producing 108B (wrong size); (2) explicit pointer temporaries (int *src/dst per copy) — produces the addiu/lw/addiu/sw shape but compiler postpones the move s1,v0 because it can use v0 directly, giving 39 bytes diff; (3) *(cHandle*) pointer cast for all three — interleaves further, 34 bytes diff; (4) sched=1 — 42 bytes diff; (5) cHandle/handle/cHandle struct triple — 9 bytes diff; (6) variant orderings of dstIndex/srcIndex declarations.

The 6-byte form (current src on disk) is the same as the prior attempt: declare dstIndex before mHandle1=, srcIndex between assigns, mHandle2 last. Permuter has been run twice for 5min in the prior session with zero improvement. The diff is in addiu/lw/sw operand bits (not bnel/beql), so this does not qualify as the documented bnel divergence — stays 'failed'.

### Note 2

- session `9e0b210c`

Added a canonical C++ gcPartialBodyControllerTemplate::AssignCopy method and tried direct member copies, cHandle casts for the 0x0C field, pointer temporaries, volatile pointer temporaries, local scalar temporaries, grouped struct copies, sched=1, explicit cHandle assignment, and reordered declarations. The best hand-written form was 9 bytes different; a 5-minute permuter run improved it to 6 bytes different and a second 5-minute run found no further improvement.

The remaining mismatch is in the three 4-byte header copies at offsets 0x08, 0x0C, and 0x10. The expected code uses address-materializing addiu/lw/sw sequences and a specific register schedule; SNC keeps choosing direct lw/sw or different source/destination registers for the middle 0x0C copy. The rest of the function shape, both cBaseArray assignment calls, the short copy at 0x1C, and epilogue match structurally.


## Disassembly

```asm
2a54fc:	27bdfff0 	addiu	sp,sp,-16
2a5500:	afb00000 	sw	s0,0(sp)
2a5504:	00808025 	move	s0,a0
2a5508:	afb10004 	sw	s1,4(sp)
2a550c:	afbf0008 	sw	ra,8(sp)
2a5510:	0c08a4bb 	jal	0x2292ec
2a5514:	00a02025 	move	a0,a1
2a5518:	00408825 	move	s1,v0
2a551c:	26240008 	addiu	a0,s1,8
2a5520:	8c840000 	lw	a0,0(a0)
2a5524:	26050008 	addiu	a1,s0,8
2a5528:	aca40000 	sw	a0,0(a1)
2a552c:	2624000c 	addiu	a0,s1,12
2a5530:	8c840000 	lw	a0,0(a0)
2a5534:	2605000c 	addiu	a1,s0,12
2a5538:	26260010 	addiu	a2,s1,16
2a553c:	aca40000 	sw	a0,0(a1)
2a5540:	8cc60000 	lw	a2,0(a2)
2a5544:	26050010 	addiu	a1,s0,16
2a5548:	aca60000 	sw	a2,0(a1)
2a554c:	26040014 	addiu	a0,s0,20
2a5550:	0c001b9f 	jal	0x6e7c
2a5554:	26250014 	addiu	a1,s1,20
2a5558:	8625001c 	lh	a1,28(s1)
2a555c:	26040020 	addiu	a0,s0,32
2a5560:	a605001c 	sh	a1,28(s0)
2a5564:	0c001b9f 	jal	0x6e7c
2a5568:	26250020 	addiu	a1,s1,32
2a556c:	8fb00000 	lw	s0,0(sp)
2a5570:	8fb10004 	lw	s1,4(sp)
2a5574:	8fbf0008 	lw	ra,8(sp)
2a5578:	03e00008 	jr	ra
2a557c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcPartialBodyControllerTemplate::AssignCopy(const cBase *)
// Address: 0x002a54fc, Size: 132B
// Obj: gcAll_psp.obj

? cBaseArray__operator_eq_constcBaseArrayref(void *, void *, s32); /* extern */
void *dcastdcast_gcPartialBodyControllerTemplateptr__constcBaseptr(?); /* extern */

void gcPartialBodyControllerTemplate__AssignCopy_constcBaseptr(void *arg0, ? arg1) {
    s32 temp_a2;
    void *temp_v0;

    temp_v0 = dcastdcast_gcPartialBodyControllerTemplateptr__constcBaseptr(arg1);
    arg0->unk8 = (s32) temp_v0->unk8;
    arg0->unkC = (s32) temp_v0->unkC;
    temp_a2 = temp_v0->unk10;
    arg0->unk10 = temp_a2;
    cBaseArray__operator_eq_constcBaseArrayref(arg0 + 0x14, temp_v0 + 0x14, temp_a2);
    arg0->unk1C = (s16) temp_v0->unk1C;
    cBaseArray__operator_eq_constcBaseArrayref(arg0 + 0x20, temp_v0 + 0x20);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
