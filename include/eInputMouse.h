#ifndef EINPUTMOUSE_H
#define EINPUTMOUSE_H

class eInputMouse {
public:
    static bool PlatformInitialize();
    static void PlatformPreUpdate();
    static void PlatformUpdate();
    static int PlatformPresent();
};

#endif
