#include "gcUIWidget.h"

void *gcUIWidget::GetResizableSprites(void) {
    return (void *)(*(int *)((char *)this + 0x20) + 0x80);
}
