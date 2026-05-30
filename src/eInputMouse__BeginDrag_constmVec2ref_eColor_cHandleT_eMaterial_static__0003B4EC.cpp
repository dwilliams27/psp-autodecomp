// eInputMouse::BeginDrag(const mVec2 &, eColor, cHandleT<eMaterial>) static
// Address: 0x0003b4ec, Size: 68B
// Emits: __0fLeInputMouseJBeginDragRC6FmVec26GeColor6IcHandleT76JeMaterial_T

#include "eInputMouse.h"

// Local wrapper types matching the original signature shapes.
struct mVec2 {
    float x;
    float y;
};

class eColor {
public:
    int value;
};

class eMaterial;

template <class T>
class cHandleT {
public:
    int handle;
};

// Drag state singleton at D_00045338.
struct eMouseDragState {
    char pad7F[0x7F];   // 0x00 .. 0x7E
    char active;        // 0x7F
    char padA0[0xA0 - 0x80];
    float startX;       // 0xA0
    float startY;       // 0xA4
    float endX;         // 0xA8
    float endY;         // 0xAC
    int color;          // 0xB0
    int material;       // 0xB4
};

extern eMouseDragState D_00045338;
extern char D_0037C120;

void eInputMouse::BeginDrag(const mVec2 &pos, eColor color, cHandleT<eMaterial> material)
{
    D_00045338.active = 1;
    D_00045338.color = color.value;
    D_00045338.material = material.handle;
    D_00045338.startX = pos.x;
    D_00045338.startY = pos.y;
    D_00045338.endX = pos.x;
    D_00045338.endY = pos.y;
    D_0037C120 = 0;
}
