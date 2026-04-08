#include "gcMap.h"

gcRegion *gcMap::GetLoadedRegion(int index) const {
    return mLoadedRegions[index];
}
