#include "gcGame.h"

void *gcGame::GetGroups(int *count) {
    *count = 0x1C;
    return (char *)this + 0x4D0;
}
