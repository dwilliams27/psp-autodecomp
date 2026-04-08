#ifndef GCCINEMATIC_H
#define GCCINEMATIC_H

class cMemPool;
class cInStream;

class gcCinematic {
public:
    void Reset(cMemPool *, bool);
};

class gcCinematicInstance {
public:
    void Reset(cMemPool *, bool);
    static void UpdateProfile(void);
};

class gcReplicationVisitor {
public:
    void *mMemCardInStream;   // offset 0x00
    int mPad04;               // offset 0x04
    cInStream *mMemCardStream; // offset 0x08
    int mPad0C;               // offset 0x0C
    int mNetConnection;       // offset 0x10
    int mReadActive;          // offset 0x14

    void SetNetConnection(int);
    void SetMemCardStream(cInStream *);
    void EndRead(void);
};

#endif
