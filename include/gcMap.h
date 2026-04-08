#ifndef GCMAP_H
#define GCMAP_H

class gcRegion;

class gcMap {
public:
    bool AreRegionsLoading(void) const;
    void *GetGroups(int *) ;
    void *GetContainedFactories(int *);
    gcRegion *GetLoadedRegion(int) const;
    void OnRegionDeleted(gcRegion *);
    void LoadObjectBackgroundEnd(void);

    char pad_000[0x140];
    int mFlags;                    // 0x140
    char pad_144[4];               // 0x144
    int mGroups[42];               // 0x148
    char pad_1F0[0x20];            // 0x1F0
    gcRegion *mLoadedRegions[2];   // 0x210
};

#endif
