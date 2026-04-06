#include "eWorld.h"

void eWorld::LockWorld(bool lock) const {
    if (lock) {
        lockCount++;
    } else {
        lockCount--;
    }
}
