#include "eInputJoystick.h"

extern "C" int sceCtrlSetSamplingMode(unsigned int uiMode);
extern "C" int sceCtrlSetSamplingCycle(unsigned int uiCycle);
extern "C" int sceCtrlSetIdleCancelThreshold(int iUnHoldThreshold, int iHoldThreshold);
extern "C" int sceKernelPowerTick(int type);
extern "C" void cCorePowerSave(int enable);

extern "C" int cGetCurrentPlatform(void);

int eInputJoystick::GetFirstPresentController() {
    int controller = 0;
    do {
        if (PlatformControllerPresent(controller)) {
            return controller;
        }
        controller++;
    } while (controller < 4);
    return -1;
}

void eInputJoystick::PlatformPreUpdate() {
}

void eInputJoystick::PlatformUpdateIdle(int idx, bool active) {
    if (idx == 0) {
        int enable = 0;
        if (active) {
            volatile int idle[2];
            idle[1] = GetIdleTime(idx);
            int idleTime = idle[1];
            int rate = *(int *)0x36C7FC;
            int limit = rate * 60;
            if (limit < idleTime) {
                enable = 1;
            }
        }
        cCorePowerSave(enable & 0xFF);
        if (!active) {
            sceKernelPowerTick(6);
        }
    }
}

void eInputJoystick::BindButton(int, eButton, bool) {
}

int eInputJoystick::GetButtonBinding(int, eButton button) {
    return (int)button;
}

void eInputJoystick::SetPointerParam(int, float, float) {
}

void eInputJoystick::SetAccelParam(int, float, float) {
}

void eInputJoystick::ZeroMainAccelPosVel(int) {
}

struct sJoyState {
    char _pad0[0x834];
    float rumble;
    char _pad1[0xC];
};

extern char D_00041118[];

void eInputJoystick::Reset() {
    int i = 0;
    char *state0 = (char *)0x41118;
    char *state1 = (char *)0;
    char *addr1 = (char *)0x43228;
    state0 = state1 + (unsigned int)state0;
    state1 = state1 + (unsigned int)addr1;
    do {
        ((eInputState *)state0)->Reset();
        ((eInputState *)state1)->Reset();
        i++;
        state0 += 0x844;
        state1 += 0x844;
    } while (i < 4);
    StopRumble(-1);
}

void eInputJoystick::RecheckPresent() {
    int i = 0;
    char val = 1;
    char *p = D_00041118;
    do {
        p[0x814] = val;
        i++;
        p += 0x844;
    } while (i < 4);
}

void eInputJoystick::PlatformUpdateRumble(int idx) {
    ((sJoyState *)(idx * 0x844 + D_00041118))->rumble = 0.0f;
}

bool eInputJoystick::PlatformInitialize() {
    sceCtrlSetSamplingMode(1);
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetIdleCancelThreshold(0x40, -1);
    return true;
}

void eInputJoystick::Draw(const eCamera &cam) {
    int i = 0;
    do {
        DrawPointer(i, cam);
        i++;
    } while (i < 4);
}

bool eInputJoystick::Initialize() {
    if (!PlatformInitialize()) {
        return false;
    }
    *(bool *)0x37D1ED = (cGetCurrentPlatform() == 8);
    return true;
}
