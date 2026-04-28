#ifndef GCUIWIDGETGROUP_H
#define GCUIWIDGETGROUP_H

class cBase;
class cMemPool;

class gcUIWidgetGroup {
public:
    static cBase *New(cMemPool *pool, cBase *parent);
    int NeedsEffectUpdate(void) const;
    void *GetResizableSprites(void);
    void Reset(cMemPool *pool, bool flag);
};

#endif
