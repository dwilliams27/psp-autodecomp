#ifndef EINPUTMOUSE_H
#define EINPUTMOUSE_H

struct mVec2;
class eCamera;

class eInputMouse {
public:
    struct eInputState {
        void Reset();
    };

    static bool PlatformInitialize();
    static void PlatformPreUpdate();
    static void PlatformUpdate();
    static int PlatformPresent();
    static void RecheckPresent();
    static void UpdateDrag(const mVec2 &drag);
    static void EndDrag();
    static void Reset();
    static void Draw(const eCamera &cam);
    static void DrawCursor(const eCamera &cam);
    static void DrawDragBox(const eCamera &cam);
};

#endif
