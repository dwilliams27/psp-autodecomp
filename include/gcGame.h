#ifndef GCGAME_H
#define GCGAME_H

#include "gcGameCore.h"

class gcGame {
public:
    gcGame &operator=(const gcGame &);
    void NetReplicate(bool);
};

class gcNetGame {
public:
    static int StartServer(void);
    static void CheckSynchronization(void);
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
