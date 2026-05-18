# Battle Packet: `eDynamicMesh::New(cMemPool *, cBase *) static`

## Target

- Address: `0x001f3b7c`
- Size: `124` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eDynamicMesh`
- Method family: `New`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 12B; New retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0004e234` `matched` 152B `eDynamicMesh::eDynamicMesh(cBase *)`

## Placement

- Canonical source: `src/eDynamicMesh.cpp`
- Header(s): `include/eDynamicMesh.h`
- Allowed source(s): `src/eDynamicMesh.cpp`
- Split-TU prefix: `src/eDynamicMesh_*.cpp`

## Compiler Guidance

New template: use pool allocation table lookup, null guard, placement construction or ctor wrapper, and return-null shape matching nearby matched factories.

## Class Header: `include/eDynamicMesh.h`

```cpp
#pragma once

#include "cObject.h"

class cFile;
class cMemPool;
class cName;

// eDynamicMesh: mesh data object (inherits cObject).
// cObject base: offsets 0x00..0x43 (from cObject ctor 0x9d28)
// Struct size: at least 0x84 bytes
class eDynamicMesh : public cObject {
public:
    eDynamicMesh(cBase *);
    ~eDynamicMesh();

    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void PlatformFree();
    int GetNodeIndex(const cName &, int) const;
    int GetCollisionShapeIndex(const cName &) const;
    int HasSkin() const;
    void Free();
    void Reset(cMemPool *, bool);

    void AssignCopy(const cBase *);
    static eDynamicMesh *New(cMemPool *, cBase *);
};
```

## Matched Same-Class Neighbors

- `0x0004e1fc` 56B `eDynamicMesh::PlatformFree(void)` — `src/eDynamicMesh.cpp`
- `0x0004e234` 152B `eDynamicMesh::eDynamicMesh(cBase *)` — `src/eDynamicModel.cpp`
- `0x0004e2cc` 644B `eDynamicMesh::~eDynamicMesh(void)` — `src/eDynamicMesh.cpp`
- `0x0004e740` 32B `eDynamicMesh::HasSkin(void) const` — `src/eDynamicMesh.cpp`
- `0x001f3a9c` 224B `eDynamicMesh::AssignCopy(const cBase *)` — `src/eDynamicMesh.cpp`

## Matched Method Exemplars

### Exemplar 1: `eConfigBase::New(cMemPool *, cBase *) static`

- Address: `0x001df384`
- Size: `124` bytes
- Source: `src/eConfigBase.cpp`

```cpp
// ── eConfigBase::New(cMemPool *, cBase *) static @ 0x001df384 ──
cBase *eConfigBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eConfigBase *result = 0;
    eConfigBase *obj = (eConfigBase *)rec->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eConfigBase__eConfigBase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
```

### Exemplar 2: `eConfigPSP::New(cMemPool *, cBase *) static`

- Address: `0x001df84c`
- Size: `124` bytes
- Source: `src/eConfigPSP.cpp`

```cpp
// eConfigPSP::New(cMemPool *, cBase *) static @ 0x001df84c
cBase *eConfigPSP::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eConfigPSP *result = 0;
    eConfigPSP *obj = (eConfigPSP *)rec->fn(base, 0x94, 4, 0, 0);
    if (obj != 0) {
        eConfigPSP__eConfigPSP_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
```

### Exemplar 3: `eSound::New(cMemPool *, cBase *) static`

- Address: `0x001e0250`
- Size: `124` bytes
- Source: `src/eSound.cpp`

```cpp
//   eSound::New(cMemPool *, cBase *) static     @ 0x001e0250  (124B)
//
// cObject is declared with INLINE ctor/dtor bodies so SNC inlines them into
// eSound's ctor (leaf, no jal) and dtor (cObject vtable written inline at
// the chain point, no jal to ~cObject). Compare with eSoundData.cpp where
// cObject is non-inline and the ctor/dtor each emit a jal to cObject's
// implementation in cAll_psp.obj.

class cBase;
class cFile;
class cMemPool;
class cType;
class eSound;
class eWorld;

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T> T *dcast(const cBase *);

extern char eSoundvirtualtable[];      // 0x37FB80
extern char cObjectvirtualtable[];     // 0x37E990
extern cType *D_000385DC;
extern cType *D_00040F74;

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};
```

### Exemplar 4: `eBspTree::New(cMemPool *, cBase *) static`

- Address: `0x001e0b40`
- Size: `124` bytes
- Source: `src/eBspTree.cpp`

```cpp
cBase *eBspTree::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eBspTree *result = 0;
    eBspTree *obj = (eBspTree *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) eBspTree(parent);
        result = obj;
    }
    return (cBase *)result;
}
```


## Prior Failure Notes

### Note 1

- session `d9389656`

The C++ factory implementation follows the matched pool-allocation pattern: use pool slot 9, allocation table +0x28, offset + allocator callback, size 0x84, alignment 4, null guard, call eDynamicMesh constructor wrapper, and return null or the constructed object. Under the current Makefile default -Xsched=2 it still mismatches by 12/124 bytes: the body bytes match, but the prologue schedules `move s0,a1` before the full save sequence, while the original saves s0/s1/s2/ra first and only then moves parent into s0 and initializes s2 to zero.

A direct manual compile with `-Xsched=1` for the same src/eDynamicMesh.cpp produced relocation-masked diff 0 for `__0fMeDynamicMeshDNewP6IcMemPoolP6FcBaseT`. This confirms the source is byte-exact under the correct scheduler setting. Agents are not allowed to edit the Makefile, so the remaining blocker is adding a per-file sched=1 override for `build/src/eDynamicMesh.cpp.o`.

### Note 2

- session `3205687c`

C source is a direct translation of the pool-allocation pattern used by eBumpOffsetMap::New / eTextureMap::New (verified matched). At sched=2 (current Makefile default), 16 bytes differ: the register save prologue is reordered. At sched=1 my compiled output is byte-identical to the expected (verified by direct pspsnc.exe invocation).

This function is at 0x1f3b7c — OUTSIDE the known sched=1 zone (0x6e000-0xbab28) and outside the transition zone. Suggests eDynamicMesh class methods may be in a separate sched=1 region of the eAll_psp.obj unity build. Fix: add `$(BUILD_DIR)/src/eDynamicMesh.cpp.o: CFLAGS := $(ECFLAGS)` to the Makefile. Agents cannot modify the Makefile, so reporting as failed per SCHED HINT instructions.


## Disassembly

```asm
1f3b7c:	27bdfff0 	addiu	sp,sp,-16
1f3b80:	afb00000 	sw	s0,0(sp)
1f3b84:	afb10004 	sw	s1,4(sp)
1f3b88:	afb20008 	sw	s2,8(sp)
1f3b8c:	afbf000c 	sw	ra,12(sp)
1f3b90:	00a08025 	move	s0,a1
1f3b94:	34120000 	li	s2,0x0
1f3b98:	8c840024 	lw	a0,36(a0)
1f3b9c:	8c85001c 	lw	a1,28(a0)
1f3ba0:	24a50028 	addiu	a1,a1,40
1f3ba4:	84a60000 	lh	a2,0(a1)
1f3ba8:	00862021 	addu	a0,a0,a2
1f3bac:	8ca90004 	lw	t1,4(a1)
1f3bb0:	34050084 	li	a1,0x84
1f3bb4:	34060004 	li	a2,0x4
1f3bb8:	00003825 	move	a3,zero
1f3bbc:	0120f809 	jalr	t1
1f3bc0:	00004025 	move	t0,zero
1f3bc4:	00408825 	move	s1,v0
1f3bc8:	12200004 	beqz	s1,0x1f3bdc
1f3bcc:	02202025 	move	a0,s1
1f3bd0:	0c01388d 	jal	0x4e234
1f3bd4:	02002825 	move	a1,s0
1f3bd8:	02209025 	move	s2,s1
1f3bdc:	02401025 	move	v0,s2
1f3be0:	8fb00000 	lw	s0,0(sp)
1f3be4:	8fb10004 	lw	s1,4(sp)
1f3be8:	8fb20008 	lw	s2,8(sp)
1f3bec:	8fbf000c 	lw	ra,12(sp)
1f3bf0:	03e00008 	jr	ra
1f3bf4:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eDynamicMesh::New(cMemPool *, cBase *) static
// Address: 0x001f3b7c, Size: 124B
// Obj: eAll_psp.obj

? eDynamicMesh__eDynamicMesh_cBaseptr(s32, ?);      /* extern */

s32 eDynamicMesh__New_cMemPoolptr_cBaseptrstatic(void *arg0, ? arg1) {
    s32 temp_v0;
    s32 var_s2;
    void *temp_a0;
    void *temp_a1;

    var_s2 = 0;
    temp_a0 = arg0->unk24;
    temp_a1 = temp_a0->unk1C;
    temp_v0 = (temp_a1 + 0x28)->unk4(temp_a0 + temp_a1->unk28, 0x84, 4, 0);
    if (temp_v0 != 0) {
        eDynamicMesh__eDynamicMesh_cBaseptr(temp_v0, arg1);
        var_s2 = temp_v0;
    }
    return var_s2;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
