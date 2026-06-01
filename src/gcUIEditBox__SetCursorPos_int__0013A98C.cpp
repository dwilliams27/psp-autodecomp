// gcUIEditBox::SetCursorPos(int) @ 0x0013a98c
// Split-TU: declare the class locally so we don't touch the shared gcUIEditBox.cpp.

struct cTimeValue {
    int v;
};

class gcUIEditBox {
public:
    // layout (relevant offsets only)
    char pad_0[0xD0];
    wchar_t *mString;       // 0xD0
    char pad_D4[0x3C];      // 0xD4 .. 0x10F
    unsigned int mEditFlags; // 0x110
    char pad_114[0x4];      // 0x114
    cTimeValue mUnk118;     // 0x118
    int mCursorPos;         // 0x11C
    cTimeValue mUnk120;     // 0x120

    void SetCursorPos(int);
    void SetSizeDirty(void);
};

extern int cStrLength(const wchar_t *);

void gcUIEditBox::SetCursorPos(int pos) {
    int len = 0;
    if (mString != 0) {
        len = cStrLength(mString);
    }

    int newPos = 0;
    if (pos > 0) {
        newPos = (pos >= len) ? len : pos;
    }
    pos = newPos;

    if (pos != mCursorPos) {
        mCursorPos = pos;
        mEditFlags |= 0x80000000;
        SetSizeDirty();
        mUnk120 = mUnk118;  // cTimeValue copy
    }
}
