#include "gcEntity.h"

void gcEntity::SetPreviousPrimaryController(bool active) {
    signed char prev = *(signed char *)((char *)this + 0x64);
    if (prev != -1) {
        SetPrimaryController(prev, active);
        *(signed char *)((char *)this + 0x64) = -1;
    }
}
