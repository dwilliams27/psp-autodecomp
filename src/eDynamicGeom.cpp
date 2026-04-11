// eDynamicGeom::eDynamicGeom(cBase *)
// Address: 0x00044c44, Size: 140B, Obj: eAll_psp.obj
//
// STATUS: NEAR-MATCH (2 instructions swapped, 8/140 bytes differ)
//
// The scalar part matches PERFECTLY with -Xmopt=0 flag (needs Makefile change).
// The VFPU identity matrix init compiles correctly via .word inline asm.
//
// BLOCKER: The compiler places 'move v0, s0' (constructor return) AFTER the
// last sv.q store, but the original has it BETWEEN the 3rd and 4th sv.q.
// This interleaving cannot be reproduced from C with .word inline asm because
// the compiler treats asm blocks as opaque scheduling barriers. The original
// likely used SNC's native VFPU support, letting the scheduler see individual
// VFPU instructions.
//
// REQUIRED FLAG: -Xmopt=0 (Makefile override needed for eDynamicGeom)
// Without -Xmopt=0, the compiler merges two -1 constants (CSE), producing
// 136 bytes instead of 140. With -Xmopt=0, CSE is disabled and the compiler
// correctly generates separate 'li a0, -1' loads for mGeomIndex and mParentIndex.
//
// Assembly match available in src/eDynamicGeom_ctor.s for reference.

#include "eDynamicGeom.h"
#include "mOCS.h"

extern char eDynamicGeomvirtualtable[];

void eDynamicGeom::GetSubObjectToWorld(int, mOCS *out) const {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
    __asm__ volatile(
        "lv.q C120, 0x40(%0)\n"
        "sv.q C120, 0x30(%1)\n"
        "lv.q C120, 0x10(%0)\n"
        "sv.q C120, 0x0(%1)\n"
        "lv.q C120, 0x20(%0)\n"
        "sv.q C120, 0x10(%1)\n"
        "lv.q C120, 0x30(%0)\n"
        "sv.q C120, 0x20(%1)\n"
        : : "r"((const void *)this), "r"((void *)out)
    );
}

eDynamicGeom::eDynamicGeom(cBase *base) : eGeom(base) {
    *(void **)((char *)this + 4) = eDynamicGeomvirtualtable;
    *(short *)((char *)this + 0xD0) = -1;
    *(unsigned char *)((char *)this + 0xD2) = 0x10;
    *(float *)((char *)this + 0xD4) = 1.0f;
    *(int *)((char *)this + 0xD8) = -1;
    *(int *)((char *)this + 0xDC) = 0;
    *(int *)((char *)this + 0xE0) = 0;
    *(int *)((char *)this + 0xE4) = 0;
    *(int *)((char *)this + 0xE8) = 0;
    __asm__ volatile(
        ".word 0xf3838080\n"
        ".word 0xd0008086\n"
        ".word 0xd0008187\n"
        ".word 0xd0008288\n"
        ".word 0xd0008389\n"
        ".word 0xfa060090\n"
        ".word 0xfa0700a0\n"
        ".word 0xfa0800b0\n"
        ".word 0xfa0900c0\n"
    );
}
