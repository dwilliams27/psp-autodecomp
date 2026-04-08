#include "gcMap.h"

void *gcMap::GetContainedFactories(int *count) {
    *count = 2;
    return mLoadedRegions;
}
