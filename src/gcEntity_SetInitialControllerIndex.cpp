#include "gcEntity.h"

void gcEntity::SetInitialControllerIndex(int index) {
    *(signed char *)((char *)this + 0x60) = (signed char)(index - 1);
}
