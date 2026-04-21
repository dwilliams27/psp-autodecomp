#include "gcRegion.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct TypeDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

extern char gcRegionclassdesc[];   // at 0x387918
extern void *g_gcMap;              // at 0x37D7FC

void gcRegionBase_MemCardReplicate(void *, gcReplicationVisitor &);
void gcRegionBase_Write(const void *, cFile &);
void gcRegionBase_assignop(void *, const void *);
void gcEvent_assignop(void *, const void *);
void gcMap_OnRegionDeleted(void *, gcRegion *);
void cFactory_DeleteGroups(void *);
void *cMemPool_GetPoolFromPtr(const void *);
gcRegion *dcast(const cBase *);
void gcRegion_ctor(void *, cBase *);

extern "C" {
    void gcEvent___dtor_gcEvent_void(void *, int);
    void gcRegionBase___dtor_gcRegionBase_void(void *, int);
}

int gcRegion::AllowParentCreate(void) const {
    return 1;
}

void gcRegion::MemCardReplicate(gcReplicationVisitor &v) {
    gcRegionBase_MemCardReplicate(this, v);
}

void gcRegion::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcRegionBase_Write(this, file);
    {
        char *typePtr = *(char **)((char *)this + 0xAC);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xA8;
        entry->fn(base + entry->offset, wb._data[0]);
    }
    {
        char *typePtr = *(char **)((char *)this + 0xC8);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xC4;
        entry->fn(base + entry->offset, wb._data[0]);
    }
    wb.End();
}

void gcRegion::AssignCopy(const cBase *base) {
    gcRegion *other = dcast(base);
    gcRegionBase_assignop(this, other);
    int i = 0;
    const int *src = (const int *)other;
    int *dst = (int *)this;
    do {
        i++;
        *(int *)((char *)dst + 0x78) = *(const int *)((const char *)src + 0x78);
        src = (const int *)((const char *)src + 4);
        dst = (int *)((char *)dst + 4);
    } while (i < 12);
    gcEvent_assignop((char *)this + 0xA8, (const char *)other + 0xA8);
    gcEvent_assignop((char *)this + 0xC4, (const char *)other + 0xC4);
}

cBase *gcRegion::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcRegion *result = 0;
    gcRegion *obj = (gcRegion *)entry->fn(base, 0xE0, 4, 0, 0);
    if (obj != 0) {
        gcRegion_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

extern "C" {

void gcRegion___dtor_gcRegion_void(gcRegion *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = gcRegionclassdesc;
        gcMap_OnRegionDeleted(g_gcMap, self);
        cFactory_DeleteGroups(self);
        gcEvent___dtor_gcEvent_void((char *)self + 0xC4, 2);
        gcEvent___dtor_gcEvent_void((char *)self + 0xA8, 2);
        gcRegionBase___dtor_gcRegionBase_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
