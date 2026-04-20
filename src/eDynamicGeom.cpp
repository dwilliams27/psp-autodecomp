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
#include "cFile.h"

extern char eDynamicGeomvirtualtable[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

void eGeom_Write(const void *, cFile &);

void eDynamicGeom::SetDynamicGeomFlagsOnOff(unsigned int on, unsigned int off) {
    mGeomType |= on;
    mGeomType &= ~off;
}

void eDynamicGeom::DetachChildren(void) {
    eDynamicGeom *child = (eDynamicGeom *)mFieldE0;
    if (child != 0) {
        do {
            child->Detach();
            child = (eDynamicGeom *)mFieldE0;
        } while (child != 0);
    }
}

void eDynamicGeom::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eGeom_Write(this, file);
    wb.End();
}

// eDynamicGeom::UpdateChildFlags — STATUS: NEAR-MATCH (47/92 bytes differ)
// Structurally identical 23 instructions, but SNC register-allocates differently.
// Expected uses: child→a0, off→a1, on→a2 (stays), a3 as multi-purpose temp.
// Mine uses:     child→a2, on→t0 (saved), a1→on, a3→off (stays).
// Root cause: the register allocator picks different argument-register assignments
// based on unidentified heuristics. Permuter ran 300s with only marginal gain
// (47→45). Tried: (1) simple form, (2) `unsigned int newOn=on/newOff=off` locals
// (this puts child→a0 correctly but coalesces away the phi → 22 insns only, 88B),
// (3) if-else with explicit finalOff (inserts unconditional branch), (4) local
// `unsigned char flag = child->mGeomType` (no effect), (5) struct VtEntry for
// vtable lookup (no effect), (6) reassigning newOn=on at end of if-body (coalesced).
// The two-variant tradeoff: can get correct SIZE (23 insns) with wrong reg alloc,
// OR correct reg alloc for `child` but missing one phi-move instruction.
// Not bnel divergence (diff >10 bytes). Likely needs specific source idiom or
// a different -Xflag combination to trigger SNC's alternate allocation strategy.
void eDynamicGeom::UpdateChildFlags(eDynamicGeom *child, unsigned int on, unsigned int off) {
    if (child->mGeomType & 1) {
        on &= ~1u;
        off &= ~1u;
    }
    int *vt = *(int **)((char *)child + 4);
    int *entry = (int *)((char *)vt + 0x70);
    short adj = *(short *)entry;
    void (*fn)(void *, unsigned int, unsigned int) =
        (void (*)(void *, unsigned int, unsigned int))entry[1];
    fn((char *)child + adj, on, off);
}

void eDynamicGeom::UpdateLocalToWorldRecurse(void) {
    int *vt = *(int **)((char *)this + 4);
    int *entry = (int *)((char *)vt + 0xB8);
    short adj = *(short *)entry;
    void (*fn)(void *) = (void (*)(void *))entry[1];
    fn((char *)this + adj);
    eDynamicGeom *child = (eDynamicGeom *)mFieldE0;
    if (child != 0) {
        do {
            child->UpdateLocalToWorldRecurse();
            child = (eDynamicGeom *)child->mFieldE8;
        } while (child != (eDynamicGeom *)mFieldE0);
    }
}

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
