#include "eTextureMap.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046CA4;

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
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
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

// ── GetType ──

const cType *eSilhouetteModelMtl::GetType(void) const {
    if (D_00046CA4 == 0) {
        if (D_00046B28 == 0) {
            if (D_00040FF8 == 0) {
                if (D_00040FEC == 0) {
                    if (D_000385E4 == 0) {
                        if (D_000385E0 == 0) {
                            if (D_000385DC == 0) {
                                const char *name = (const char *)0x36CD74;
                                const char *desc = (const char *)0x36CD7C;
                                __asm__ volatile("" : "+r"(name), "+r"(desc));
                                D_000385DC = cType::InitializeType(
                                    name, desc, 1, 0, 0, 0, 0, 0);
                            }
                            const cType *parentType = D_000385DC;
                            cBase *(*factory)(cMemPool *, cBase *) =
                                (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                            D_000385E0 = cType::InitializeType(
                                0, 0, 2, parentType, factory, 0, 0, 0);
                        }
                        D_000385E4 = cType::InitializeType(
                            0, 0, 3, D_000385E0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385E4;
                    const char *kindName = (const char *)0x36CDCC;
                    const char *kindDesc = (const char *)0x36CDD8;
                    __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
                    D_00040FEC = cType::InitializeType(
                        0, 0, 0x10, parentType, 0, kindName, kindDesc, 5);
                }
                D_00040FF8 = cType::InitializeType(0, 0, 0x12, D_00040FEC,
                                                   0, 0, 0, 0);
            }
            D_00046B28 = cType::InitializeType(0, 0, 0x13, D_00040FF8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00046B28;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x21B340;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046CA4 = cType::InitializeType(0, 0, 0x80, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046CA4;
}

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

// ── Write ──

void eSilhouetteModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eModelMtl *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0x48))->Write(wb);
    wb.Write(*(const float *)((const char *)this + 0x78));
    wb.Write(*(const float *)((const char *)this + 0x74));
    wb.Write(*(const float *)((const char *)this + 0x80));
    wb.Write(*(const float *)((const char *)this + 0x7C));
    wb.End();
}

// ── operator= ──

eSilhouetteModelMtl &
eSilhouetteModelMtl::operator=(const eSilhouetteModelMtl &other) {
    unsigned char field5E;
    signed char field5F;
    int *dstWord;
    const int *srcWord;

    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    ((unsigned char *)this)[0x5C] = ((const unsigned char *)&other)[0x5C];
    ((unsigned char *)this)[0x5D] = ((const unsigned char *)&other)[0x5D];
    field5E = ((const unsigned char *)&other)[0x5E];
    ((unsigned char *)this)[0x5E] = field5E;
    field5F = ((const signed char *)&other)[0x5F];
    ((signed char *)this)[0x5F] = field5F;
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((const char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))
        ->operator=(*(const cArrayBase<cHandleT<eMaterial> > *)((const char *)&other + 0x64));
    dstWord = (int *)((char *)this + 0x68);
    srcWord = (const int *)((const char *)&other + 0x68);
    __asm__ volatile("" : "+r"(dstWord), "+r"(srcWord));
    *dstWord = *srcWord;
    dstWord = (int *)((char *)this + 0x6C);
    srcWord = (const int *)((const char *)&other + 0x6C);
    __asm__ volatile("" : "+r"(dstWord), "+r"(srcWord));
    *dstWord = *srcWord;
    *(int *)((char *)this + 0x70) = *(const int *)((const char *)&other + 0x70);
    return *this;
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
