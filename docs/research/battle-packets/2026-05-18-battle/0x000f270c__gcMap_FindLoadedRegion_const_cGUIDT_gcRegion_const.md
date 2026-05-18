# Battle Packet: `gcMap::FindLoadedRegion(const cGUIDT<gcRegion> &) const`

## Target

- Address: `0x000f270c`
- Size: `100` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMap`
- Method family: `FindLoadedRegion`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 3B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `3`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `3`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context


Callers:
- `0x000f20a8` `untried` 340B `gcMap::LoadRegion(const cGUIDT<gcRegion> &, int, bool)`
- `0x000f2390` `untried` 820B `gcMap::LoadRegionsBackground(const cGUIDT<gcRegion> *, const gcEvent *, const cGUIDT<gcRegion> *, bool, cGUIDT)`
- `0x00136880` `matched` 332B `gcMsgUpdateRegionState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x001369cc` `untried` 388B `gcMsgUpdateRegionState::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)`

## Placement

- Canonical source: `src/gcMap.cpp`
- Header(s): `include/gcMap.h`
- Allowed source(s): `src/gcMap.cpp`
- Split-TU prefix: `src/gcMap_*.cpp`

## Class Header: `include/gcMap.h`

```cpp
#ifndef GCMAP_H
#define GCMAP_H

class gcRegion;
class gcRegionBase;
class gcEntity;
class cBase;
class cMemPool;
class cFile;
class cType;
class cGUID;
class gcEvent;
class gcReplicationVisitor;

template <class T>
class cGUIDT {
public:
    int mA;
    int mB;
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

class gcMap {
public:
    struct cObjectLoad {
        char pad_000[8];
        int mField8;
        int mFieldC;
        cObjectLoad();
    };

    bool AreRegionsLoading(void) const;
    gcMap(cBase *, int);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void *GetGroups(int *) ;
    void *GetContainedFactories(int *);
    gcRegion *GetLoadedRegion(int) const;
    void GetLoadedRegions(cGUIDT<gcRegion> *) const;
    void OnRegionDeleted(gcRegion *);
    void LoadObjectBackgroundEnd(void);
    int AddToWorld(void);
    void PostUpdate(void);
    void HandleCinematicSkip(void);
    void HandleNextRegionLoad(void);
    void LoadRegionsBackground(int, int, const gcEvent *, const gcEvent *, bool, bool);
    void ResetNetConnection(int);
    void CancelCurrentCinematic(void);
    void LoadObjectBackgroundBegin(cHandleT<gcEntity>);
    void UnloadAllRegions(void);
    void DeleteRegion(int);
    void DeleteDynamicLoadedObjects(int, cHandleT<gcEntity>);
    void ResetRegionStates(void);
    void CaptureRegionStates(void);
    void CaptureRegionState(gcRegionBase *);
    void ApplyRegionStates(void);
    void ApplyRegionState(gcRegionBase *);
    gcRegion *FindLoadedRegion(const cGUIDT<gcRegion> &) const;
    void ClearRegionSetState(int, int);
    int IsDynamicallyLoadedObjectSetToLoad(const cGUID &) const;
    int IsStreamedCinematicChained(void) const;
    void GetRegionSetName(unsigned int, char *) const;
    int FindRegionSet(unsigned int, int *, int *) const;
    static int IsMapLoading(bool);
    static int IsObjectLoading(void);
    void LoadRegionSet(const cGUIDT<gcRegion> *);
    void LoadRegionSet(int, int);
    int FillRegionList(cGUIDT<gcRegion> *, int) const;
    void LoadRegion(const cGUIDT<gcRegion> &, int, bool);
    void MemCardReplicate(gcReplicationVisitor &);
    void MemCardReplicateStreamedCinematic(gcReplicationVisitor &);
    void MemCardReplicateStreamedCinematicLoad(gcReplicationVisitor &);

    char pad_000[0x140];
    int mFlags;                             // 0x140
    char pad_144[4];                        // 0x144
    int mGroups[42];                        // 0x148
    char pad_1F0[0x10];                     // 0x1F0
    int *mObjectLoadArray;                  // 0x200
    char pad_204[4];                        // 0x204
    int mLoadedCount;                       // 0x208
    char pad_20C[4];                        // 0x20C
    gcRegion *mLoadedRegions[2];            // 0x210
    char pad_218[0x3E4 - 0x218];            // 0x218
    cHandleT<gcEntity> mLoadBackgroundHandle;  // 0x3E4
    cHandleT<gcEntity> mDeleteDynamicHandle;   // 0x3E8
};

#endif
```

## Matched Same-Class Neighbors

- `0x000f0fcc` 428B `gcMap::Write(cFile &) const` — `src/gcMap.cpp`
- `0x000f1178` 8B `gcMap::operator=(const gcMap &)` — `src/gcReplicationVisitor.cpp`
- `0x000f1858` 124B `gcMap::IsMapLoading(bool) static` — `src/gcMap.cpp`
- `0x000f199c` 12B `gcMap::AreRegionsLoading(void) const` — `src/gcMap_AreRegionsLoading.cpp`
- `0x000f19a8` 152B `gcMap::IsObjectLoading(void) static` — `src/gcMap.cpp`

## Prior Failure Notes

### Note 1

- session `724544f5`

Best C++ attempt is structurally correct and compiles to the right 100-byte size, but compare_func remains at 3/100 differing bytes. The remaining diff is a single equivalent instruction in the inner GUID comparison delay slot: expected has `andi a3,a3,0xff` after the second word `bne`, while our SNC consistently folds that expression to `li a3,0`; all surrounding control flow, registers, branch targets, loop cursor, and return paths match.

Tried direct nested ifs, m2c-style split masked temporaries with goto labels, inverted first/second mismatch labels, int/bool/unsigned-char result locals, assigning the mask back to the same variable versus a separate masked variable, an inline cGUIDT equality operator (rejected because it emitted a separate linkonce operator symbol), short-circuit boolean assignment (regressed to xor/sltiu and 96B), and zero-code register/memory barriers. Ran `python3 tools/permuter.py src/gcMap.cpp 0x000f270c --time 300 --save-best`; it generated 10848 candidates, compiled 3173, and found zero improvements over the 3-byte baseline. This is not a bnel issue: the differing instruction is li vs andi, not branch-likely selection.


## Disassembly

```asm
f270c:	34060000 	li	a2,0x0
f2710:	24840210 	addiu	a0,a0,528
f2714:	8c820000 	lw	v0,0(a0)
f2718:	5040000e 	beqzl	v0,0xf2754
f271c:	24c60001 	addiu	a2,a2,1
f2720:	8c480020 	lw	t0,32(v0)
f2724:	8ca90000 	lw	t1,0(a1)
f2728:	15090006 	bne	t0,t1,0xf2744
f272c:	34070000 	li	a3,0x0
f2730:	8c480024 	lw	t0,36(v0)
f2734:	8ca90004 	lw	t1,4(a1)
f2738:	15090003 	bne	t0,t1,0xf2748
f273c:	30e700ff 	andi	a3,a3,0xff
f2740:	34070001 	li	a3,0x1
f2744:	30e700ff 	andi	a3,a3,0xff
f2748:	14e00007 	bnez	a3,0xf2768
f274c:	00000000 	nop
f2750:	24c60001 	addiu	a2,a2,1
f2754:	28c70002 	slti	a3,a2,2
f2758:	14e0ffee 	bnez	a3,0xf2714
f275c:	24840004 	addiu	a0,a0,4
f2760:	03e00008 	jr	ra
f2764:	00001025 	move	v0,zero
f2768:	03e00008 	jr	ra
f276c:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcMap::FindLoadedRegion(const cGUIDT<gcRegion> &) const
// Address: 0x000f270c, Size: 100B
// Obj: gcAll_psp.obj

void *gcMap__FindLoadedRegion_constcGUIDT_gcRegion_refconst(s32 arg0, void *arg1) {
    s32 var_a2;
    s32 var_a3;
    s32 var_a3_2;
    void **var_a0;
    void *temp_v0;

    var_a2 = 0;
    var_a0 = arg0 + 0x210;
loop_1:
    temp_v0 = *var_a0;
    if (temp_v0 != NULL) {
        var_a3 = 0;
        if (temp_v0->unk20 == arg1->unk0) {
            var_a3_2 = 0 & 0xFF;
            if (temp_v0->unk24 == arg1->unk4) {
                var_a3 = 1;
                goto block_5;
            }
        } else {
block_5:
            var_a3_2 = var_a3 & 0xFF;
        }
        if (var_a3_2 == 0) {
            goto block_7;
        }
        return temp_v0;
    }
block_7:
    var_a2 += 1;
    var_a0 += 4;
    if (var_a2 >= 2) {
        return NULL;
    }
    goto loop_1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
