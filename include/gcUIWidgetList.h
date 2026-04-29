#ifndef GCUIWIDGETLIST_H
#define GCUIWIDGETLIST_H

#include "gcUIWidget.h"

class cBase;
class cFile;
class cMemPool;
class cTimeValue;
class gcEventStackData;

struct gcUICell {
    int row;
    int col;
};

class gcUIWidgetList {
public:
    gcUIWidgetList(cBase *);
    void FillCell(gcUICell, int);
    void PlayEffect(gcUIWidget::gcUIEffect);
    int IsUpdateEmpty(bool, bool) const;
    void Focus(bool, bool);
    void UpdateUI(cTimeValue, const gcEventStackData &);
    void FocusCurCell(bool);
    void FillCurVisibleItems(void);
    void Write(cFile &) const;
    static gcUIWidgetList *New(cMemPool *, cBase *);
};

#endif
