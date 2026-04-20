#pragma once

#include "eDynamicGeom.h"

class cFile;
class cMemPool;

class cTimeValue {
public:
    int mTime;
};

class eShadow : public eDynamicGeom {
public:
    eShadow(cBase *);
    ~eShadow();

    void Write(cFile &) const;
    void Reset(cMemPool *, bool);
    void Update(cTimeValue);
};
