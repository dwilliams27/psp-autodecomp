# Battle Packet: `gcMsgPlayCinematic::New(nwMsgBuffer &) static`

## Target

- Address: `0x00264540`
- Size: `156` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMsgPlayCinematic`
- Method family: `New`
- Leaf: `True`

## Queue Metadata

- score: `182`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; New retry family; manageable size`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `constant_cse_codegen`, `needs_layout`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/gcMsgPlayCinematic.cpp`
- Header(s): `include/gcMsgPlayCinematic.h`
- Allowed source(s): `src/gcMsgPlayCinematic.cpp`
- Split-TU prefix: `src/gcMsgPlayCinematic_*.cpp`

## Compiler Guidance

New template: use pool allocation table lookup, null guard, placement construction or ctor wrapper, and return-null shape matching nearby matched factories.

## Class Header: `include/gcMsgPlayCinematic.h`

Not found.

## Matched Same-Class Neighbors

- `0x002644f4` 76B `gcMsgPlayCinematic::GetType(void) const` — `src/gcMsgPlayCinematic.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcEnumeration::New(cMemPool *, cBase *) static`

- Address: `0x00238d70`
- Size: `156` bytes
- Source: `src/gcDoViewportSetConfig.cpp`

```cpp
//   0x00238d70 gcEnumeration::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cFileHandle;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```

### Exemplar 2: `gcEntityAnimationConfigSet::New(cMemPool *, cBase *) static`

- Address: `0x0025a908`
- Size: `156` bytes
- Source: `src/gcEntityAnimationConfigSet.cpp`

```cpp
//   0x0025a908 gcEntityAnimationConfigSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```

### Exemplar 3: `gcEntityAttackSet::New(cMemPool *, cBase *) static`

- Address: `0x0025c6ec`
- Size: `156` bytes
- Source: `src/gcEntityAttackSet.cpp`

```cpp
//   0x0025c6ec gcEntityAttackSet::New(cMemPool *, cBase *) static

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```

### Exemplar 4: `gcDesiredEntityTemplate::New(cMemPool *, cBase *) static`

- Address: `0x00268e50`
- Size: `156` bytes
- Source: `src/gcValSaveGame.cpp`

```cpp
cBase *gcDesiredEntityTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredEntityTemplate *result = 0;
    gcDesiredEntityTemplate *obj =
        (gcDesiredEntityTemplate *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] = D_00389508;
        *(int *)((char *)obj + 0xC) = 7;
        *(int *)((char *)obj + 0x10) = 0;
        *(int *)((char *)obj + 0x14) = (int)parent | 1;
        result = obj;
    }
    return (cBase *)result;
}
```


## Prior Failure Notes

### Note 1

- session `027ec4ae`; src `src/gcMsgPlayCinematic.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/027ec4ae/00264540__agent_self_reported_failure__src_gcMsgPlayCinematic.cpp`

gcMsgPlayCinematic::New (156B static) — IN PROGRESS, current best 8/156 bytes diff (down from prior agent's 27/156). Permuter ran 240s/4206 candidates from this 8-byte baseline with zero improvement (best=8 across the entire run). The 8 differing bytes are pure register allocation: SNC chose t1($9) for the constant 1 AND reuses it for the zb[1] reload; chose a3($7)/t0($8) for the byte-zero sb sources. My output gets a1($5) for the 1, separate a3($7) for zb[1] reload (no reuse), and a1($5)/a2($6) for the byte zeros. The 4 logical register choices each appear in 2 instructions = 8 total bytes diff. Exact diff offsets: 0x44/0x4c (the '1' const), 0x5c/0x68 (the zb[1] reload), 0x6c/0x78 (b1=0 sb), 0x74/0x7c (b2=0 sb). Tried: removing all `register int x asm("$N")` hints (SNC ignores them, no change). Tried reusing the same variable name `int t = 1; t = zb[1]` (SSA breaks this — no merge). Tried int-typed b1/b2 with cast on store. Permuter from 8-byte baseline produced no improvement. Root cause: SNC's allocator picks $5 (a1) over $9 (t1) for the first scratch live interval because a1 was just freed (it held the cursor compute briefly). To force t1, we'd need to keep a1 alive across the '1' const's live range — likely requires a different decomposition of the function. Next agent: try expressing the cGUID and cHandle members as actual sub-objects with their own ctors that emit the addiu/sw pattern naturally. The structure `obj->mGuid = cGUID()` where cGUID has a default ctor `: a(0), b(0)` may produce different IR ordering. NOT bnel-related (decision 006 does not apply). Function structure is correct — only register allocation differs.

### Note 2

- session `7a85a75d`

gcMsgPlayCinematic::New (156B static) — close but not matched after 6 source attempts + 261s permuter run. Best diff: 27/156 bytes (started 63/156, permuter took it to 27/156, plateaued at best=27 across 5175 compiled candidates). The function bump-allocates 32 bytes from nwMsgBuffer.mOffset (+0x4B0) and runs a constructor that (a) zeroes a 32-byte stack temp via 8 sw zero stores at sp+0..sp+0x1C, (b) reloads sp+8 once into a register and reuses it as the source for sw to sp+0x10 and sp+0x18, (c) writes vtable=0x388FD0 at obj+0, mNum=1 at obj+4, copies 8 bytes from sp+0/+4 to obj+8/+0xC via an explicit (addiu base, a0, 8; sw,0(base); sw,4(base)) sequence, zeros obj+0x10, writes two byte zeros at obj+0x14/0x15 from li-loaded GPRs (a3, t0), copies 4 bytes from sp+0x18 to obj+0x18 via (addiu base, a0, 0x18), and stores 0.0f at obj+0x1C. Source structure that produces the right size+structure: a `volatile int zb[8]` init, `int *p1=(int*)((char*)obj+8); int *p2=(int*)((char*)obj+0x18);`, char b1=0,b2=0; locals for vtable/one/zf. Remaining 27-byte diff is purely register-allocation/scheduling: the original assigns a2 for vtable, t1 for the constant 1, a3/t0 for the byte-store sources, and interleaves the lui/addiu of vtable INSIDE the stack-store stream (between sw a1,0x18(sp) and sw a1,0x10(sp)). My output picks a1 for vtable (because a1 is freed after the cursor computation), and so the scheduler can't interleave the same way. Tried: ordering the local declarations to influence reg-alloc (vt before/after one/zf, p1/p2 before/after value loads), inserting/removing intermediate temporaries, swapping zb store order. Permuter explored 8496 candidates and stuck at 27 bytes — strong signal this is a register-allocation pinch the permuter can't solve via mechanical mutation. Next agent: try forcing vtable into a 'late-allocated' register by computing it AFTER more locals are live (e.g., move `void *vt=(void*)0x388FD0` to right before `obj->vtable=vt`). Also try: replace `int *p1=...; p1[0]=zb[0]; p1[1]=zb[1];` with a struct-pointer aliased copy (e.g. `*(MyPair*)((char*)obj+8) = *(MyPair*)&zb[0];`) which may force the addiu-base pattern more naturally. The class signature in the .h needs `static nwMsg *New(nwMsgBuffer&);` and the layout should be 32 bytes with members at the offsets noted above. Diffs are NOT bnel-related (bnel decision 006 does not apply).


## Disassembly

```asm
264540:	27bdffe0 	addiu	sp,sp,-32
264544:	8c8504b0 	lw	a1,1200(a0)
264548:	24a50020 	addiu	a1,a1,32
26454c:	ac8504b0 	sw	a1,1200(a0)
264550:	00852021 	addu	a0,a0,a1
264554:	1080001f 	beqz	a0,0x2645d4
264558:	34020000 	li	v0,0x0
26455c:	afa00000 	sw	zero,0(sp)
264560:	afa00008 	sw	zero,8(sp)
264564:	afa00004 	sw	zero,4(sp)
264568:	8fa50008 	lw	a1,8(sp)
26456c:	afa0000c 	sw	zero,12(sp)
264570:	3c060039 	lui	a2,0x39
264574:	afa50018 	sw	a1,24(sp)
264578:	24c68fd0 	addiu	a2,a2,-28720
26457c:	afa50010 	sw	a1,16(sp)
264580:	ac860000 	sw	a2,0(a0)
264584:	34090001 	li	t1,0x1
264588:	afa00014 	sw	zero,20(sp)
26458c:	ac890004 	sw	t1,4(a0)
264590:	8fa50000 	lw	a1,0(sp)
264594:	afa0001c 	sw	zero,28(sp)
264598:	24860008 	addiu	a2,a0,8
26459c:	8fa90004 	lw	t1,4(sp)
2645a0:	acc50000 	sw	a1,0(a2)
2645a4:	44806000 	mtc1	zero,$f12
2645a8:	acc90004 	sw	t1,4(a2)
2645ac:	34070000 	li	a3,0x0
2645b0:	ac800010 	sw	zero,16(a0)
2645b4:	34080000 	li	t0,0x0
2645b8:	a0870014 	sb	a3,20(a0)
2645bc:	a0880015 	sb	t0,21(a0)
2645c0:	8fa50018 	lw	a1,24(sp)
2645c4:	24860018 	addiu	a2,a0,24
2645c8:	acc50000 	sw	a1,0(a2)
2645cc:	e48c001c 	swc1	$f12,28(a0)
2645d0:	00801025 	move	v0,a0
2645d4:	03e00008 	jr	ra
2645d8:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcMsgPlayCinematic::New(nwMsgBuffer &) static
// Address: 0x00264540, Size: 156B
// Obj: gcAll_psp.obj

void *gcMsgPlayCinematic__New_nwMsgBufferrefstatic(void *arg0) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 temp_a1;
    void *temp_a0;
    void *var_v0;

    temp_a1 = arg0->unk4B0 + 0x20;
    arg0->unk4B0 = temp_a1;
    temp_a0 = arg0 + temp_a1;
    var_v0 = NULL;
    if (temp_a0 != NULL) {
        sp0 = 0;
        sp8 = 0;
        sp4 = 0;
        spC = 0;
        sp18 = sp8;
        sp10 = sp8;
        temp_a0->unk0 = 0x388FD0;
        sp14 = 0;
        temp_a0->unk4 = 1;
        sp1C = 0;
        temp_a0->unk8 = sp0;
        (temp_a0 + 8)->unk4 = sp4;
        temp_a0->unk10 = 0;
        temp_a0->unk14 = 0;
        temp_a0->unk15 = 0;
        temp_a0->unk18 = sp8;
        temp_a0->unk1C = 0.0f;
        var_v0 = temp_a0;
    }
    return var_v0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
