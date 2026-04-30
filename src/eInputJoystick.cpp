#include "eInputJoystick.h"

extern "C" int sceCtrlSetSamplingMode(unsigned int uiMode);
extern "C" int sceCtrlSetSamplingCycle(unsigned int uiCycle);
extern "C" int sceCtrlSetIdleCancelThreshold(int iUnHoldThreshold, int iHoldThreshold);

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
