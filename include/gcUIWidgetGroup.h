#ifndef GCUIWIDGETGROUP_H
#define GCUIWIDGETGROUP_H

class cBase;
class cMemPool;
class cType;

class gcUIWidgetGroup {
public:
    static cBase *New(cMemPool *pool, cBase *parent);
    gcUIWidgetGroup &operator=(const gcUIWidgetGroup &);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    int NeedsEffectUpdate(void) const;
    void *GetResizableSprites(void);
    void Reset(cMemPool *pool, bool flag);
};

#endif
