#include "mVec3.h"

// External helpers (unresolved; relocations masked during comparison).
extern "C" int cIRand();
extern "C" float sinf(float);
extern "C" float cosf(float);

// --------------------------------------------------------------------------
// mVec3::mVec3(float, float, float) @ 0x00010d98
// Produces the mtv/sv.q quad-store pattern for a 3-float initializer.
// --------------------------------------------------------------------------
mVec3::mVec3(float x, float y, float z) {
    int ia, ib, ic;
    __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(x));
    __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(y));
    __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(ia), "r"(ib), "r"(ic), "r"(this)
    );
}

// mVec3::IRandDir(float) moved to its own byte-exact TU
// (src/mVec3__IRandDir_float__0019F5DC.cpp); the prior in-TU attempt here was
// a 2-byte commutative-operand-order near-miss.

// --------------------------------------------------------------------------
// eBspTree::eBspTree(cBase *) @ 0x00021998
// --------------------------------------------------------------------------
class cBase;

struct eBspTree {
    void *m_parent;   // 0x00
    void *m_vtable;   // 0x04
    int m_u08;        // 0x08
    int m_u0C;        // 0x0C
    int m_u10;        // 0x10
    eBspTree(cBase *parent);
};

eBspTree::eBspTree(cBase *parent) {
    m_parent = parent;
    m_vtable = (void *)0x37FB80;
    m_u08 = 0;
    m_u0C = 0;
    m_u10 = 0;
}

// --------------------------------------------------------------------------
// eCameraEffectMgr::Reset(void) @ 0x0002d798
// --------------------------------------------------------------------------
struct eCameraEffectMgr {
    void Reset();
};

void eCameraEffectMgr::Reset() {
    char *p = (char *)this + 16;
    int i = 0;
    do {
        *p = 0;
        i += 1;
        p += 20;
    } while (i < 8);
}

// --------------------------------------------------------------------------
// ePhysicsControllerConfig::ePhysicsControllerConfig(cBase *) @ 0x0004b520
// --------------------------------------------------------------------------
struct ePhysicsControllerConfig {
    void *m_parent;        // 0x00
    void *m_vtable;        // 0x04
    unsigned char m_u08;   // 0x08
    char m_pad09[0x13];    // 0x09..0x1B
    short m_u1C;           // 0x1C
    short m_u1E;           // 0x1E
    ePhysicsControllerConfig(cBase *parent);
};

ePhysicsControllerConfig::ePhysicsControllerConfig(cBase *parent) {
    m_parent = parent;
    m_vtable = (void *)0x381418;
    m_u1C = 0;
    m_u1E = 0;
    m_u08 = 0;
}
