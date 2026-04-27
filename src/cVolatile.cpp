// cVolatile: small bump-allocator backed by a fixed-size buffer plus a
// semaphore that gates concurrent access. Lock/Unlock and LockUtil/Unlock-
// Util are nested counter pairs sharing the same semaphore; LockUtil
// additionally drains the GE pipeline and invalidates the buffer pointer.
//
// Already matched in src/std.cpp: cVolatile::Unlock, cVolatile::UnlockUtil.
// This file adds the remaining five: Initialize, Alloc, Lock, LockUtil,
// GetFreeSize. The static layout (s_pBase, s_nSize, s_pCurrent, _, s_sema,
// s_nLockCount, s_nLockUtilCount) reproduces the .bss layout at 0x37BC34.

#include <thread.h>
#include <geman.h>

class cVolatile {
public:
    static int s_pBase;
    static int s_nSize;
    static int s_pCurrent;
    static int s_pad40;
    static int s_sema;
    static int s_nLockCount;
    static int s_nLockUtilCount;

    static int  Initialize(void);
    static int  Alloc(unsigned int size, int align);
    static void Lock(void);
    static void LockUtil(void);
    static int  GetFreeSize(int align);
};

int cVolatile::s_pBase;
int cVolatile::s_nSize;
int cVolatile::s_pCurrent;
int cVolatile::s_pad40;
int cVolatile::s_sema;
int cVolatile::s_nLockCount;
int cVolatile::s_nLockUtilCount;

// Loaded from a global pointer at 0x0037C074 (offset 0x2B8 = an init flag).
struct cVolatileMgr {
    char           m_pad[0x2B8];
    unsigned char  m_bSkipInit;
};
extern cVolatileMgr *g_pVolatileMgr;

int cVolatile::Initialize(void) {
    cVolatileMgr *p = g_pVolatileMgr;
    if (p != 0 && p->m_bSkipInit == 0) {
        return 1;
    }
    s_sema = sceKernelCreateSema("veVolatile", 0, 1, 1, 0);
    if (s_sema < 0) return 0;
    if (s_pBase == 0) return 0;
    s_pCurrent = s_pBase;
    return 1;
}

int cVolatile::Alloc(unsigned int size, int align) {
    if (s_pBase == 0) return 0;
    int n = align;
    if (align < 4) n = 4;
    unsigned int un = (unsigned int)(n + 3) >> 2 << 2;
    unsigned int m1 = un - 1u;
    unsigned int aligned = ((unsigned int)s_pCurrent + m1) / un * un;
    unsigned int newCur = aligned + size;
    if ((unsigned int)s_nSize < newCur - (unsigned int)s_pBase) return 0;
    s_pCurrent = (int)newCur;
    return (int)aligned;
}

void cVolatile::Lock(void) {
    if (s_sema >= 0) {
        if (++s_nLockCount == 1) {
            sceKernelWaitSema(s_sema, 1, 0);
        }
    }
}

void cVolatile::LockUtil(void) {
    if (s_sema >= 0) {
        if (++s_nLockUtilCount == 1) {
            sceKernelWaitSema(s_sema, 1, 0);
            sceGeDrawSync(0);
            s_pBase = 0;
        }
    }
}

int cVolatile::GetFreeSize(int align) {
    if (s_pBase == 0) return 0;
    int n = align;
    if (align < 4) n = 4;
    unsigned int un = (unsigned int)(n + 3) >> 2 << 2;
    unsigned int cur = (unsigned int)s_pCurrent;
    unsigned int m1 = un - 1u;
    return s_nSize - (int)((cur + m1) / un * un - (unsigned int)s_pBase);
}
