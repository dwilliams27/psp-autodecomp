#include "gcGame.h"

extern gcGame *gpGame;

void gcNetGame::ResetAllNetworkConnections(void) {
    int i = 0;
    do {
        gpGame->ResetNetConnection(i);
        i++;
    } while (i < 1);
}
