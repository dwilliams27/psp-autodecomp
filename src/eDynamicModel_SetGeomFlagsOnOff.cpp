// ODR-WARNING: this TU intentionally redeclares eDynamicModel with a minimal
// member list (no data, no full method set) so SNC's register allocator and
// vtable-emission logic for SetGeomFlagsOnOff/ResetAnimationState don't see
// the same TU-wide context as eDynamicModel.cpp. Including eDynamicModel.h
// here would re-introduce the perturbation that broke SetSkin's match.
// DO NOT add `#include "eDynamicModel.h"`. Add new methods to this local
// class only if they're called from bodies in this file.
#include "eDynamicGeom.h"

class eDynamicModel : public eDynamicGeom {
public:
    void AddToBroadphase(void);
    void SetGeomFlagsOnOff(unsigned int, unsigned int);
    bool HasCollision(void) const;
    void RemoveFromBroadphase(void);
    void ResetAnimationState(void);
};

struct eDynamicModel_AnimEntry {
    int   f0;
    float f4;
    float f8;
    union {
        unsigned int rawC;
        struct {
            unsigned int lo31 : 31;
            unsigned int hi1  : 1;
        };
    };
};

#pragma control sched=2

void eDynamicModel::ResetAnimationState(void) {
    char *arr = *(char **)((char *)this + 0x124);
    int empty = 1;
    if (arr != 0) empty = (*(int *)(arr - 4) & 0x3FFFFFFF) == 0;
    if (empty) return;

    *(unsigned char *)((char *)this + 0x13C) = 0;
    *(unsigned char *)((char *)this + 0xD2) |= 0x10;

    int i = 0;
    int byteOff = 0;
    while (true) {
        int len = 0;
        if (arr != 0) len = *(int *)(arr - 4) & 0x3FFFFFFF;
        if (i >= len) break;
        eDynamicModel_AnimEntry *e = (eDynamicModel_AnimEntry *)(arr + byteOff);
        e->f0 = 0;
        e->f4 = 0.0f;
        e->f8 = 1.0f;
        e->lo31 = 0;
        e->hi1 = 0;
        i++;
        byteOff += 16;
        __asm__ volatile("" ::: "memory");
        arr = *(char **)((char *)this + 0x124);
    }
}

#pragma control sched=2

void eDynamicModel::SetGeomFlagsOnOff(unsigned int on, unsigned int off) {
    unsigned int flags = *(unsigned char *)((char *)this + 0x8C);
    unsigned int newFlags = (flags | on) & ~off;
    bool was = (flags & 2) != 0;
    bool will = (newFlags & 2) != 0;
    if (was && !will) {
        if (HasCollision()) RemoveFromBroadphase();
    } else if (!was && will) {
        if (HasCollision()) AddToBroadphase();
    }
    eDynamicGeom::SetGeomFlagsOnOff(on, off);
}
