#ifndef GCREPLICATIONVISITOR_H
#define GCREPLICATIONVISITOR_H

#include "gcGame.h"

class cOutStream;
class cInStream;

class gcReplicationVisitor {
public:
    void *mMemCardInStream;   // offset 0x00
    int mPad04;               // offset 0x04
    cInStream *mMemCardStream; // offset 0x08
    int mPad0C;               // offset 0x0C
    int mNetConnection;       // offset 0x10
    int mReadActive;          // offset 0x14

    void SetNetConnection(int);
    void SetMemCardStream(cOutStream *);
    void SetMemCardStream(cInStream *);
    void EndRead(void);
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
