#include "gcMap.h"

void gcMap::LoadObjectBackgroundEnd(void) {
    int flags = mFlags;
    if ((flags & 0x40000) && (*(int *)0x0037D7F8 == 0)) {
        mFlags = flags & ~0x40000;
    }
}
