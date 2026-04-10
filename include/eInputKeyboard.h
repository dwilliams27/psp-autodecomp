#ifndef EINPUTKEYBOARD_H
#define EINPUTKEYBOARD_H

#include "eInput.h"

class cTimeValue;

class eInputKeyboard {
public:
    struct eInputState {
        eInput::eButtonState buttons[107];
        char asciiKey;
        unsigned char present;
        unsigned char changed;
        char pad;
        int timeAccum;

        void Reset();
    };

    enum eKey {};

    static bool Initialize();
    static void Reset();
    static void RecheckPresent();
    static void Update(cTimeValue dt, bool reset, bool accumTime);
    static char KeyToASCII(eKey key);
    static bool PlatformInitialize();
    static void PlatformPreUpdate();
    static void PlatformUpdate();
    static bool PlatformPresent();
};

#endif
