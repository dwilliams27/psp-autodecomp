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
template <class T> class cGUIDT;
class gcMap;

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
    int GetMapIndex(const cGUIDT<gcMap> &) const;
    static void OnMovieDraw(void *);
    static cBase *New(cMemPool *, cBase *);
};

class gcNetGame {
public:
    static int StartServer(void);
    static void Update(cTimeValue);
    static void CheckSynchronization(void);
    static void SetMaxConnections(int);
    static int GetMaxConnections(void);
    static void ResetAllNetworkConnections(void);
    static void SetAllSendMaskOnOff(unsigned int, unsigned int);
    static void SendAllBufferedMessages(void);
    static int IsClientConnected(void);
    static int IsServerReady(void);
    static int GetNumActiveConnections(unsigned int);
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
