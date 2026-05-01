#ifndef EINPUTJOYSTICK_H
#define EINPUTJOYSTICK_H

class eCamera;

class eInputJoystick {
public:
    struct eInputState {
        void Reset();
    };

    enum eAnalog {};
    enum eAxis {};
    enum eAxisElement {};
    enum eButton {};

    static bool Initialize();
    static void Reset();
    static int GetIdleTime(int);
    static void ResetIdleTime(int);
    static int GetFirstPresentController();
    static void RecheckPresent();
    static void Draw(const eCamera &cam);
    static void DrawPointer(int idx, const eCamera &cam);
    static bool PlatformInitialize();
    static void PlatformUpdateRumble(int);
    static void PlatformUpdateIdle(int, bool);
    static void PlatformPreUpdate();
    static void PlatformCapture(int);
    static bool PlatformControllerPresent(int);
    static bool PlatformControllerBatteryLow(int);
    static void BindReset(int);
    static void BindAnalog(int, eAnalog, eAxis, eAxisElement);
    static void BindButton(int, eButton, bool);
    static int GetButtonBinding(int, eButton);
    static void SetPointerParam(int, float, float);
    static void SetAccelParam(int, float, float);
    static void ZeroMainAccelPosVel(int);
    static void ZeroExtAccelPosVel(int);
    static void EnablePointer(int, bool);
    static void EnableAimingMode(int, bool);
    static void StopRumble(int);
    static float GetControllerExtension(int);
};

#endif
