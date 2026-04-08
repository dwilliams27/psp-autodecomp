#ifndef GCUIWIDGETLIST_H
#define GCUIWIDGETLIST_H

#include "gcUIWidget.h"

struct gcUICell {
    int row;
    int col;
};

class gcUIWidgetList {
public:
    void FillCell(gcUICell, int);
    void PlayEffect(gcUIWidget::gcUIEffect);
};

#endif
