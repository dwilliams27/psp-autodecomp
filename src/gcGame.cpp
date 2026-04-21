#include "gcGame.h"

class gcGameSettings {
public:
    void Reset(cMemPool *, bool);
};

class gcCinematicInstanceX {
public:
    void DrawDialogs(void) const;
};

void gcGame::NetReplicate(bool) {
}

int gcNetGame::StartServer(void) {
    return 0;
}

void gcNetGame::CheckSynchronization(void) {
}

void gcEntityController::PostPhysicsMove(cTimeValue) {
}

void gcPlayer::HandleClientWarp(void) {
}

// 0x00106c18 — gcGame::DeleteLoadingScreen(void)
void gcGame::DeleteLoadingScreen(void) {
    ResetLoadingCinematicPools();
}

// 0x001062a8 — gcGame::Reset(cMemPool *, bool)
void gcGame::Reset(cMemPool *pool, bool) {
    ((gcGameSettings *)((char *)this + 0x1700))->Reset(pool, false);
}

// 0x00107fe8 — gcGame::OnMovieDraw(void *) static
void gcGame::OnMovieDraw(void *p) {
    if (p) {
        ((gcCinematicInstanceX *)p)->DrawDialogs();
    }
}

// 0x001067c8 — gcGame::DeleteMap(void)
class gcMapVDispatch {
public:
    int _pad0;
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v10(int);
};

void gcGame::DeleteMap(void) {
    gcMapVDispatch *map = *(gcMapVDispatch **)0x37D7FC;
    if (map != 0) {
        map->v10(3);
    }
}

// 0x0024ed4c — gcGame::New(cMemPool *, cBase *) static
namespace {
struct gcGame_PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct gcGame_AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};
}

extern void gcGame_ctor(gcGame *, cBase *);

cBase *gcGame::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((gcGame_PoolBlock *)block)->allocTable;
    gcGame_AllocEntry *entry = (gcGame_AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcGame *result = 0;
    gcGame *obj = (gcGame *)entry->fn(base, 0x17A8, 4, 0, 0);
    if (obj != 0) {
        gcGame_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
