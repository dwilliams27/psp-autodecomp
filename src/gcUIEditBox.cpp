#include "gcUIEditBox.h"

int gcUIEditBox::GetCursor(cHandleT<eSprite> *out) const {
    if (((mEditFlags & 0x80000000u) != 0) & 0xFF) {
        *out = mCursorSprite;
        return mCursorPos;
    }
    out->mIndex = 0;
    return -1;
}

void gcUIEditBox::DeltaCursorPos(int delta) {
    SetCursorPos(mCursorPos + delta);
}

void gcUIEditBox::Backspace(void) {
    if (mCursorPos > 0) {
        DeleteString(1, mCursorPos - 1);
    }
}

void gcUIEditBox::UpdateUI(cTimeValue t, const gcEventStackData &data) {
    gcUIWidget::UpdateUI(t, data);
    HandleBlink(t);
}

void gcUIEditBox::Focus(bool focus, bool b2) {
    gcUIWidget::Focus(focus, b2);
    gcUIWidget::CaptureFocus(focus);
}
