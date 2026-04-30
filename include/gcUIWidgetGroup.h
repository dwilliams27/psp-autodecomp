#ifndef GCUIWIDGETGROUP_H
#define GCUIWIDGETGROUP_H

#include "gcUIWidget.h"
#include "gcUIWidgetList.h"
#include "mVec2.h"

class cBase;
class cMemPool;
class cType;

class gcUIWidgetGroup : public gcUIWidget {
public:
    gcUIWidgetGroup(cBase *);
    static cBase *New(cMemPool *pool, cBase *parent);
    gcUIWidgetGroup &operator=(const gcUIWidgetGroup &);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    int NeedsEffectUpdate(void) const;
    void *GetResizableSprites(void);
    void Reset(cMemPool *pool, bool flag);
    void SetDimensions(int, int);
    void DeleteAllWidgets(bool);
    void InsertInDrawOrder(int, gcUIWidget *, gcUIWidget *);
    mVec2 GetCellPos(gcUICell) const;
    float GetCellPos(gcUICell, int) const;
};

#endif
