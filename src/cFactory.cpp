#include "cObject.h"

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

// ── cFactory::Write(cFile &) const ──
void cFactory::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cObject *)this)->Write(file);
    wb.End();
}

// ── cFactory::Clean(bool) ──
void cFactory::Clean(bool flag) {
    this->ClearVisitedReferences(0xfe00);
    this->MarkForClean(this->mField48);
    this->DeleteMarkedForClean(this->mField48, flag);
}

// ── cFactory::LoadLocalized(void) ──
class cStr {
public:
    char _data[256];
    cStr() { _data[0] = '\0'; }
    operator const char *() const { return _data; }
};

class cGUID;
class cType;

struct TypeDispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *);
};

namespace cObject_Static {
    void GetLocalizedFilename(const cType *, const cGUID &, cStr *);
}

void cFactory::LoadLocalized(void) {
    char *classdesc = *(char **)((char *)this + 4);
    cStr strBuf;
    TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 8);
    cType *type = entry->fn((char *)this + entry->offset);
    cObject_Static::GetLocalizedFilename(type, *(cGUID *)((char *)this + 32), &strBuf);
    this->LoadLocalized((const char *)strBuf);
}

// ── cFactory::Load(void) ──
struct LoadDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, unsigned int, int, int, int, int);
};

int cFactory::Load(void) {
    this->LoadLocalized();
    if (!((cObject *)this)->Load()) {
        return 0;
    }
    unsigned int f48 = this->mField48;
    if (f48 != 0) {
        char *classdesc = *(char **)((char *)this + 4);
        LoadDispatchEntry *entry = (LoadDispatchEntry *)(classdesc + 96);
        entry->fn((char *)this + entry->offset, f48, 0, 0, 0, 1);
    }
    return 1;
}

// ── cFactory::~cFactory(void) ──
extern char cFactoryvirtualtable[];

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
void cObject___dtor_cObject_void(void *self, int flags);
void *cMemPool_GetPoolFromPtr(const void *);

void cFactory___dtor_cFactory_void(cFactory *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = cFactoryvirtualtable;
        cObject___dtor_cObject_void(self, 0);
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
