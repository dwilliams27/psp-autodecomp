#include "gcUIEditBox.h"
#include "gcGameCore.h"

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
    gcUIWidget::UpdateUI(time, evt);
    HandleBlink(time);
}

void gcUIEditBox::Focus(bool focus, bool effect) {
    gcUIWidget::Focus(focus, effect);
    gcUIWidget::CaptureFocus(focus);
}
