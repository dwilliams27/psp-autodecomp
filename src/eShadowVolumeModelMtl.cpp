#include "eTextureMap.h"

// ── Forward class declarations ──

class eModelMtl {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct eShadowVolumeModelMtl_Fields {
    char pad[0x74];
    bool mField74;
    bool mField75;
};

// ── PlatformFree (trivial stub — sched-agnostic) ──

void eShadowVolumeModelMtl::PlatformFree(void) {
}

#pragma control sched=1

// ── Write @ 0x00086010 ──

void eShadowVolumeModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eModelMtl *)this)->Write(file);
    const eShadowVolumeModelMtl_Fields *self = (const eShadowVolumeModelMtl_Fields *)this;
    wb.Write(self->mField74);
    wb.Write(self->mField75);
    wb.End();
}

// ── PlatformRead @ 0x000861cc ──

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

void eShadowVolumeModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── New @ 0x0021a8c4 ──

extern "C" void eShadowVolumeModelMtl_eShadowVolumeModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eShadowVolumeModelMtl *eShadowVolumeModelMtl::New(cMemPool *pool, cBase *parent) {
    eShadowVolumeModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadowVolumeModelMtl *obj = (eShadowVolumeModelMtl *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        eShadowVolumeModelMtl_eShadowVolumeModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
