#include "gcUIDialog.h"
#include "gcUI.h"
#include "gcUIWidgetGroup.h"

void gcUIDialog::CalcInstanceSize(void) {
}

void gcUIDialog::OnFinalOpen(void) {
    mFlags = mFlags & ~0x10;
}

void gcUIDialog::ClearEvents(void) {
    mEvent0 = 0;
    mEvent1 = 0;
    mEvent2 = 0;
    mEvent3 = 0;
    mEvent4 = 0;
}

void gcUIDialog::Reset(cMemPool *pool, bool flag) {
    ((gcUIWidgetGroup *)((char *)this + 0x98))->Reset(pool, true);
}

void gcUIDialog::Focus(void) {
    if (mpUI) {
        mpUI->ActualOpenDialog(this);
    }
}

void gcUIDialog::Close(void) {
    if (mpUI) {
        mpUI->CloseDialog(this, 0, 0);
    }
}
