#include "eTextureMap.h"

#pragma control sched=1

// ── Forward class declarations ──

class eColor {
public:
    unsigned int mColor;
};

class eDrawInfo;

class eModelMtl {
public:
    void ApplyCommonDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
};

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);

extern char eSilhouetteModelMtlclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct eSilhouettePSPState {
    char _pad0[0x50];
    volatile int unk50;
};
extern eSilhouettePSPState gSilhouetteGpuState;

struct eSilhouettePSPDList {
    char _pad0[0x8];
    int *cursor;
};
extern eSilhouettePSPDList gSilhouetteGpuDList;

// ── ApplyDynamic ──

void eSilhouetteModelMtl::ApplyDynamic(const eDrawInfo &di, const mOCS &ocs, float f, unsigned int u, eColor c) const {
    ((const eModelMtl *)this)->ApplyCommonDynamic(di, ocs, f, u | 8, c);
}

// ── Unapply ──

void eSilhouetteModelMtl::Unapply(void) const {
    if (gSilhouetteGpuState.unk50 != 2) {
        int *p = gSilhouetteGpuDList.cursor;
        gSilhouetteGpuDList.cursor = (int *)((char *)p + 8);
        p[0] = 0x1D000001;
        p[1] = 0x9B000000;
        gSilhouetteGpuState.unk50 = 2;
    }
}

// ── GetSilhouetteOffset ──

float eSilhouetteModelMtl::GetSilhouetteOffset(const eDrawInfo &di) const {
    float unk7C = ((const float *)this)[0x7C / 4];
    float unk74 = ((const float *)this)[0x74 / 4];
    float range = unk7C - unk74;
    __asm__ volatile("" ::: "memory");
    float invRange = 1.0f / range;
    float scaled = invRange * unk7C;
    __asm__ volatile("" ::: "memory");
    float diVal = (float)((const short *)&di)[0x1E / 2];
    float t = scaled - (diVal / 5.0f) * invRange;
    float f12 = (t <= 0.0f) ? 0.0f : ((t >= 1.0f) ? 1.0f : t);
    float unk80 = ((const float *)this)[0x80 / 4];
    return unk80 + f12 * (((const float *)this)[0x78 / 4] - unk80);
}

// ── Constructor ──

eSilhouetteModelMtl::eSilhouetteModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((void **)this)[0x04 / 4] = eSilhouetteModelMtlclassdesc;
    ((volatile float *)this)[0x74 / 4] = 0.5f;
    ((volatile float *)this)[0x78 / 4] = 0.002f;
    ((volatile float *)this)[0x7C / 4] = 5.0f;
    ((volatile float *)this)[0x80 / 4] = 0.015f;
    CreateData();
}

// ── New ──

extern "C" void eSilhouetteModelMtl_eSilhouetteModelMtl(void *, cBase *);

eSilhouetteModelMtl *eSilhouetteModelMtl::New(cMemPool *pool, cBase *parent) {
    eSilhouetteModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSilhouetteModelMtl *obj = (eSilhouetteModelMtl *)entry->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        eSilhouetteModelMtl_eSilhouetteModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
