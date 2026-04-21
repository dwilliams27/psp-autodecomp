#include "thread.h"

class eMoviePlatform {
public:
    char _pad0[0x2B4];
    int vblank_eventFlag;  // 0x2B4

    static void control_vblankHandler(int, void *);
};

void eMoviePlatform::control_vblankHandler(int, void *arg) {
    sceKernelSetEventFlag(((eMoviePlatform *)arg)->vblank_eventFlag, 3);
}
