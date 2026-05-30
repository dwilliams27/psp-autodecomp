#include "gcEntity.h"

void *gcEntity::GetSubGeomController(int index) {
    if (index < 0) {
        return 0;
    }
    int count = 0;
    void **arr = *(void ***)((char *)this + 0x1F0);
    if (arr != 0) {
        count = *((int *)arr - 1);
    }
    if (index < count) {
        return arr[index];
    }
    return 0;
}
