#include "thread.h"

class eMoviePlatform {
public:
    char _pad0[0x2B4];
    int control_eventFlag;       // 0x2B4
    char _pad1[0x10];            // 0x2B8 - 0x2C8
    int dispbuf_eventFlag;       // 0x2C8

    void dispbuf_setPlayMode(void);
    int checkDecodeEnd(void);
    int read_isEnd(void);
    void control_waitDisp(void);
    void control_waitDecode(void);
    int control_getCondition(void);
};

void eMoviePlatform::dispbuf_setPlayMode(void) {
    sceKernelClearEventFlag(dispbuf_eventFlag, 0);
}

int eMoviePlatform::checkDecodeEnd(void) {
    return (control_getCondition() & 8) != 0;
}

int eMoviePlatform::read_isEnd(void) {
    return (control_getCondition() & 4) != 0;
}

void eMoviePlatform::control_waitDisp(void) {
    sceKernelWaitEventFlag(control_eventFlag, 1, 0x21, 0, 0);
}

void eMoviePlatform::control_waitDecode(void) {
    sceKernelWaitEventFlag(control_eventFlag, 2, 0x21, 0, 0);
}
