#include "gcMap.h"

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
