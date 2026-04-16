// eDynamicGeom::eDynamicGeom(cBase *)
// Address: 0x00044c44, Size: 140B, Obj: eAll_psp.obj
//
// STATUS: NEAR-MATCH (8/140 bytes differ — 2 instructions swapped at 0x74/0x78)
//
// Scalar part matches perfectly using inline asm for -1 loads (prevents CSE)
// and for the 0x10 load (controls scheduling). No -Xmopt=0 flag needed.
//
// REMAINING BLOCKER: The compiler places 'move v0, s0' (constructor return)
// AFTER the last sv.q store, but the expected has it BETWEEN the 3rd and 4th
// sv.q. This cannot be fixed because asm blocks are opaque to the scheduler.
// The original compiler could interleave MIPS code with VFPU stores natively.

#include "eDynamicGeom.h"
#include "ePath.h"
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
    void *vt = eDynamicGeomvirtualtable;
    int neg;
    __asm__ volatile("addiu %0, $0, -1" : "=r"(neg));
    *(void **)((char *)this + 4) = vt;
    int typ;
    __asm__ volatile("ori %0, $0, 0x10" : "=r"(typ));
    *(short *)((char *)this + 0xD0) = (short)neg;
    *(unsigned char *)((char *)this + 0xD2) = (unsigned char)typ;
    float f = 1.0f;
    int neg2;
    __asm__ volatile("addiu %0, $0, -1" : "=r"(neg2));
    *(float *)((char *)this + 0xD4) = f;
    *(int *)((char *)this + 0xD8) = neg2;
    *(int *)((char *)this + 0xDC) = 0;
    *(int *)((char *)this + 0xE0) = 0;
    *(int *)((char *)this + 0xE4) = 0;
    *(int *)((char *)this + 0xE8) = 0;
    __asm__ volatile(
        "vmidt.q M000\n"
        "vmov.q C120, C000\n"
        "vmov.q C130, C010\n"
        "vmov.q C200, C020\n"
        "vmov.q C210, C030\n"
        "sv.q C120, 0x90(%0)\n"
        "sv.q C130, 0xA0(%0)\n"
        "sv.q C200, 0xB0(%0)\n"
        "sv.q C210, 0xC0(%0)\n"
        : : "r"(this) : "memory"
    );
}

float ePath::PathT2Units(float t) const {
    int intPart;
    __asm__ volatile(
        "trunc.w.s $f13, %1\n"
        "mfc1 %0, $f13\n"
        : "=r"(intPart) : "f"(t)
    );
    float nextT = t + 1.0f;
    ePathPoint *pts = mPoints;
    float maxIdx;

    if (pts != 0) {
        int count = *(int *)((char *)pts - 4) & 0x3FFFFFFF;
        maxIdx = (float)(count - 1);
    } else {
        maxIdx = -1.0f;
    }

    int nextIdx;
    if (nextT <= 0.0f) {
        nextIdx = (int)0.0f;
    } else {
        if (maxIdx <= nextT) {
            nextT = maxIdx;
        }
        nextIdx = (int)nextT;
    }

    float dist = pts[intPart].mDistance;
    float nextDist = pts[nextIdx].mDistance;
    float frac = t - (float)intPart;
    return dist + frac * (nextDist - dist);
}
