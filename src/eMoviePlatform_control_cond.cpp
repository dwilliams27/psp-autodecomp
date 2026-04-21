#include "thread.h"

class eMoviePlatform {
public:
    char _pad0[0x2B0];
    int control_eventFlag;       // 0x2B0
    char _pad1[0x14];
    int dispbuf_eventFlag;       // 0x2C8

    int control_getCondition(void);
    void control_waitConditionDispStart(void);
    void control_waitConditionSoundStart(void);
    int checkPlayerErrCondition(void);
    int dispbuf_checkDelayMode(void);
};

int eMoviePlatform::control_getCondition(void) {
    SceKernelEventFlagInfo info;
    info.size = sizeof(info);
    sceKernelReferEventFlagStatus(control_eventFlag, &info);
    return info.currentPattern;
}

void eMoviePlatform::control_waitConditionDispStart(void) {
    sceKernelWaitEventFlag(control_eventFlag, 1, 0, 0, 0);
}

void eMoviePlatform::control_waitConditionSoundStart(void) {
    sceKernelWaitEventFlag(control_eventFlag, 2, 0, 0, 0);
}

int eMoviePlatform::checkPlayerErrCondition(void) {
    int cond = control_getCondition();
    __asm__ volatile("" ::: "memory");
    int r = 0;
    if (cond & 0x800) r = -1;
    return r;
}

int eMoviePlatform::dispbuf_checkDelayMode(void) {
    SceKernelEventFlagInfo info;
    info.size = sizeof(info);
    sceKernelReferEventFlagStatus(dispbuf_eventFlag, &info);
    return info.currentPattern == 1;
}
