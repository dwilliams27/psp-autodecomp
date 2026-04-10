#ifndef GCGAME_H
#define GCGAME_H

#include "gcGameCore.h"

typedef int nwSocketHandle;

class nwConnectionHandle {
public:
    int mHandle;
};

class gcGame {
public:
    gcGame &operator=(const gcGame &);
    void NetReplicate(bool);
    void ResetNetConnection(int);
    void *GetGroups(int *);
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
