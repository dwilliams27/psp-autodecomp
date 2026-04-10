#ifndef GCREPLICATIONVISITOR_H
#define GCREPLICATIONVISITOR_H

#include "gcGame.h"

class cOutStream;
class cInStream;
class nwConnection;
class gcReplicationGroup;

class nwSocket {
public:
    static nwConnection *GetConnection(nwConnectionHandle);
};

class gcReplicationVisitor {
public:
    int mMode;                // offset 0x00
    cOutStream *mOutStream;   // offset 0x04
    cInStream *mInStream;     // offset 0x08
    int mStreamPos;           // offset 0x0C
    int mNetConnection;       // offset 0x10
    int mReadActive;          // offset 0x14
    bool mWriteEnabled;       // offset 0x18
    bool mByteSwap;           // offset 0x19

    void SetNetConnection(int);
    void SetMemCardStream(cOutStream *);
    void SetMemCardStream(cInStream *);
    void SetNetStream(cOutStream *, nwConnectionHandle, bool, bool);
    void SetNetStream(cInStream *, nwConnectionHandle, bool, bool);
    void EndRead(void);
    int BeginRead(gcReplicationGroup *);
    cOutStream *BeginWrite(gcReplicationGroup *, int);
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
