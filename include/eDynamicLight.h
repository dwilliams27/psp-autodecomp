#ifndef EDYNAMICLIGHT_H
#define EDYNAMICLIGHT_H

#include "eDynamicGeom.h"

class cBase;
class cFile;
class cMemPool;
class cType;
class eDrawInfo;

class eDynamicLight : public eDynamicGeom {
public:
    int Read(cFile &, cMemPool *);
    void Draw(const eDrawInfo &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

#endif
