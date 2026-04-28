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
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

struct eOnePassModelMtl_Fields {
    char pad[0x74];
    int mField74;
    const cBase *mField78;
};

#pragma control sched=1

// ── Write @ 0x00081cc4 ──

void eOnePassModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eModelMtl *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0x48))->Write(wb);
    const eOnePassModelMtl_Fields *self = (const eOnePassModelMtl_Fields *)this;
    wb.Write(self->mField74);
    wb.WriteBase(self->mField78);
    wb.End();
}

// ── New @ 0x00218920 ──

extern "C" void eOnePassModelMtl_eOnePassModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eOnePassModelMtl *eOnePassModelMtl::New(cMemPool *pool, cBase *parent) {
    eOnePassModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eOnePassModelMtl *obj = (eOnePassModelMtl *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        eOnePassModelMtl_eOnePassModelMtl(obj, parent);
        result = obj;
    }
    return result;
}
