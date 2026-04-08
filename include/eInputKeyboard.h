#ifndef EINPUTKEYBOARD_H
#define EINPUTKEYBOARD_H

class eInputKeyboard {
public:
    static bool PlatformInitialize();
    static void PlatformPreUpdate();
    static void PlatformUpdate();
};

#endif
