#include "gcUIEditBox.h"
#include "gcGameCore.h"

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct gcUIEditBoxAllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char gcUIEditBoxvirtualtable[];

gcUIEditBox::~gcUIEditBox() {
    *(void **)((char *)this + 4) = gcUIEditBoxvirtualtable;
}

cBase *gcUIEditBox::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    gcUIEditBoxAllocRec *rec = (gcUIEditBoxAllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIEditBox *result = 0;
    gcUIEditBox *obj = (gcUIEditBox *)rec->fn(base, 0x124, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUIEditBox(parent);
        result = obj;
    }
    return (cBase *)result;
}

int gcUIEditBox::GetCursor(cHandleT<eSprite> *handle) const {
    int visible = ((mEditFlags & 0x80000000) != 0) & 0xFF;
    if (visible) {
        *handle = mCursorSprite;
        return mCursorPos;
    }
    handle->mIndex = 0;
    return -1;
}

void gcUIEditBox::DeltaCursorPos(int delta) {
    SetCursorPos(mCursorPos + delta);
}

void gcUIEditBox::Backspace(void) {
    int pos = mCursorPos;
    if (pos > 0) {
        DeleteString(1, pos - 1);
    }
}

void gcUIEditBox::UpdateUI(cTimeValue time, const gcEventStackData &evt) {
    ((gcUIWidget *)this)->UpdateUI(time, evt);
    HandleBlink(time);
}

void gcUIEditBox::Focus(bool focus, bool effect) {
    ((gcUIWidget *)this)->Focus(focus, effect);
    ((gcUIWidget *)this)->CaptureFocus(focus);
}
