// cVolatile::Initialize (0x00002164, cAll_psp.obj) — isolated split-TU.
// Static layout reproduces the .bss layout at 0x37BC34:
//   s_pBase(0x37BC34), s_nSize, s_pCurrent, s_pad40, s_sema(0x37BC44), ...
// A global manager pointer at 0x0037C074 carries an init/skip flag at +0x2B8.

#include <thread.h>

// Default mopt over-merges the two `return 1` sites here, hoisting the
// constant into a callee-saved register and bloating the frame by 16B.
// mopt=2 reproduces the original branch-likely + re-materialized `li v0,1`.
// The `base` temp for s_pBase in the success path is also load-bearing for
// the register allocation/store scheduling of s_pCurrent = s_pBase.
#pragma control mopt=2

class cVolatile {
public:
    static int s_pBase;
    static int s_nSize;
    static int s_pCurrent;
    static int s_pad40;
    static int s_sema;
    static int s_nLockCount;
    static int s_nLockUtilCount;

    static int Initialize(void);
};

int cVolatile::s_pBase;
int cVolatile::s_nSize;
int cVolatile::s_pCurrent;
int cVolatile::s_pad40;
int cVolatile::s_sema;
int cVolatile::s_nLockCount;
int cVolatile::s_nLockUtilCount;

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
    if (s_sema < 0) {
        return 0;
    }
    int base = s_pBase;
    if (base == 0) {
        return 0;
    }
    s_pCurrent = base;
    return 1;
}
