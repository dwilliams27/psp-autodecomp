#ifndef GCENTITY_H
#define GCENTITY_H

#include "mVec3.h"

class gcEntity {
public:
    void SetInitialControllerIndex(int);
    void SetVelocity(const mVec3 &);
    void *GetSubGeomController(int);
    void GetVelocity(mVec3 *) const;
    void SetPreviousPrimaryController(bool);
    void SetPrimaryController(int, bool);
};

#endif
