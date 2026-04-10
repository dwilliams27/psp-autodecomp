#include "eInputMouse.h"
#include "mVec2.h"

extern char D_00045338[];
extern char D_000453F0[];
extern char D_0037C120;

void eInputMouse::PlatformUpdate() {
}

void eInputMouse::RecheckPresent() {
    *(char *)(D_00045338 + 0x7C) = 1;
}

struct eInputMouseData {
    char pad[0xA8];
    float dragX;
    float dragY;
};

void eInputMouse::UpdateDrag(const mVec2 &drag) {
    eInputMouseData *data = (eInputMouseData *)D_00045338;
    data->dragX = drag.x;
    data->dragY = drag.y;
}

struct eInputMouseData2 {
    char pad1[0x7F];
    char field7F;
    char pad2[0x34];
    int fieldB4;
};

void eInputMouse::EndDrag() {
    eInputMouseData2 *data = (eInputMouseData2 *)D_00045338;
    data->field7F = 0;
    data->fieldB4 = 0;
    D_0037C120 = 0;
}

void eInputMouse::Reset() {
    ((eInputState *)D_00045338)->Reset();
    ((eInputState *)D_000453F0)->Reset();
}

void eInputMouse::Draw(const eCamera &cam) {
    DrawCursor(cam);
    DrawDragBox(cam);
}
