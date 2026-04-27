#include "eInputMouse.h"
#include "mVec2.h"

class eColor {
public:
    unsigned int mColor;
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

class eMaterial;

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

// ---- eInputMouse::BeginDrag(const mVec2 &, eColor, cHandleT<eMaterial>) static ----
// Address: 0x0003b4ec, Size: 68B
struct eInputMouseBeginDragData {
    char _pad0[0x7F];
    char active;       // +0x7F
    char _pad1[0x20];
    float startX;      // +0xA0
    float startY;      // +0xA4
    float endX;        // +0xA8
    float endY;        // +0xAC
    int color;         // +0xB0
    int material;      // +0xB4
};

class eRect {
public:
    float x;
    float y;
    float w;
    float h;
};

extern "C" float fabsf(float);

void eInputMouse::GetDragRect(eRect *rect) {
    eInputMouseBeginDragData *data = (eInputMouseBeginDragData *)D_00045338;
    float minX;
    if (data->startX <= data->endX) {
        minX = data->startX;
    } else {
        minX = data->endX;
    }
    rect->x = minX;
    float minY = data->startY;
    if (!(data->startY <= data->endY)) {
        minY = data->endY;
    } else {
        minY = data->startY;
    }
    rect->y = minY;
    rect->w = fabsf(data->endX - data->startX);
    rect->h = fabsf(data->endY - data->startY);
}

int eInputMouse::IsDragEmpty() {
    eRect r;
    GetDragRect(&r);
    int result = 0;
    if (r.w <= 1.0f) {
        if (r.h <= 1.0f) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

void eInputMouse::BeginDrag(const mVec2 &pos, eColor color, cHandleT<eMaterial> material) {
    eInputMouseBeginDragData *data = (eInputMouseBeginDragData *)D_00045338;
    data->active = 1;
    data->color = color.mColor;
    data->material = material.mIndex;
    data->startX = pos.x;
    data->startY = pos.y;
    data->endX = pos.x;
    data->endY = pos.y;
    D_0037C120 = 0;
}
