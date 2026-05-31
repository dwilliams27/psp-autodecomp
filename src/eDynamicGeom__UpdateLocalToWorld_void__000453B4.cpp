// eDynamicGeom::UpdateLocalToWorld(void)
// Address: 0x000453b4, Size: 252B, Obj: eAll_psp.obj
// Symbol: __0fMeDynamicGeomSUpdateLocalToWorldv

#include "eDynamicGeom.h"
#include "mOCS.h"

// Tail call into eGeom::UpdateLocalToWorld (0x2cb20).
extern "C" void eGeom__UpdateLocalToWorld(void *);

// Vtable entry at +0xE8 of the object pointed to by mFieldDC:
//   { short offset; short pad; void (*fn)(void *, short, void *); }
struct _LtwEntry {
    short offset;
    short pad;
    void (*fn)(void *, short, void *);
};

void eDynamicGeom::UpdateLocalToWorld(void) {
    void *src = (void *)mFieldDC;
    unsigned int call;
    if (src != 0) {
        void *vt = *(void **)((char *)src + 4);
        _LtwEntry *e = (_LtwEntry *)((char *)vt + 0xE8);
        short adj = e->offset;
        // local 0x40-byte matrix at sp+0
        char ltw[0x40];
        e->fn((char *)src + adj, mGeomIndex, ltw);

        unsigned int d2 = *(unsigned char *)((char *)this + 0xD2);
        unsigned int f8c = *(unsigned char *)((char *)this + 0x8C);
        call = f8c & 4;
        if (d2 & 4) {
            __asm__ volatile(
                "vmidt.t M000\n"
                "vmov.t  C120, C000\n"
                "vmov.t  C130, C010\n"
                "vmov.t  C200, C020\n"
                "sv.q    C120, 0x0(%0)\n"
                "sv.q    C130, 0x10(%0)\n"
                "sv.q    C200, 0x20(%0)\n"
                :
                : "r"(ltw)
                : "memory"
            );
        }
        SceULong128 r3;
        __asm__ volatile(
            "lv.q  C120, 0x90(%2)\n"
            "lv.q  C130, 0xa0(%2)\n"
            "lv.q  C200, 0xb0(%2)\n"
            "lv.q  C210, 0xc0(%2)\n"
            "lv.q  C000, 0x0(%1)\n"
            "lv.q  C010, 0x10(%1)\n"
            "lv.q  C020, 0x20(%1)\n"
            "lv.q  C030, 0x30(%1)\n"
            "vtfm3.t  C220, E000, C120\n"
            "vtfm3.t  C120, E000, C130\n"
            "vtfm3.t  C130, E000, C200\n"
            "vhtfm4.q %0, E000, C210\n"
            "sv.q  C220, 0x10(%2)\n"
            "sv.q  C120, 0x20(%2)\n"
            "sv.q  C130, 0x30(%2)\n"
            : "=X"(r3)
            : "r"(ltw), "r"(this)
            : "memory"
        );
        *(SceULong128 *)((char *)this + 0x40) = r3;
    } else {
        SceULong128 e3;
        __asm__ volatile(
            "lv.q  C120, 0xc0(%1)\n"
            "lv.q  C130, 0x90(%1)\n"
            "sv.q  C120, 0x40(%1)\n"
            "lv.q  C120, 0xa0(%1)\n"
            "sv.q  C130, 0x10(%1)\n"
            "lv.q  %0, 0xb0(%1)\n"
            "sv.q  C120, 0x20(%1)\n"
            : "=X"(e3)
            : "r"(this)
            : "memory"
        );
        unsigned int f8c = *(unsigned char *)((char *)this + 0x8C);
        *(SceULong128 *)((char *)this + 0x30) = e3;
        call = f8c & 4;
    }
    if (call != 0) {
        eGeom__UpdateLocalToWorld(this);
    }
}
