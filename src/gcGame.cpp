#include "gcGame.h"
#include "gcMap.h"

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

extern "C" int cIRand(void);
extern cGUIDT<gcMap> D_00099B08[];

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

// 0x00106970 — gcGame::SetNextMapToLoadRegionSetIndex(const cGUIDT<gcMap> &, const cGUIDT<gcLoadingScreen> &, int, int)
void gcGame::SetNextMapToLoadRegionSetIndex(
    const cGUIDT<gcMap> &map,
    const cGUIDT<gcLoadingScreen> &loadingScreen,
    int regionSetIndex,
    int regionSet) {
    *(cGUIDT<gcMap> *)((char *)this + 0x16F0) = map;
    *(cGUIDT<gcLoadingScreen> *)((char *)this + 0x16F8) = loadingScreen;
    __asm__ volatile("" ::: "memory");
    int savedRegionSetIndex = regionSetIndex;

    cGUIDT<gcMap> empty;
    empty.mA = 0;
    empty.mB = 0;

    regionSetIndex = 0;
    cGUIDT<gcMap> *pending = &D_00099B08[regionSetIndex];
    do {
        *pending = empty;
        regionSetIndex += 1;
        pending += 1;
    } while (regionSetIndex < 2);

    *(int *)0x37D7EC = savedRegionSetIndex;
    *(int *)0x37D7F0 = regionSet;
    *(int *)((char *)this + 0x16EC) = cIRand() | 1;
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

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A300;
        gcGame_DispatchEntry *entry = (gcGame_DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
                goto cast_done;
            }
            type = type->mParent;
            if (type != 0) {
                goto loop_cast;
            }
            goto invalid_cast;
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcGame *)base;
        }
    }
    operator=(*other);
}
