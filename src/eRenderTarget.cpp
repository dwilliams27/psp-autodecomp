#include "eRenderTarget.h"

class cObject {
public:
    cObject &operator=(const cObject &);
};

template <class T> T dcast(const cBase *);

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
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

struct WordCopy {
    int value;
};

typedef int v4sf_t __attribute__((mode(V4SF)));

// ── Flip ──

extern int g_flipCount;

void eRenderTarget::Flip(void) {
    g_flipCount += 1;
}

// ── Constructor ──

extern char eRenderTargetclassdesc[];

eRenderTarget::eRenderTarget(cBase *parent) : eTexture(parent) {
    ((void **)this)[1] = eRenderTargetclassdesc;
    field_4C = 0;
    field_4D = 0;
    field_4E = 0;
    field_4F = 0;
    field_50 = 0;
    field_54 = 0;
    field_58 = 0;
}

// ── Write ──

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
    void End(void);
};

void eRenderTarget::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eTexture::Write(file);
    wb.Write(field_4C);
    wb.Write(field_4D);
    wb.Write(field_4E);
    wb.End();
}

// ── Apply ──

class eRenderSurface;

class eVRAMMgrHelper {
public:
    static void SetTextureFromSurface(const eRenderSurface *, bool);
};

struct RenderState {
    char pad[0x4C];
    int textureFlag;
};

extern RenderState D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eRenderTarget::Apply(int, const eCamera *, const eWorld *) const {
    unsigned char flags = ((const unsigned char *)this)[0x47];
    if (flags & 2) {
        eVRAMMgrHelper::SetTextureFromSurface(*(const eRenderSurface **)field_50, true);
    } else {
        int flag = D_00098428.textureFlag;
        if (flag != 0) {
            int *p = D_000984D0.ptr;
            D_000984D0.ptr = p + 1;
            *p = 0x1E000000;
            flag = 0;
            D_00098428.textureFlag = flag;

        }
    }
}

// ── New ──

void eRenderTarget_eRenderTarget(eRenderTarget *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eRenderTarget *eRenderTarget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eRenderTarget *result = 0;
    eRenderTarget *obj = (eRenderTarget *)entry->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        eRenderTarget_eRenderTarget(obj, parent);
        result = obj;
    }
    return result;
}

// ── Static stubs ──

int eRenderTarget::Initialize(void) {
    return 1;
}

void eRenderTarget::Uninitialize(void) {
}

// ── AssignCopy ──

#pragma control sched=2
void eRenderTarget::AssignCopy(const cBase *src) {
    eRenderTarget *other = dcast<eRenderTarget *>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(signed char *)((char *)this + 0x44) = *(signed char *)((char *)other + 0x44);
    *(signed char *)((char *)this + 0x45) = *(signed char *)((char *)other + 0x45);
    *(signed char *)((char *)this + 0x46) = *(signed char *)((char *)other + 0x46);
    *(unsigned char *)((char *)this + 0x47) = *(unsigned char *)((char *)other + 0x47);
    *(signed short *)((char *)this + 0x48) = *(signed short *)((char *)other + 0x48);
    *(signed short *)((char *)this + 0x4A) = *(signed short *)((char *)other + 0x4A);
    *(unsigned char *)((char *)this + 0x4C) = *(unsigned char *)((char *)other + 0x4C);
    *(unsigned char *)((char *)this + 0x4D) = *(unsigned char *)((char *)other + 0x4D);
    *(unsigned char *)((char *)this + 0x4E) = *(unsigned char *)((char *)other + 0x4E);
    *(unsigned char *)((char *)this + 0x4F) = *(unsigned char *)((char *)other + 0x4F);
    *(int *)((char *)this + 0x50) = *(int *)((char *)other + 0x50);
    *(int *)((char *)this + 0x54) = *(int *)((char *)other + 0x54);
    WordCopy *dst58 = (WordCopy *)((char *)this + 0x58);
    WordCopy *src58 = (WordCopy *)((char *)other + 0x58);
    *dst58 = *src58;
}
#pragma control sched=2

class eShadowVolumeModelMtl {
public:
    void AssignCopy(const cBase *);
};

#pragma control sched=1
void eShadowVolumeModelMtl::AssignCopy(const cBase *src) {
    eShadowVolumeModelMtl *other = dcast<eShadowVolumeModelMtl *>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)other);
    *(unsigned char *)((char *)this + 0x5C) = *(unsigned char *)((char *)other + 0x5C);
    *(unsigned char *)((char *)this + 0x5D) = *(unsigned char *)((char *)other + 0x5D);
    *(unsigned char *)((char *)this + 0x5E) = *(unsigned char *)((char *)other + 0x5E);
    *(signed char *)((char *)this + 0x5F) = *(signed char *)((char *)other + 0x5F);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->operator=(
        *(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->operator=(
        *(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x64));
    WordCopy *dst68 = (WordCopy *)((char *)this + 0x68);
    WordCopy *src68 = (WordCopy *)((char *)other + 0x68);
    *dst68 = *src68;
    WordCopy *dst6C = (WordCopy *)((char *)this + 0x6C);
    WordCopy *src6C = (WordCopy *)((char *)other + 0x6C);
    *dst6C = *src6C;
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(unsigned char *)((char *)this + 0x74) = *(unsigned char *)((char *)other + 0x74);
    *(unsigned char *)((char *)this + 0x75) = *(unsigned char *)((char *)other + 0x75);
}
#pragma control sched=2

class eRigidBodyControllerConfig {
public:
    void AssignCopy(const cBase *);
};

struct RigidConfigBlock18 {
    int value[6];
};

#pragma control sched=1
void eRigidBodyControllerConfig::AssignCopy(const cBase *src) {
    eRigidBodyControllerConfig *other = dcast<eRigidBodyControllerConfig *>(src);
    RigidConfigBlock18 *src8 = (RigidConfigBlock18 *)((char *)other + 0x08);
    RigidConfigBlock18 *dst8 = (RigidConfigBlock18 *)((char *)this + 0x08);
    __asm__ volatile("" : "+r"(src8), "+r"(dst8));
    int w0 = src8->value[0];
    int w1 = src8->value[1];
    int w2 = src8->value[2];
    dst8->value[0] = w0;
    dst8->value[1] = w1;
    dst8->value[2] = w2;
    int w3 = src8->value[3];
    int w4 = src8->value[4];
    int w5 = src8->value[5];
    dst8->value[3] = w3;
    dst8->value[4] = w4;
    dst8->value[5] = w5;
    *(float *)((char *)this + 0x20) = *(float *)((char *)other + 0x20);
    *(float *)((char *)this + 0x24) = *(float *)((char *)other + 0x24);
    *(float *)((char *)this + 0x28) = *(float *)((char *)other + 0x28);
    *(float *)((char *)this + 0x2C) = *(float *)((char *)other + 0x2C);
    *(float *)((char *)this + 0x30) = *(float *)((char *)other + 0x30);
    *(float *)((char *)this + 0x34) = *(float *)((char *)other + 0x34);
    *(float *)((char *)this + 0x38) = *(float *)((char *)other + 0x38);
    *(float *)((char *)this + 0x3C) = *(float *)((char *)other + 0x3C);
    *(int *)((char *)this + 0x40) = *(int *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(float *)((char *)other + 0x44);
    *(v4sf_t *)((char *)this + 0x50) = *(v4sf_t *)((char *)other + 0x50);
}
#pragma control sched=2
