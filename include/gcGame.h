#ifndef GCGAME_H
#define GCGAME_H

#include "gcGameCore.h"

typedef int nwSocketHandle;

class nwConnectionHandle {
public:
    int mHandle;
};

class cBase;
class cMemPool;
class cType;

class gcGame {
public:
    gcGame(cBase *);
    gcGame &operator=(const gcGame &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void NetReplicate(bool);
    void ResetNetConnection(int);
    void *GetGroups(int *);
    void ResetLoadingCinematicPools(void);
    void DeleteLoadingScreen(void);
    void Reset(cMemPool *, bool);
    void DeleteMap(void);
    static void OnMovieDraw(void *);
    static cBase *New(cMemPool *, cBase *);
};

class gcNetGame {
public:
    static int StartServer(void);
    static void CheckSynchronization(void);
    static void SetMaxConnections(int);
    static int GetMaxConnections(void);
    static void ResetAllNetworkConnections(void);
    static void SendAllBufferedMessages(void);
    static int IsClientConnected(void);
    static int IsServerReady(void);
};

class gcEntityController {
public:
    void PostPhysicsMove(cTimeValue);
};

class gcPlayer {
public:
    static void HandleClientWarp(void);
};

#endif
