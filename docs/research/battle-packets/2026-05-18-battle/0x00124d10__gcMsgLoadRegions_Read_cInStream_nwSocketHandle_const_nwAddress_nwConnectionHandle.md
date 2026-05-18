# Battle Packet: `gcMsgLoadRegions::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)`

## Target

- Address: `0x00124d10`
- Size: `228` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMsgLoadRegions`
- Method family: `Read`
- Leaf: `False`

## Queue Metadata

- score: `128`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 16B; codegen-specific diagnosis; Read hard family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `16`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000024f8` `matched` 84B `cGUID::Read(cInStream &)`
- `0x000f2390` `untried` 820B `gcMap::LoadRegionsBackground(const cGUIDT<gcRegion> *, const gcEvent *, const cGUIDT<gcRegion> *, bool, cGUIDT)`

## Placement

- Canonical source: `src/gcMsgLoadRegions.cpp`
- Header(s): `include/gcMsgLoadRegions.h`
- Allowed source(s): `src/gcMsgLoadRegions.cpp`
- Split-TU prefix: `src/gcMsgLoadRegions_*.cpp`

## Compiler Guidance

Read template: preserve read-block/stream construction, old-position restore paths, and result initialization location. Compare against same stream type exemplars when available.

## Class Header: `include/gcMsgLoadRegions.h`

Not found.

## Matched Same-Class Neighbors

- `0x00124b94` 152B `gcMsgLoadRegions::gcMsgLoadRegions(cGUIDT<gcRegion> *, bool)` — `src/gcMsgLoadRegions.cpp`
- `0x00124c2c` 228B `gcMsgLoadRegions::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` — `src/gcMsgLoadRegions.cpp`
- `0x0026484c` 76B `gcMsgLoadRegions::GetType(void) const` — `src/gcMsgRequestLoadedState.cpp`
- `0x00264898` 96B `gcMsgLoadRegions::New(nwMsgBuffer &) static` — `src/gcMsgLoadRegions.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcEnumerationEntry::Read(cFile &, cMemPool *)`

- Address: `0x000d3dd0`
- Size: `228` bytes
- Source: `src/gcEnumerationEntry.cpp`

```cpp
int gcEnumerationEntry::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19") = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2 || cNamed::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], (char *)this + 0x24, 4);
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], (char *)this + 0x20, 1);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 2: `gcDesiredCamera::Read(cFile &, cMemPool *)`

- Address: `0x001214f0`
- Size: `224` bytes
- Source: `src/gcDesiredCamera.cpp`

```cpp
// 0x001214f0 - gcDesiredCamera::Read(cFile &, cMemPool *)
int gcDesiredCamera::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 8, 4);
    char *typeInfo = *(char **)((char *)this + 0x10);
    char *base = (char *)this + 0x0C;
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    short offset = rec->offset;
    cFileHandle *handle = *(cFileHandle **)&rb[0];
    char *adjusted = base + offset;
    cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
    void (*fn)(void *, cFileHandle *, cMemPool *) = rec->fn;
    fn(adjusted, handle, childPool);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 3: `gcBoolSet::Read(cFile &, cMemPool *)`

- Address: `0x000d4fc4`
- Size: `232` bytes
- Source: `src/gcBoolSet.cpp`

```cpp
//   0x000d4fc4 gcBoolSet::Read(cFile &, cMemPool *)

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```

### Exemplar 4: `gcFloatSet::Read(cFile &, cMemPool *)`

- Address: `0x000fea3c`
- Size: `232` bytes
- Source: `src/gcFloatSet.cpp`

```cpp
//   0x000fea3c gcFloatSet::Read(cFile &, cMemPool *)

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
```


## Prior Failure Notes

### Note 1

- session `a23450bd`

Rechecked the existing gcMsgLoadRegions::Read implementation after adding the constructor. It remains a 16/228 byte mismatch. The functional body still matches the expected shape: two cGUID::Read calls, the inlined cInStream bit read, the map readiness branch-likely check, and the final LoadRegionsBackground call sequence using the small inline asm block.

The remaining difference is still the prologue scheduling described by the prior session: this build saves ra immediately after s0-s3, then initializes s3/s2/s0/s1; the target initializes those saved-register locals first and saves ra immediately before the first cGUID::Read call. I did not find a new source-level shape that changes this without disturbing the matched body. This is not a bnel issue.

### Note 2

- session `65d6f170`

Best result is a 16/228 byte near-match. The functional body matches the target shape: two cGUID::Read calls over the region GUID array, the inlined cInStream bit read, the global gcMap load at 0x37D7FC, the branch-likely readiness check on map->mFlags & 1, and the final LoadRegionsBackground call with a0=map, a1=this+4, a2=0, a3=0, t0=mFlag, t1=0 in the jal delay slot. The nonstandard final call required a small inline-asm block with .set noreorder; plain C prototypes either passed the by-value cGUIDT through the stack or preserved this+4 in an extra saved register, producing much larger diffs.

The remaining mismatch is purely prologue scheduling: our SNC saves ra at 0x74 before initializing s3/s2/s0/s1, while the target saves ra after those register initializations just before the first cGUID::Read call. Tried do-loop and for-loop forms, cGUIDT template/inheritance so the cGUID::Read relocation is correct, normal extern-C scalar alias for the LoadRegionsBackground call, inline asm with and without ra clobber, an empty memory barrier after loop variable setup, -Xsched=0/1 and -Xmopt=0 checks, and the permuter for 300s (7,344 candidates generated, 4,210 compiled, zero improvements from best=16). This is not a bnel issue; all remaining differences are the position of the ra save and the resulting shifted prologue bytes.


## Disassembly

```asm
124d10:	27bdffe0 	addiu	sp,sp,-32
124d14:	afb00000 	sw	s0,0(sp)
124d18:	afb10004 	sw	s1,4(sp)
124d1c:	afb20008 	sw	s2,8(sp)
124d20:	afb3000c 	sw	s3,12(sp)
124d24:	34130000 	li	s3,0x0
124d28:	24920004 	addiu	s2,a0,4
124d2c:	00808025 	move	s0,a0
124d30:	00a08825 	move	s1,a1
124d34:	afbf0010 	sw	ra,16(sp)
124d38:	02402025 	move	a0,s2
124d3c:	0c00093e 	jal	0x24f8
124d40:	02202825 	move	a1,s1
124d44:	26730001 	addiu	s3,s3,1
124d48:	2a640002 	slti	a0,s3,2
124d4c:	1480fffa 	bnez	a0,0x124d38
124d50:	26520008 	addiu	s2,s2,8
124d54:	8e240008 	lw	a0,8(s1)
124d58:	8e250000 	lw	a1,0(s1)
124d5c:	000420c3 	sra	a0,a0,0x3
124d60:	26260008 	addiu	a2,s1,8
124d64:	00a42021 	addu	a0,a1,a0
124d68:	8cc50000 	lw	a1,0(a2)
124d6c:	90840000 	lbu	a0,0(a0)
124d70:	30a70007 	andi	a3,a1,0x7
124d74:	34080001 	li	t0,0x1
124d78:	00e83804 	sllv	a3,t0,a3
124d7c:	24a50001 	addiu	a1,a1,1
124d80:	00872024 	and	a0,a0,a3
124d84:	acc50000 	sw	a1,0(a2)
124d88:	0004202b 	sltu	a0,zero,a0
124d8c:	a2040014 	sb	a0,20(s0)
124d90:	3c040038 	lui	a0,0x38
124d94:	8c91d7fc 	lw	s1,-10244(a0)
124d98:	1220000f 	beqz	s1,0x124dd8
124d9c:	00000000 	nop
124da0:	8e250140 	lw	a1,320(s1)
124da4:	34040000 	li	a0,0x0
124da8:	30a50001 	andi	a1,a1,0x1
124dac:	54a00001 	bnezl	a1,0x124db4
124db0:	34040001 	li	a0,0x1
124db4:	10800008 	beqz	a0,0x124dd8
124db8:	00000000 	nop
124dbc:	92080014 	lbu	t0,20(s0)
124dc0:	26050004 	addiu	a1,s0,4
124dc4:	02202025 	move	a0,s1
124dc8:	00003025 	move	a2,zero
124dcc:	00003825 	move	a3,zero
124dd0:	0c03c8e4 	jal	0xf2390
124dd4:	00004825 	move	t1,zero
124dd8:	8fb00000 	lw	s0,0(sp)
124ddc:	8fb10004 	lw	s1,4(sp)
124de0:	8fb20008 	lw	s2,8(sp)
124de4:	8fb3000c 	lw	s3,12(sp)
124de8:	8fbf0010 	lw	ra,16(sp)
124dec:	03e00008 	jr	ra
124df0:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcMsgLoadRegions::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)
// Address: 0x00124d10, Size: 228B
// Obj: gcAll_psp.obj

? cGUID__Read_cInStreamref(s32, void *);            /* extern */
? gcMap__LoadRegionsBackground_constcGUIDT_gcRegion_ptr_constgcEventptr_constcGUIDT_gcRegion_ptr_bool_cGUIDT(void *, void *, ?, ?); /* extern */

void gcMsgLoadRegions__Read_cInStreamref_nwSocketHandle_constnwAddressref_nwConnectionHandle(void *arg0, void *arg1) {
    s32 temp_a0;
    s32 temp_a1;
    s32 var_a0;
    s32 var_s2;
    s32 var_s3;
    void *temp_s1;

    var_s3 = 0;
    var_s2 = arg0 + 4;
    do {
        cGUID__Read_cInStreamref(var_s2, arg1);
        var_s3 += 1;
        var_s2 += 8;
    } while (var_s3 < 2);
    temp_a1 = arg1->unk8;
    temp_a0 = *(arg1->unk0 + ((s32) arg1->unk8 >> 3)) & (1 << (temp_a1 & 7));
    arg1->unk8 = (s32) (temp_a1 + 1);
    arg0->unk14 = (u8) (temp_a0 != 0);
    temp_s1 = *(void **)0x37D7FC;
    if (temp_s1 != NULL) {
        var_a0 = 0;
        if (temp_s1->unk140 & 1) {
            var_a0 = 1;
        }
        if (var_a0 != 0) {
            gcMap__LoadRegionsBackground_constcGUIDT_gcRegion_ptr_constgcEventptr_constcGUIDT_gcRegion_ptr_bool_cGUIDT(temp_s1, arg0 + 4, 0, 0);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
