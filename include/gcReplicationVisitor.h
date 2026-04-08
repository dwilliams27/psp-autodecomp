#ifndef GCREPLICATIONVISITOR_H
#define GCREPLICATIONVISITOR_H

#include "gcGame.h"

class cOutStream;

class gcReplicationVisitor {
public:
    void SetMemCardStream(cOutStream *);
};

class gcRegionBase {
public:
    gcRegionBase &operator=(const gcRegionBase &);
};

class gcLoadingScreen {
public:
    gcLoadingScreen &operator=(const gcLoadingScreen &);
};

class gcMap {
public:
    gcMap &operator=(const gcMap &);
};

#endif
