#include "gcUIWidget.h"

void gcUIWidget::InitialUpdateUI(void) {
    int result = IsUpdateEmpty(true, true);
    int masked = result & 0xFF;
    if (masked & 0xFF) {
        mFlags |= 0x40000000;
    } else {
        mFlags &= 0xBFFFFFFF;
    }
}
