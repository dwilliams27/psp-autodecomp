#ifndef GCENTITY_H
#define GCENTITY_H

#include "mVec3.h"

class cBase;
class cMemPool;
class gcPlayer;

class gcEntity {
public:
    gcEntity(cBase *);
    static cBase *New(cMemPool *, cBase *);
    static void OnFullscreenCinematicStarting(void);
    gcPlayer *GetPlayer(void) const;
    void SetInitialControllerIndex(int);
    void SetVelocity(const mVec3 &);
    void *GetSubGeomController(int);
    void GetVelocity(mVec3 *) const;
    void SetPreviousPrimaryController(bool);
    void SetPrimaryController(int, bool);
    float GetSoundFrequencyOffset(int) const;
};

#endif
