#include "eStandardGeomTrailMtl.h"
#include "eStandardHeightmapMtl.h"

void eStandardGeomTrailMtl::PlatformFree(void) {
}

void eStandardGeomTrailMtl::Unapply(void) const {
}

void eStandardGeomTrailMtl::CreateData(void) {
}

void eStandardHeightmapMtl::PlatformFree(void) {
}

void eStandardHeightmapMtl::Unapply(void) const {
}

// ── Forward class declarations for Write/New ──

class eGeomTrailMtl {
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

#pragma control sched=1

// ── eStandardGeomTrailMtl::Write(cFile &) const @ 0x00087750 ──
void eStandardGeomTrailMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eGeomTrailMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);

    wb.Write(*(int *)((char *)this + 0x6C));
    wb.WriteBase(*(const cBase **)((char *)this + 0x70));
    wb.End();
}

// ── eStandardGeomTrailMtl::New(cMemPool *, cBase *) static @ 0x0021b628 ──

extern "C" void eStandardGeomTrailMtl_eStandardGeomTrailMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eStandardGeomTrailMtl *eStandardGeomTrailMtl::New(cMemPool *pool, cBase *parent) {
    eStandardGeomTrailMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStandardGeomTrailMtl *obj = (eStandardGeomTrailMtl *)entry->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        eStandardGeomTrailMtl_eStandardGeomTrailMtl(obj, parent);
        result = obj;
    }
    return result;
}
