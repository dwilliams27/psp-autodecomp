#ifndef EINPUTJOYSTICK_H
#define EINPUTJOYSTICK_H

class eInputJoystick {
public:
    enum eAnalog {};
    enum eAxis {};
    enum eAxisElement {};

    static void PlatformCapture(int);
    static void BindReset(int);
    static void BindAnalog(int, eAnalog, eAxis, eAxisElement);
};

#endif
