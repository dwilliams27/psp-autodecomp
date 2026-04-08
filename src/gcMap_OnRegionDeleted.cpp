#include "gcMap.h"

void gcMap::OnRegionDeleted(gcRegion *region) {
    for (int i = 0; i < 2; i++) {
        if (mLoadedRegions[i] == region) {
            mLoadedRegions[i] = 0;
        }
    }
}
