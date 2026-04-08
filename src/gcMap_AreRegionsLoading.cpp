#include "gcMap.h"

bool gcMap::AreRegionsLoading(void) const {
    return *(int *)((char *)this + 0x3A0) > 0;
}
