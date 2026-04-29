#include "gcGame.h"

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct gcGame_DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

struct gcGame_ClassDesc {
    char _pad[8];
    gcGame_DispatchEntry dispatch;
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C90;
extern cType *D_0009A300;

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

// 0x0024edc8 — gcGame::GetType(void) const
const cType *gcGame::GetType(void) const {
    if (D_0009A300 == 0) {
        if (D_00040C90 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(
                    0, 0, 3, D_000385E0, 0, 0, 0, 0);
            }
            D_00040C90 = cType::InitializeType(
                0, 0, 5, D_000385E4, 0, 0, 0, 0);
        }
        D_0009A300 = cType::InitializeType(
            0, 0, 0x64, D_00040C90, &gcGame::New,
            (const char *)0x36D948, (const char *)0x36D950, 1);
    }
    return D_0009A300;
}

// 0x0024eb5c — gcGame::AssignCopy(const cBase *)
void gcGame::AssignCopy(const cBase *base) {
    const gcGame *other = 0;

    if (base != 0) {
        if (D_0009A300 == 0) {
            if (D_00040C90 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894,
                                (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            &cNamed::New, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                D_00040C90 = cType::InitializeType(
                    0, 0, 5, D_000385E4, 0, 0, 0, 0);
            }
            D_0009A300 = cType::InitializeType(
                0, 0, 0x64, D_00040C90, &gcGame::New,
                (const char *)0x36D948, (const char *)0x36D950, 1);
        }

        gcGame_ClassDesc *classDesc = *(gcGame_ClassDesc **)((char *)base + 4);
        cType *target = D_0009A300;
        short offset = classDesc->dispatch.offset;
        cType *(*fn)(void *, short, void *) = classDesc->dispatch.fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            if (type != 0) {
loop_cast:
                if (type != target) {
                    type = type->mParent;
                    if (type != 0) {
                        goto loop_cast;
                    }
                    goto invalid_cast;
                }
                isValid = 1;
            } else {
invalid_cast:
                isValid = 0;
            }
        } else {
            goto invalid_cast;
        }

        if (isValid != 0) {
            other = (const gcGame *)base;
        }
    }
    operator=(*other);
}
