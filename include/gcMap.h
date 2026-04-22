#ifndef GCMAP_H
#define GCMAP_H

class gcRegion;
class gcEntity;

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
    void *GetGroups(int *) ;
    void *GetContainedFactories(int *);
    gcRegion *GetLoadedRegion(int) const;
    void OnRegionDeleted(gcRegion *);
    void LoadObjectBackgroundEnd(void);
    void PostUpdate(void);
    void HandleCinematicSkip(void);
    void LoadObjectBackgroundBegin(cHandleT<gcEntity>);
    void UnloadAllRegions(void);
    void DeleteRegion(int);
    void DeleteDynamicLoadedObjects(int, cHandleT<gcEntity>);
    void ResetRegionStates(void);
    void ClearRegionSetState(int, int);

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
