#include "gcUIWidget.h"

bool gcUIWidget::NeedsEffectUpdate(void) const {
    return (mFlags & 0x08000000) != 0;
}
