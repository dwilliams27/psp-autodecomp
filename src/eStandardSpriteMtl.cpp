#include "eStandardSpriteMtl.h"

void eStandardSpriteMtl::PlatformFree(void) {
}

void eStandardSpriteMtl::Unapply(void) const {
}

extern "C" void eStandardSpriteMtl_eStandardSpriteMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// ── eStandardSpriteMtl::New(cMemPool *, cBase *) static @ 0x001e44c4 ──
eStandardSpriteMtl *eStandardSpriteMtl::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eStandardSpriteMtl *result = 0;
    eStandardSpriteMtl *obj = (eStandardSpriteMtl *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        eStandardSpriteMtl_eStandardSpriteMtl(obj, parent);
        result = obj;
    }
    return result;
}
