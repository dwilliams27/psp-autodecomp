#include "gcUIEditBox.h"
#include "gcGameCore.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcUIControl {
public:
    static cBase *New(cMemPool *, cBase *);
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct gcUIEditBoxAllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char gcUIEditBoxvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_0009F40C;
extern cType *D_0009F410;
extern cType *D_0009F588;

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

const cType *gcUIEditBox::GetType(void) const {
    if (D_0009F588 == 0) {
        if (D_0009F410 == 0) {
            if (D_0009F40C == 0) {
                if (D_0009990C == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    D_0009990C = cType::InitializeType(
                        0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
                }
                D_0009F40C = cType::InitializeType(
                    0, 0, 0x201, D_0009990C, gcUIControl::New, 0, 0, 0);
            }
            D_0009F410 = cType::InitializeType(
                0, 0, 0x200, D_0009F40C, gcUITextControl::New, 0, 0, 0);
        }
        D_0009F588 = cType::InitializeType(
            0, 0, 0x173, D_0009F410, gcUIEditBox::New, 0, 0, 0);
    }
    return D_0009F588;
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
