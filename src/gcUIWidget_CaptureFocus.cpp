#include "gcUIWidget.h"

void gcUIWidget::CaptureFocus(bool capture) {
    unsigned int flags = mFlags;
    int hasFocus = ((flags & 0x10) != 0) & 0xFF;
    if (capture != hasFocus) {
        if (!capture || (((flags & 0x4) != 0) & 0xFF)) {
            if ((capture & 0xFF) & 0xFF) {
                mFlags |= 0x10;
            } else {
                mFlags &= ~0x10;
            }
        }
    }
}
