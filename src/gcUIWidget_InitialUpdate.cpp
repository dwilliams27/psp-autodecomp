#include "gcUIWidget.h"

void gcUIWidget::InitialUpdate(void) {
    int result = IsUpdateEmpty(false, true);
    int masked = result & 0xFF;
    if (masked & 0xFF) {
        mFlags |= 0x80000000;
    } else {
        mFlags &= 0x7FFFFFFF;
    }
}
