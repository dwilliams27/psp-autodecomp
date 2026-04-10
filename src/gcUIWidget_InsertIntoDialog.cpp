#include "gcUIWidget.h"
#include "gcUIDialog.h"

void gcUIWidget::InsertIntoDialog(gcUIDialog *dialog) {
    mpDialog = dialog;
    mpDialog->CreateControl(this);
}
