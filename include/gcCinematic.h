#ifndef GCCINEMATIC_H
#define GCCINEMATIC_H

#include "gcReplicationVisitor.h"

class cMemPool;

class gcCinematic {
public:
    void Reset(cMemPool *, bool);
};

class gcCinematicInstance {
public:
    void Reset(cMemPool *, bool);
    static void UpdateProfile(void);
};

#endif
