// gcDoMouseOp::Evaluate(void) const
// Address: 0x002e7bd8, Size: 256B
// Emits: __0fLgcDoMouseOpIEvaluatevK

#include "eInputMouse.h"

struct mVec2 {
    float x;
    float y;
};

struct eColor {
    int value;
};

class eMaterial;

template <class T>
struct cHandleT {
    int handle;
};

// gcDoMouseOp drag-state singleton at D_00045338.
struct gcMouseState {
    char pad94[0x94];   // 0x00 .. 0x93
    int cursorX;        // 0x94
    int cursorY;        // 0x98
    int result;         // 0x9C
};

extern gcMouseState D_00045338;

// Sprite object accessor (gcDesiredObjectT<gcDesiredSprite, ...>::Get).
struct eSpriteObj {
    char pad30[0x30];
    int id;             // 0x30
};

class gcDesiredSpriteHelper {
public:
    eSpriteObj *Get(bool create) const;
};

extern void *D_0037D854;

// gcDoMouseOp action expression node.
class gcDoMouseOp {
public:
    char base[0x0C];
    int mMode;                          // 0x0C
    eColor mColor;                      // 0x10
    cHandleT<eMaterial> mHandle;        // 0x14
    gcDesiredSpriteHelper mSprite;      // 0x18

    float Evaluate(void) const;
};

float gcDoMouseOp::Evaluate(void) const {
    mVec2 pos;
    pos.x = (float)D_00045338.cursorX;
    pos.y = (float)D_00045338.cursorY;

    switch (mMode) {
    case 0:
        eInputMouse::BeginDrag(pos, mColor, mHandle);
        break;
    case 1:
        eInputMouse::UpdateDrag(pos);
        break;
    case 2:
        eInputMouse::EndDrag();
        break;
    case 3: {
        eSpriteObj *obj = mSprite.Get(false);
        int val = obj ? obj->id : 0;
        // Load-bearing scaffolding: these volatile/dead writes force the exact
        // SNC stack-slot + register coloring of the original. Do NOT "clean up"
        // — removing them breaks the byte-exact match.
        volatile int m0;
        m0 = val;
        int t = m0;
        int m[2];
        m[0] = t;
        m[1] = t;
        D_00045338.result = t;
        break;
    }
    case 4: {
        int *p = (int *)((char *)D_0037D854 + 0x4B0);
        int val = *p;
        int n[2];
        n[0] = val;
        n[1] = val;
        D_00045338.result = val;
        break;
    }
    }
    return 0.0f;
}
