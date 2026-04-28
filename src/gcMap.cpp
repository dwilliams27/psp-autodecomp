#include "gcMap.h"

inline void *operator new(unsigned int, void *p) { return p; }

struct gcMap_AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcMap::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    gcMap_AllocRec *rec = (gcMap_AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcMap *result = 0;
    gcMap *obj = (gcMap *)rec->fn(base, 0x430, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcMap(parent, 0);
        result = obj;
    }
    return (cBase *)result;
}

void gcMap::PostUpdate(void) {
    HandleCinematicSkip();
}

void gcMap::LoadObjectBackgroundBegin(cHandleT<gcEntity> handle) {
    if (!(mFlags & 0x40000)) {
        mLoadBackgroundHandle = handle;
        *(int *)0x0037D7F8 = 0;
        mFlags |= 0x40000;
    }
}

void gcMap::UnloadAllRegions(void) {
    for (int i = 0; i < 2; i++) {
        DeleteRegion(i);
    }
}

void gcMap::DeleteDynamicLoadedObjects(int arg0, cHandleT<gcEntity> handle) {
    if (!(mFlags & 0x100000) && !(mFlags & 0x80000)) {
        mFlags |= 0x80000;
        mDeleteDynamicHandle = handle;
        *(int *)0x0037D7F4 = arg0;
    }
}

void gcMap::ResetRegionStates(void) {
    int count;
    if (mObjectLoadArray != 0) {
        count = mObjectLoadArray[-1] & 0x3FFFFFFF;
    } else {
        count = 0;
    }
    for (int i = 0; i < count; i++) {
        mObjectLoadArray[i] = -1;
    }
    mLoadedCount = 0;
}
