# Battle Packet: `gcMap::LoadRegionSet(int, int)`

## Target

- Address: `0x000f2bf8`
- Size: `148` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMap`
- Method family: `LoadRegionSet`
- Leaf: `False`

## Queue Metadata

- score: `139`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 17B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `17`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `17`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000f2c8c` `matched` 104B `gcMap::LoadRegionSet(const cGUIDT<gcRegion> *)`

Callers:
- `0x000f1a40` `untried` 416B `gcMap::Load(bool)`

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

- session `d3a7655e`; src `src/gcMap.cpp`; snapshot `logs/failure_snapshots/match_20260505_191203/d3a7655e/000f2bf8__agent_self_reported_failure__src_gcMap.cpp`

gcMap::LoadRegionSet(int, int) — control flow structure matches expected exactly: bltz/bnezl/slt bounds check pattern x2, with goto label structure mirroring the matched ClearRegionSetState. Best diff: 17/148 bytes. The remaining differences are register allocation: SNC chose a1 for both 'group' and 'set' (chaining loads into same register), while expected put 'group' in a3 and 'set' in a1. Tried: void* vs gcRegionSetGroup* typing, set=0 init at decl, if(group==0) goto vs if(group!=0) wrapping, declaring set inside vs outside if-block, dummy variable trick. Permuter ran 180s, 1969 candidates, ZERO improvement. Root cause: SNC register allocator merges group/sets/set into one register chain in mine, expected splits group→a3, set→a1 to allow set=0 to fill the beqz delay slot. Source preserved at lines 394-435 in src/gcMap.cpp for retry.


## Disassembly

```asm
f2bf8:	27bdfff0 	addiu	sp,sp,-16
f2bfc:	afbf0000 	sw	ra,0(sp)
f2c00:	04a00008 	bltz	a1,0xf2c24
f2c04:	00000000 	nop
f2c08:	8c870398 	lw	a3,920(a0)
f2c0c:	34080000 	li	t0,0x0
f2c10:	54e00001 	bnezl	a3,0xf2c18
f2c14:	8ce8fffc 	lw	t0,-4(a3)
f2c18:	00a8402a 	slt	t0,a1,t0
f2c1c:	15000003 	bnez	t0,0xf2c2c
f2c20:	00052880 	sll	a1,a1,0x2
f2c24:	10000003 	b	0xf2c34
f2c28:	34070000 	li	a3,0x0
f2c2c:	00e52821 	addu	a1,a3,a1
f2c30:	8ca70000 	lw	a3,0(a1)
f2c34:	10e0000e 	beqz	a3,0xf2c70
f2c38:	34050000 	li	a1,0x0
f2c3c:	04c00008 	bltz	a2,0xf2c60
f2c40:	00000000 	nop
f2c44:	8ce70020 	lw	a3,32(a3)
f2c48:	34050000 	li	a1,0x0
f2c4c:	54e00001 	bnezl	a3,0xf2c54
f2c50:	8ce5fffc 	lw	a1,-4(a3)
f2c54:	00c5282a 	slt	a1,a2,a1
f2c58:	14a00003 	bnez	a1,0xf2c68
f2c5c:	00062880 	sll	a1,a2,0x2
f2c60:	10000003 	b	0xf2c70
f2c64:	34050000 	li	a1,0x0
f2c68:	00e52821 	addu	a1,a3,a1
f2c6c:	8ca50000 	lw	a1,0(a1)
f2c70:	10a00003 	beqz	a1,0xf2c80
f2c74:	00000000 	nop
f2c78:	0c03cb23 	jal	0xf2c8c
f2c7c:	24a50008 	addiu	a1,a1,8
f2c80:	8fbf0000 	lw	ra,0(sp)
f2c84:	03e00008 	jr	ra
f2c88:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcMap::LoadRegionSet(int, int)
// Address: 0x000f2bf8, Size: 148B
// Obj: gcAll_psp.obj

? gcMap__LoadRegionSet_constcGUIDT_gcRegion_ptr(s32, void *); /* extern */

void gcMap__LoadRegionSet_int_int(void *arg0, s32 arg1, s32 arg2) {
    s32 var_a1;
    s32 var_a1_2;
    s32 var_t0;
    void *temp_a3;
    void *var_a3;

    if (arg1 >= 0) {
        temp_a3 = arg0->unk398;
        var_t0 = 0;
        if (temp_a3 != NULL) {
            var_t0 = temp_a3->unk-4;
        }
        if (arg1 >= var_t0) {
            goto block_4;
        }
        var_a3 = *(temp_a3 + (arg1 * 4));
    } else {
block_4:
        var_a3 = NULL;
    }
    var_a1 = 0;
    if (var_a3 != NULL) {
        if (arg2 >= 0) {
            var_a3 = var_a3->unk20;
            var_a1_2 = 0;
            if (var_a3 != NULL) {
                var_a1_2 = var_a3->unk-4;
            }
            if (arg2 >= var_a1_2) {
                goto block_11;
            }
            var_a1 = *(var_a3 + (arg2 * 4));
        } else {
block_11:
            var_a1 = 0;
        }
    }
    if (var_a1 != 0) {
        gcMap__LoadRegionSet_constcGUIDT_gcRegion_ptr(var_a1 + 8, var_a3);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
