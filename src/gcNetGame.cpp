#include "gcGame.h"

class gcExpressionList {
public:
    float Evaluate(void) const;
};

struct nwConnection {
    char _pad[0x20];
    int status;
    char _pad24[4];
    int flags;
};

struct nwSocketHandleStruct {
    int handle;
};

class nwNetwork {
public:
    static void Update(cTimeValue);
    static void *GetLobby(void);
    static nwSocketHandleStruct GetGameSocket(void);
};

class nwSocket {
public:
    static nwSocket *GetSocket(nwSocketHandleStruct);
    static nwConnection *GetConnection(nwConnectionHandle);
    nwConnectionHandle GetConnection(int) const;
    int GetMaxConnections(void) const;
    void FlushAll(void);
};

struct gcNetGameDispatchEntry {
    short offset;
    short pad;
    int (*fn)(void *);
};

struct gcNetGameFillEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct gcNetGameSendEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *, int);
};

struct gcNetGameScratch {
    int words0[4];
    int zero10;
    int zero14;
    unsigned char flag18;
    char pad19[3];
    int word1C;
    float floats20[10];
    int pad48[4];
    int word58;
    int word5C;
    int word60;
    int word64;
    int word68;
    int word6C;
    int word70;
    char vec74[16];
};

struct gcNetGameEvalRecord {
    void *prev;
    int zero;
    void *list;
    void *scratchBase;
    void *exprData;
};

struct gcNetGameMessage {
    char pad0[0x28];
    int word28;
    int words2C[4];
    int word3C;
    char pad40[0x0C];
    int numConnections;
    int mapIndex;
    int pad54;
};

struct gcNetGameUpdateFrame {
    char bytes[0x100];
};

struct gcNetGameUpdateLayout {
    char pad4[4];
    gcNetGameScratch scratch;
    gcNetGameMessage msg;
    gcNetGameEvalRecord rec;
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void *memset(void *, int, unsigned int);

extern gcGame *gpGame;
extern nwConnectionHandle gClientConnection;
extern nwConnectionHandle D_0009A3C8;
extern void *g_expressionEvalStack;

void gcNetGame::SetMaxConnections(int n) {
    *(int *)0x37D860 = n;
}

int gcNetGame::GetMaxConnections(void) {
    return *(int *)0x37D860;
}

void gcNetGame::ResetAllNetworkConnections(void) {
    int i = 0;
    do {
        gpGame->ResetNetConnection(i);
        i++;
    } while (i < 1);
}

void gcNetGame::Update(cTimeValue t) {
    gcNetGameUpdateLayout u;
    void *lobby;

    nwNetwork::Update(t);
    lobby = nwNetwork::GetLobby();
    if (lobby != 0) {
        gcNetGameDispatchEntry *entry =
            (gcNetGameDispatchEntry *)(*(char **)lobby + 0x218);
        short off = entry->offset;
        int (*fn)(void *) = entry->fn;
        if (fn((char *)lobby + off) != 0) {
            void *exprBase;

            u.scratch.zero10 = 0;
            u.scratch.zero14 = 0;
            u.scratch.flag18 = 1;
            u.scratch.words0[0] = 0;
            u.scratch.words0[1] = 0;
            u.scratch.words0[2] = 0;
            u.scratch.words0[3] = 0;
            u.scratch.word1C = 0;
            u.scratch.floats20[0] = 0.0f;
            u.scratch.word58 = 0;
            exprBase = (char *)gpGame + 0x3E8;
            __vec_new(u.scratch.vec74, 2, 8, (void (*)(void *))0x2275F0);
            u.scratch.floats20[1] = 0.0f;
            u.scratch.floats20[2] = 0.0f;
            u.scratch.floats20[3] = 0.0f;
            u.scratch.floats20[4] = 0.0f;
            u.scratch.floats20[5] = 0.0f;
            u.scratch.floats20[6] = 0.0f;
            u.scratch.floats20[7] = 0.0f;
            u.scratch.floats20[8] = 0.0f;
            u.scratch.floats20[9] = 0.0f;
            u.scratch.word5C = 0;
            u.scratch.word60 = 0;
            u.scratch.word64 = 0;
            u.scratch.word68 = 0;
            u.scratch.word6C = 0;
            u.scratch.word70 = 0;
            if (!((unsigned char)(*(int *)((char *)exprBase + 8) == 0)) &&
                ((unsigned char)(*(int *)0x37D7B0 == 1))) {
                int *exprData;

                u.rec.zero = 0;
                u.rec.prev = g_expressionEvalStack;
                u.rec.list = exprBase;
                u.rec.scratchBase = &u.scratch;
                exprData = &u.scratch.word1C;
                u.rec.exprData = exprData;
                g_expressionEvalStack = &u.rec;
                *exprData = *exprData & -4;
                ((gcExpressionList *)((char *)exprBase + 8))->Evaluate();
                g_expressionEvalStack = u.rec.prev;
            }
        }
    }
    if (*(int *)0x37D858 & 1) {
        nwSocketHandleStruct handle = nwNetwork::GetGameSocket();
        if (nwSocket::GetSocket(handle) != 0) {
            if (lobby != 0) {
                nwSocketHandleStruct handle2 = nwNetwork::GetGameSocket();
                if (*(unsigned char *)((char *)nwSocket::GetSocket(handle2) + 0x20) != 0) {
                    gcGame *game;
                    gcNetGameFillEntry *fill;
                    gcNetGameSendEntry *send;

                    u.msg.word28 = 0;
                    memset(&u.msg.words2C[0], 0, 0x10);
                    u.msg.word3C = 0;
                    fill = (gcNetGameFillEntry *)(*(char **)lobby + 0x258);
                    fill->fn((char *)lobby + fill->offset, &u.msg);
                    u.msg.numConnections =
                        gcNetGame::GetNumActiveConnections(0) + 1;
                    game = *(gcGame **)0x37D854;
                    u.msg.mapIndex = game->GetMapIndex(
                        *(cGUIDT<gcMap> *)((char *)game + 0x16F0));
                    send = (gcNetGameSendEntry *)(*(char **)lobby + 0x260);
                    send->fn((char *)lobby + send->offset,
                             &u.msg,
                             send->offset);
                }
            }
        }
    }
}

void gcNetGame::SetAllSendMaskOnOff(unsigned int setMask,
                                    unsigned int clearMask) {
    if (nwSocket::GetConnection(D_0009A3C8) != 0) {
        nwConnection *conn = nwSocket::GetConnection(D_0009A3C8);
        conn->flags = (conn->flags | setMask) & ~clearMask;
    }
    nwSocketHandleStruct handle = nwNetwork::GetGameSocket();
    if (nwSocket::GetSocket(handle) != 0) {
        int i = 0;
        unsigned int keepMask = ~clearMask;
        while (i < nwSocket::GetSocket(handle)->GetMaxConnections()) {
            nwConnectionHandle connHandle =
                nwSocket::GetSocket(handle)->GetConnection(i);
            nwConnection *conn = nwSocket::GetConnection(connHandle);
            if (conn != 0) {
                conn->flags = (conn->flags | setMask) & keepMask;
            }
            i++;
        }
    }
}

void gcNetGame::SendAllBufferedMessages(void) {
    nwSocketHandleStruct handle = nwNetwork::GetGameSocket();
    nwSocket *sock = nwSocket::GetSocket(handle);
    if (sock) {
        nwSocketHandleStruct handle2 = nwNetwork::GetGameSocket();
        nwSocket *sock2 = nwSocket::GetSocket(handle2);
        sock2->FlushAll();
    }
}

int gcNetGame::IsClientConnected(void) {
    int result = 0;
    if (nwSocket::GetConnection(gClientConnection) != 0) {
        if (nwSocket::GetConnection(gClientConnection)->status == 2) {
            result = 1;
        }
    }
    return result & 0xFF;
}

int gcNetGame::IsServerReady(void) {
    int result = 0;
    if (nwSocket::GetConnection(D_0009A3C8) != 0) {
        if (nwSocket::GetConnection(D_0009A3C8)->flags & 1) {
            result = 1;
        }
    }
    return result & 0xFF;
}
