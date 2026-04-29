#include "eTextureMap.h"

// ── Forward class declarations ──

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eDrawInfo;

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00040FF8;
extern cType *D_00046B28;
extern cType *D_00046C5C;

class eModelMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

// ── CreateData / PlatformFree (trivial stubs — sched-agnostic) ──

void eShadowFillModelMtl::CreateData(void) {
}

void eShadowFillModelMtl::PlatformFree(void) {
}

#pragma control sched=1

// ── GetType ──

const cType *eShadowFillModelMtl::GetType(void) const {
    if (D_00046C5C == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x216ABC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C5C = cType::InitializeType(0, 0, 0x12A, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C5C;
}

// ── Unapply ──

struct RenderState {
    char pad[0x20];
    int field_20;
    int field_24;
    char pad28[0xC];
    int field_34;
    int field_38;
};

extern RenderState D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eShadowFillModelMtl::Unapply(void) const {
    int f = D_00098428.field_20;
    if (f != 0) {
        int *p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = 0x24000000;
        f = 0;
        D_00098428.field_20 = f;
    }
}

// ── Write ──

void eShadowFillModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eModelMtl *)this)->Write(file);
    wb.End();
}

// ── Constructor ──

extern "C" void eGeomMtl_eGeomMtl(void *, cBase *);

extern char eShadowFillModelMtlclassdesc[];

eShadowFillModelMtl::eShadowFillModelMtl(cBase *parent) {
    eGeomMtl_eGeomMtl(this, parent);
    ((int *)this)[0x68 / 4] = -1;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((void **)this)[0x4 / 4] = eShadowFillModelMtlclassdesc;
    CreateData();
}

// ── New ──

extern "C" void eShadowFillModelMtl_eShadowFillModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eShadowFillModelMtl *eShadowFillModelMtl::New(cMemPool *pool, cBase *parent) {
    eShadowFillModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadowFillModelMtl *obj = (eShadowFillModelMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        eShadowFillModelMtl_eShadowFillModelMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── AssignCopy ──

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT &operator=(const cHandleT &rhs) { mIndex = rhs.mIndex; return *this; }
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

struct cHandle {
    int mIndex;
};

void eShadowFillModelMtl::AssignCopy(const cBase *src) {
    eShadowFillModelMtl &other = *dcast<eShadowFillModelMtl>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    ((unsigned char *)this)[0x5C] = ((unsigned char *)&other)[0x5C];
    ((unsigned char *)this)[0x5D] = ((unsigned char *)&other)[0x5D];
    ((unsigned char *)this)[0x5E] = ((unsigned char *)&other)[0x5E];
    ((signed char *)this)[0x5F] = ((signed char *)&other)[0x5F];
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x64));
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x68) = *(cHandle *)((char *)&other + 0x68);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x6C) = *(cHandle *)((char *)&other + 0x6C);
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x70 / 4] = ((int *)&other)[0x70 / 4];
}

// ── cReadBlock / cMemBlockSuspend ──

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eModelMtlRW {
public:
    int Read(cFile &, cMemPool *);
    void ApplyCommonDynamic(const eDrawInfo &, const mOCS &, float, unsigned int, eColor) const;
};

// ── PlatformRead ──

void eShadowFillModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── Read ──

int eShadowFillModelMtl::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || !((eModelMtlRW *)this)->Read(file, pool)) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}

// ── ApplyDynamic ──

class eColor {
public:
    unsigned int mColor;
};

void eShadowFillModelMtl::ApplyDynamic(const eDrawInfo &di, const mOCS &ocs, float f, unsigned int u, eColor c) const {
    ((const eModelMtlRW *)this)->ApplyCommonDynamic(di, ocs, f, (u & ~4) | 0x20, c);

    int alpha = (int)((float)((const unsigned char *)this)[0x6B]
                      * 0.003921569f
                      * *(const float *)((const char *)&di + 0x18)
                      * 255.0f) & 0xff & 0xff;

    if (D_00098428.field_24 != 1 || alpha != D_00098428.field_34 || 0xff != D_00098428.field_38) {
        int cmd = alpha << 8;
        cmd = cmd | 0x1;
        cmd = cmd | 0xff0000;
        cmd = cmd | 0xdc000000;
        int *p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = cmd;
        D_00098428.field_24 = 1;
        D_00098428.field_34 = alpha;
        D_00098428.field_38 = 0xff;
    }
}

// ── Destructor ──

extern "C" void eMaterial___dtor_eMaterial_void(void *, int);
extern "C" void *cMemPool_GetPoolFromPtr(void *);

struct SFMDeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void eShadowFillModelMtl___dtor_eShadowFillModelMtl_void(eShadowFillModelMtl *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = (void *)0x384f08;
        self->PlatformFree();
        *(void **)((char *)self + 4) = (void *)0x380018;

        char *p60 = (char *)self + 0x60;

        if (((char *)self + 0x64) != 0) {
            int *arr2 = *(int **)((char *)self + 0x64);
            int count2 = 0;
            if (arr2 != 0) {
                count2 = arr2[-1] & 0x3fffffff;
            }
            int i2 = 0;
            int cond2 = i2 < count2;
            if (cond2) {
                i2 = 1;
                do {
                    cond2 = i2 < count2;
                    i2 += 1;
                } while (cond2);
            }
            if (arr2 != 0) {
                int *arr2_hdr = arr2 - 1;
                if (arr2_hdr != 0) {
                    void *pool2 = cMemPool_GetPoolFromPtr(arr2_hdr);
                    void *block2 = *(void **)((char *)pool2 + 0x24);
                    SFMDeleteRec *rec2 = (SFMDeleteRec *)(*(char **)((char *)block2 + 0x1c) + 0x30);
                    short off2 = rec2->offset;
                    rec2->fn((char *)block2 + off2, arr2_hdr);
                }
                *(void **)((char *)self + 0x64) = 0;
            }
        }

        if (p60 != 0) {
            int *arr1 = *(int **)p60;
            int count1 = 0;
            if (arr1 != 0) {
                count1 = arr1[-1] & 0x3fffffff;
            }
            int i1 = 0;
            int cond1 = i1 < count1;
            if (cond1) {
                i1 = 1;
                do {
                    cond1 = i1 < count1;
                    i1 += 1;
                } while (cond1);
            }
            if (arr1 != 0) {
                int *arr1_hdr = arr1 - 1;
                if (arr1_hdr != 0) {
                    void *pool1 = cMemPool_GetPoolFromPtr(arr1_hdr);
                    void *block1 = *(void **)((char *)pool1 + 0x24);
                    SFMDeleteRec *rec1 = (SFMDeleteRec *)(*(char **)((char *)block1 + 0x1c) + 0x30);
                    short off1 = rec1->offset;
                    rec1->fn((char *)block1 + off1, arr1_hdr);
                }
                *(void **)p60 = 0;
            }
        }

        eMaterial___dtor_eMaterial_void(self, 0);

        if ((flags & 1) && self != 0) {
            void *pool0 = cMemPool_GetPoolFromPtr(self);
            void *block0 = *(void **)((char *)pool0 + 0x24);
            SFMDeleteRec *rec0 = (SFMDeleteRec *)(*(char **)((char *)block0 + 0x1c) + 0x30);
            short off0 = rec0->offset;
            rec0->fn((char *)block0 + off0, self);
        }
    }
}
