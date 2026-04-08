#include "eVRAMMgr.h"

void eVRAMMgr::InvalidateAll(void) {
    *(int *)0x37D33C = 0;
}
