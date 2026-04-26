// Mixed batch from this session: std::terminate (gMain_psp.obj),
// eVolume::SetLocalToWorld (eAll_psp.obj), gcValLobbyStatus::AssignCopy
// (gcAll_psp.obj), cVolatile::Unlock & cVolatile::UnlockUtil (cAll_psp.obj).

#include "mOCS.h"
#include "thread.h"

class cBase;
template <class T> T *dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

// ---- Function 1: std::terminate(void) @ 0x001bb4bc, 52B ----
// Calls a global terminate-handler function pointer if set, then unconditionally
// invokes __abort_execution(3). The handler pointer lives at 0x0037E550.

extern "C" void __abort_execution(int);

namespace std {
    typedef void (*TerminateHandler)(void);
    extern TerminateHandler s_terminate_handler;

    void terminate(void) {
        if (s_terminate_handler != 0) {
            s_terminate_handler();
        }
        __abort_execution(3);
    }
}

// ---- Function 2: eVolume::SetLocalToWorld(const mOCS &) @ 0x0005d7e8, 56B ----
// Copy mOCS (4 quadwords) into this+0x30..0x70 then call UpdateLocalToWorld.
// The compiler reorders the quad copies (translation row first) and lands the
// final sv.q in the JAL's delay slot. Mirrors eGeom::ApplyLocalToWorld pattern.

class eVolume {
public:
    void SetLocalToWorld(const mOCS &ocs);
    void UpdateLocalToWorld(void);
};

#pragma control sched=1
void eVolume::SetLocalToWorld(const mOCS &ocs) {
    *(v4sf_t *)((char *)this + 0x60) = *(v4sf_t *)((char *)&ocs + 0x30);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)&ocs + 0x00);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)&ocs + 0x10);
    *(v4sf_t *)((char *)this + 0x50) = *(v4sf_t *)((char *)&ocs + 0x20);
    UpdateLocalToWorld();
}
#pragma control sched=2

// ---- Function 3: gcValLobbyStatus::AssignCopy(const cBase *) @ 0x0034e8c4, 56B ----
// dcast then copy two ints (f8, fC). Same shape as eAnimationGroup::AssignCopy.

class gcValLobbyStatus {
public:
    char _pad0[8];
    int f8;
    int fC;
    void AssignCopy(const cBase *);
};

void gcValLobbyStatus::AssignCopy(const cBase *base) {
    gcValLobbyStatus *other = dcast<gcValLobbyStatus>(base);
    f8 = other->f8;
    fC = other->fC;
}

// ---- Function 4 & 5: cVolatile::Unlock / UnlockUtil @ 0x000022b4 / 0x00002344 ----
// Both decrement a per-channel counter and signal the shared semaphore on
// reaching zero, gated by s_sema being non-negative.

class cVolatile {
public:
    static int s_sema;
    static int s_nLockCount;
    static int s_nLockUtilCount;
    static void Unlock(void);
    static void UnlockUtil(void);
};

int cVolatile::s_sema;
int cVolatile::s_nLockCount;
int cVolatile::s_nLockUtilCount;

void cVolatile::Unlock(void) {
    if (s_sema >= 0) {
        if (--s_nLockCount == 0) {
            sceKernelSignalSema(s_sema, 1);
        }
    }
}

void cVolatile::UnlockUtil(void) {
    if (s_sema >= 0) {
        if (--s_nLockUtilCount == 0) {
            sceKernelSignalSema(s_sema, 1);
        }
    }
}
