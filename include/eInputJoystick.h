#ifndef EINPUTJOYSTICK_H
#define EINPUTJOYSTICK_H

class eInputJoystick {
public:
    enum eAnalog {};
    enum eAxis {};
    enum eAxisElement {};
    enum eButton {};

    static void PlatformCapture(int);
    static void BindReset(int);
    static void BindAnalog(int, eAnalog, eAxis, eAxisElement);
    static void BindButton(int, eButton, bool);
    static int GetButtonBinding(int, eButton);
    static void SetPointerParam(int, float, float);
    static void SetAccelParam(int, float, float);
    static void ZeroMainAccelPosVel(int);
};

#endif
