#include "gcMap.h"

void *gcMap::GetGroups(int *count) {
    *count = 42;
    return mGroups;
}
