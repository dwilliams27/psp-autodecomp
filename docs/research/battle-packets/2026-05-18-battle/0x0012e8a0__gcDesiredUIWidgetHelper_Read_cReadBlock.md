# Battle Packet: `gcDesiredUIWidgetHelper::Read(cReadBlock &)`

## Target

- Address: `0x0012e8a0`
- Size: `256` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDesiredUIWidgetHelper`
- Method family: `Read`
- Leaf: `False`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; Read hard family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00004ff4` `untried` 128B `cFileSystem::Read(cFileHandle *, void *, unsigned int) static`
- `0x00005d00` `failed` 188B `cReadBlock::cReadBlock(cFile &, unsigned int, bool)`
- `0x00005dbc` `failed` 212B `cReadBlock::~cReadBlock(void)`
- `0x000093ec` `untried` 828B `cHandle::Read(cReadBlock &, cMemPool *)`

Callers:
- `0x0012ece0` `matched` 200B `gcDesiredUIWidget::Read(cFile &, cMemPool *)`
- `0x0028f658` `failed` 240B `gcUIControlString::Read(cFile &, cMemPool *)`
- `0x003092d0` `failed` 364B `gcDoUIEditBoxOp::Read(cFile &, cMemPool *)`
- `0x0030a33c` `failed` 268B `gcDoUIEffect::Read(cFile &, cMemPool *)`
- `0x0030c794` `failed` 260B `gcDoUISetFocus::Read(cFile &, cMemPool *)`
- `0x0030d094` `matched` 232B `gcDoUIListOp::Read(cFile &, cMemPool *)`
- `0x0030df90` `failed` 452B `gcDoUISendMessage::Read(cFile &, cMemPool *)`
- `0x0030f49c` `matched` 232B `gcDoUISetFont::Read(cFile &, cMemPool *)`
- `0x0030fecc` `matched` 252B `gcDoUISetOrigin::Read(cFile &, cMemPool *)`
- `0x003107c4` `matched` 268B `gcDoUISetSprite::Read(cFile &, cMemPool *)`
- `0x003112f4` `matched` 372B `gcDoUISetState::Read(cFile &, cMemPool *)`
- `0x00313744` `failed` 688B `gcDoUISetText::Read(cFile &, cMemPool *)`
- `0x0031504c` `matched` 240B `gcDoUISetTextColor::Read(cFile &, cMemPool *)`
- `0x00315878` `matched` 260B `gcDoUISetTextSprite::Read(cFile &, cMemPool *)`
- `0x003165a4` `matched` 212B `gcDoUIWidgetForEach::Read(cFile &, cMemPool *)`
- `0x00361d54` `failed` 200B `gcValUIChecked::Read(cFile &, cMemPool *)`
- `0x00362194` `failed` 200B `gcValUIEnabled::Read(cFile &, cMemPool *)`
- `0x00362b10` `matched` 232B `gcValUIListInfo::Read(cFile &, cMemPool *)`
- `0x003637c0` `failed` 200B `gcValUIOpacity::Read(cFile &, cMemPool *)`
- `0x00363f94` `matched` 272B `gcValUIPosition::Read(cFile &, cMemPool *)`
- `0x0036554c` `matched` 272B `gcValUISize::Read(cFile &, cMemPool *)`
- `0x0036685c` `failed` 292B `gcValUITexCoord::Read(cFile &, cMemPool *)`
- `0x003676ac` `failed` 616B `gcValUITextParam::Read(cFile &, cMemPool *)`
- `0x003684e8` `failed` 220B `gcValUIVar::Read(cFile &, cMemPool *)`

## Placement

- Canonical source: `src/gcDesiredUIWidgetHelper.cpp`
- Header(s): `include/gcDesiredUIWidgetHelper.h`
- Allowed source(s): `src/gcDesiredUIWidgetHelper.cpp`
- Split-TU prefix: `src/gcDesiredUIWidgetHelper_*.cpp`

## Compiler Guidance

Read template: preserve read-block/stream construction, old-position restore paths, and result initialization location. Compare against same stream type exemplars when available.

## Class Header: `include/gcDesiredUIWidgetHelper.h`

Not found.

## Matched Same-Class Neighbors

- `0x0012e844` 92B `gcDesiredUIWidgetHelper::Write(cWriteBlock &) const` — `src/gcDesiredUIWidgetHelper.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcStateFunction::Read(cFile &, cMemPool *)`

- Address: `0x001097b4`
- Size: `256` bytes
- Source: `src/gcStateFunction.cpp`

```cpp
// ── gcStateFunction::Read(cFile &, cMemPool *) @ 0x001097b4 ──
int gcStateFunction::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || cNamed::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }

    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x3C, 4);
    char *base = (char *)this + 0x20;
    char *mType = *(char **)((char *)this + 0x24);
    ParamReadVtableEntry *entry = (ParamReadVtableEntry *)(mType + 0x30);
    typedef void (*ReadFn)(void *, void *, cMemPool *);
    ((ReadFn)entry->fn)(base + entry->adj,
                        *(void **)&rb._data[0],
                        cMemPool::GetPoolFromPtr(base));
    return result;
}
```

### Exemplar 2: `gcStreamedCinematic::Read(cFile &, cMemPool *)`

- Address: `0x000ed04c`
- Size: `252` bytes
- Source: `src/gcStreamedCinematic.cpp`

```cpp
int gcStreamedCinematic::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1)
        goto fail;
    if (!((cFactory *)this)->Read(file, pool))
        goto fail;
    if ((unsigned int)rb[3] >= 2)
        goto do_groups;
    goto after_groups;
fail:
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
do_groups:
    ((cFactory *)this)->ReadGroups(*(cReadBlock *)rb, pool);
after_groups:
    gcCinematic_Read((char *)this + 0x4C, file, pool);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 3: `gcDesiredEntity::Read(cFile &, cMemPool *)`

- Address: `0x0011d6f4`
- Size: `248` bytes
- Source: `src/gcDesiredEntity.cpp`

```cpp
//   0x0011d6f4 gcDesiredEntity::Read(cFile &, cMemPool *)  248B
//   0x002e0c08 gcDoLobbyFriendOp::Read(cFile &, cMemPool *)  252B
//   0x0026023c gcDesiredEntity::GetType(void) const    220B
//   0x0026050c gcDesiredEntity::~gcDesiredEntity(void)  336B

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;
class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cSubHandleT;

template <class T, class U>
class cHandlePairT;

class cWriteBlock {
public:
    cFile *_file;       // +0
    int _pos;           // +4
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};
```

### Exemplar 4: `gcStreamedCinematicConfig::Read(cFile &, cMemPool *)`

- Address: `0x000eda4c`
- Size: `268` bytes
- Source: `src/gcStreamedCinematicConfig.cpp`

```cpp
int gcStreamedCinematicConfig::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    char enabled;
    char loop;

    if (rb[3] != 3) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((cGUID *)((char *)this + 0x08))->Read(*(cReadBlock *)rb);
    *(int *)((char *)this + 0x18) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x18);
        handle->Read(*(cReadBlock *)rb, (cMemPool *)cMemPool_GetPoolFromPtr(handle));
    }
    cFileSystem::Read(*(cFileHandle **)rb[0], &enabled, 1);
    *(unsigned char *)((char *)this + 0x1C) = (enabled != 0);
    ((cGUID *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], &loop, 1);
    *(unsigned char *)((char *)this + 0x1D) = (loop != 0);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```


## Prior Failure Notes

### Note 1

- session `7345832e`

Best result remains an 8/256 byte mismatch. The current source matches all control flow and calls, but SNC allocates the validated handle-table result to $a2 and the raw D_00038890 candidate to $a1; expected has the candidate in $a2 and the validated result in $a1. The differing instructions are the same register-field swap around the handle lookup and final beqz: li/lw/beqz/lw/move/beqz use a2 where expected uses a1, or vice versa. This is not a branch-likely divergence.

I tried additional source shapes beyond the prior session: assigning the table load through t before copying to e, hoisting e to function scope before and after t, introducing an explicit table local, using a typed handle-table entry struct, unsigned h, volatile/register-variable hints, and rewriting the candidate checks as assignment-in-condition and if/else branches. These either produced the same 8-byte register-allocation mismatch or regressed as far as 82 bytes. I restored the prior best C structure.

### Note 2

- session `c74a506c`

gcDesiredUIWidgetHelper::Read(cReadBlock&) - 8/256 bytes differ. Got within 8 bytes on first attempt and could not improve.

ROOT CAUSE: Pure register-allocation divergence. The expected disasm allocates the lookup result variable `t` to register $a1 (reusing the same register that held the `idx` intermediate after the `andi a1, a0, 0xffff` in the bnez delay slot). My SNC compiler instead allocates `t` to $a2 and the lookup intermediate `e` to $a1. This produces these specific differences:
- 0x80 (delay slot of `b 0xb0`): `li a2, 0` vs expected `li a1, 0`
- 0x94: `lw a1, 0(a1)` vs expected `lw a2, 0(a1)` (where lookup result e ends up)
- 0x9c (delay of beqz): `li a2, 0` vs expected `li a1, 0`
- 0xa0: `lw a3, 48(a1)` vs expected `lw a3, 48(a2)`
- 0xac: `move a2, a1` vs expected `move a1, a2`
- 0xb0: `beqz a2, end` vs expected `beqz a1, end`

All other bytes match perfectly including the dead-branch unreachable code (`andi a0, $zero, 0xff` materialized via inline asm `__asm__ volatile("andi %0, $0, 0xff" : "=r"(dead))`).

What was tried (6 attempts):
1. Initial: combined `if (t != 0 && dead)` -> 10B diff (also had beqzl divergence)
2. Nested ifs `if (t != 0) { ... if (dead) {...} }` -> 8B (best, kept)
3. Explicit `unsigned int idx = h & 0xFFFFu` declared before the if-else -> 16B (worse, broke reg alloc)
4. Combined e/t variable: `t = D_00038890[idx]; if (t && t->_30 != h) t = 0;` -> 15B (worse)
5. `void *t = 0; if (h != 0) { ... }` form (no else) -> SIZE MISMATCH (-4B, structure changes)
6. Permuter ran for 360 seconds, 6485 candidates compiled, 0 improvements found

The diff is NOT a bnel/branch-likely divergence (those are all matching), so this does NOT qualify for unmatchable_bnel. It's a register-allocation choice the SNC RA makes differently from the original release build, likely due to a subtle live-range/source-order interaction I haven't reproduced.

NEXT-AGENT HINTS:
- The structure is `if (h == 0) t=0; else { e = D_00038890[h&0xFFFF]; t=0; if (e && e->_30==h) t=e; }`
- Need to influence allocator to put `t` in $a1 (same reg as idx after delay-slot andi) instead of $a2.
- Possibly try: __asm__ register hints, declaring `t` after `e`, or using a single carrier variable.
- The handle-table lookup pattern in src/gcStateInfo.cpp (matched, gcStateInfo::IsValidState 0x0010BC34) uses the SAME source structure but compiles using the SAME a2-for-result allocation as mine — so the gcStateInfo example doesn't help. The expected disasm here shows a different allocation that the compiler is choosing based on something downstream of the lookup (live ranges in the dead-branch unreachable code? the next call's a1 arg?).
- The unreachable inline-asm-andi block (which materializes andi a0, $zero, 0xff) is correct and matches expected byte-for-byte; that's not the issue.


## Disassembly

```asm
12e8a0:	27bdffe0 	addiu	sp,sp,-32
12e8a4:	afb00014 	sw	s0,20(sp)
12e8a8:	00808025 	move	s0,a0
12e8ac:	8ca50000 	lw	a1,0(a1)
12e8b0:	03a02025 	move	a0,sp
12e8b4:	34060001 	li	a2,0x1
12e8b8:	afb10018 	sw	s1,24(sp)
12e8bc:	afbf001c 	sw	ra,28(sp)
12e8c0:	0c001740 	jal	0x5d00
12e8c4:	34070001 	li	a3,0x1
12e8c8:	8fa40000 	lw	a0,0(sp)
12e8cc:	02002825 	move	a1,s0
12e8d0:	8c840000 	lw	a0,0(a0)
12e8d4:	0c0013fd 	jal	0x4ff4
12e8d8:	34060004 	li	a2,0x4
12e8dc:	26110004 	addiu	s1,s0,4
12e8e0:	ae000004 	sw	zero,4(s0)
12e8e4:	0c0011e8 	jal	0x47a0
12e8e8:	02202025 	move	a0,s1
12e8ec:	02202025 	move	a0,s1
12e8f0:	03a02825 	move	a1,sp
12e8f4:	0c0024fb 	jal	0x93ec
12e8f8:	00403025 	move	a2,v0
12e8fc:	8fa40000 	lw	a0,0(sp)
12e900:	26050008 	addiu	a1,s0,8
12e904:	8c840000 	lw	a0,0(a0)
12e908:	0c0013fd 	jal	0x4ff4
12e90c:	34060004 	li	a2,0x4
12e910:	8e040004 	lw	a0,4(s0)
12e914:	14800003 	bnez	a0,0x12e924
12e918:	3085ffff 	andi	a1,a0,0xffff
12e91c:	1000000c 	b	0x12e950
12e920:	34050000 	li	a1,0x0
12e924:	3c060004 	lui	a2,0x4
12e928:	00052880 	sll	a1,a1,0x2
12e92c:	24c68890 	addiu	a2,a2,-30576
12e930:	00a62821 	addu	a1,a1,a2
12e934:	8ca60000 	lw	a2,0(a1)
12e938:	10c00005 	beqz	a2,0x12e950
12e93c:	34050000 	li	a1,0x0
12e940:	8cc70030 	lw	a3,48(a2)
12e944:	14e40002 	bne	a3,a0,0x12e950
12e948:	00000000 	nop
12e94c:	00c02825 	move	a1,a2
12e950:	10a0000c 	beqz	a1,0x12e984
12e954:	03a02025 	move	a0,sp
12e958:	300400ff 	andi	a0,zero,0xff
12e95c:	10800009 	beqz	a0,0x12e984
12e960:	03a02025 	move	a0,sp
12e964:	8e040008 	lw	a0,8(s0)
12e968:	3c058001 	lui	a1,0x8001
12e96c:	24a5ffff 	addiu	a1,a1,-1
12e970:	00852024 	and	a0,a0,a1
12e974:	3c050001 	lui	a1,0x1
12e978:	00852025 	or	a0,a0,a1
12e97c:	ae040008 	sw	a0,8(s0)
12e980:	03a02025 	move	a0,sp
12e984:	0c00176f 	jal	0x5dbc
12e988:	34050002 	li	a1,0x2
12e98c:	8fb00014 	lw	s0,20(sp)
12e990:	8fb10018 	lw	s1,24(sp)
12e994:	8fbf001c 	lw	ra,28(sp)
12e998:	03e00008 	jr	ra
12e99c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcDesiredUIWidgetHelper::Read(cReadBlock &)
// Address: 0x0012e8a0, Size: 256B
// Obj: gcAll_psp.obj

? cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(s32, void *, ?); /* extern */
? cHandle__Read_cReadBlockref_cMemPoolptr(void *, void *, s32); /* extern */
s32 cMemPool__GetPoolFromPtr_constvoidptrstatic(void *); /* extern */
? cReadBlock___dtor_cReadBlock_void(void *, ?);     /* extern */
? cReadBlock__cReadBlock_cFileref_unsignedint_bool(void *, s32, ?, ?); /* extern */

void gcDesiredUIWidgetHelper__Read_cReadBlockref(void *arg0, s32 *arg1) {
    s32 temp_a0;
    void *temp_a2;
    void *temp_s1;
    void *var_a1;

    cReadBlock__cReadBlock_cFileref_unsignedint_bool(sp, *arg1, 1, 1);
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, arg0, 4);
    temp_s1 = arg0 + 4;
    arg0->unk4 = 0;
    cHandle__Read_cReadBlockref_cMemPoolptr(temp_s1, sp, cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s1));
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, arg0 + 8, 4);
    temp_a0 = arg0->unk4;
    if (temp_a0 == 0) {
        var_a1 = NULL;
    } else {
        temp_a2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        var_a1 = NULL;
        if ((temp_a2 != NULL) && (temp_a2->unk30 == temp_a0)) {
            var_a1 = temp_a2;
        }
    }
    if ((var_a1 != NULL) && (0 & 0xFF)) {
        arg0->unk8 = (s32) ((arg0->unk8 & 0x8000FFFF) | 0x10000);
    }
    cReadBlock___dtor_cReadBlock_void(sp, 2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
