#include "eInputJoystick.h"

void eInputJoystick::PlatformPreUpdate() {
}
#include "eInputJoystick.h"

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
