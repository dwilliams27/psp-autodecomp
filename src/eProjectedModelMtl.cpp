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
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

#pragma control sched=1

// ── Write ──

void eProjectedModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eModelMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);
    ((cHandle *)((char *)h + 4))->Write(wb);

    wb.End();
}

// ── New ──

extern "C" void eProjectedModelMtl_eProjectedModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eProjectedModelMtl *eProjectedModelMtl::New(cMemPool *pool, cBase *parent) {
    eProjectedModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eProjectedModelMtl *obj = (eProjectedModelMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        eProjectedModelMtl_eProjectedModelMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── Header extensions ──
// eProjectedModelMtl declares Write/New via out-of-class declarations below
// because eTextureMap.h has only the trivial stubs.
