#include "eTextureMap.h"

// ── GetNumExternalDependencies ──

int eBumpOffsetMap::GetNumExternalDependencies(void) const {
    return ((*(short *)((char *)this + 0x1C) == 0) & 0xFF) == 0;
}

// ── Constructor ──

extern char eBumpOffsetMapclassdesc[];

eBumpOffsetMap::eBumpOffsetMap(cBase *parent) : eTexture(parent) {
    ((void **)this)[1] = eBumpOffsetMapclassdesc;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x4C] = 1;
    *(float *)((char *)this + 0x50) = 0.1f;
    *(int *)((char *)this + 0x54) = 0;
}

// ── Write ──

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void End(void);
};

void eBumpOffsetMap::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eTexture::Write(file);
    wb.Write(field_4C);
    wb.Write(field_50);
    wb.End();
}

// ── New ──

void eBumpOffsetMap_eBumpOffsetMap(eBumpOffsetMap *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eBumpOffsetMap *eBumpOffsetMap::New(cMemPool *pool, cBase *parent) {
    eBumpOffsetMap *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBumpOffsetMap *obj = (eBumpOffsetMap *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        eBumpOffsetMap_eBumpOffsetMap(obj, parent);
        result = obj;
    }
    return result;
}

// ── Destructor ──

extern char eBumpOffsetMapvirtualtable[];
void eBumpOffsetMap_PlatformFree(eBumpOffsetMap *);
void eTexture___dtor_eTexture_void(void *, int);
void *cMemPool_GetPoolFromPtr(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {

void eBumpOffsetMap___dtor_eBumpOffsetMap_void(eBumpOffsetMap *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eBumpOffsetMapvirtualtable;
        eBumpOffsetMap_PlatformFree(self);
        eTexture___dtor_eTexture_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, self);
        }
    }
}

}

// ── Stubs (already matched) ──

void eBumpOffsetMap::PlatformFree(void) {
}

void eBumpOffsetMap::CreateData(void) {
}

void eBumpOffsetMap::Apply(int, const eCamera *, const eWorld *) const {
}
