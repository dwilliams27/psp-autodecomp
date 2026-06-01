// gcUIEditBox::Update(cTimeValue, const gcEventStackData &) @ 0x0013AA40
// Split-TU: declare the classes locally so we don't touch the shared gcUIEditBox.cpp.

struct cTimeValue {
    int v;
};

struct gcEventStackData;

class gcUIWidget {
public:
    void Update(cTimeValue, const gcEventStackData &);
};

class gcUIDialog {
public:
    int CheckNextNeighbor(void);
    void FocusNextControl(int, gcUIWidget *);
};

class gcUIEditBox : public gcUIWidget {
public:
    // layout (relevant offsets only)
    char pad_0[0x20];
    gcUIDialog *mDialog;        // 0x20
    unsigned int mFlags;        // 0x24
    char pad_28[0xF4];          // 0x28 .. 0x11B
    int mCursorPos;             // 0x11C

    void Update(cTimeValue, const gcEventStackData &);
    void SetCursorPos(int);
    void HandleKeyboard(void);
    bool HasEditFocus(void) const;
};

inline bool gcUIEditBox::HasEditFocus(void) const {
    return (mFlags & 4) != 0;
}

void gcUIEditBox::Update(cTimeValue t, const gcEventStackData &e) {
    gcUIWidget::Update(t, e);

    if (HasEditFocus()) {
        int n = mDialog->CheckNextNeighbor();
        if (n >= 0) {
            switch (n) {
            case 0:
                SetCursorPos(mCursorPos - 1);
                break;
            case 1:
                SetCursorPos(mCursorPos + 1);
                break;
            case 2:
                mDialog->FocusNextControl(n, 0);
                break;
            case 3:
                mDialog->FocusNextControl(n, 0);
                break;
            default:
                break;
            }
        }
        HandleKeyboard();
    }
}
