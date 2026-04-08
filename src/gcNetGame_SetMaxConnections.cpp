#include "gcGame.h"

void gcNetGame::SetMaxConnections(int n) {
    *(int *)0x37D860 = n;
}
