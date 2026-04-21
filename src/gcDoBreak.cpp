#include "cBase.h"

class cFile;
class cMemPool;

class gcDoBreak {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern "C" {
    void cStrCat(char *, const char *);
    void *cMemPool_GetPoolFromPtr(const void *);
    void gcAction_gcAction(void *, cBase *);
    void gcAction_Write(const void *, cFile &);
    void gcAction___dtor_gcAction_void(void *, int);
}

gcDoBreak *dcast(const cBase *);

extern char gcDoBreakvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// 0x00294318, 72B
void gcDoBreak::AssignCopy(const cBase *other) {
    gcDoBreak *src = dcast(other);
    int v = ((int *)this)[2] & ~3;
    ((int *)this)[2] = v;
    ((int *)this)[2] = v | (((int *)src)[2] & 3);
}

// 0x00294360, 136B — static
cBase *gcDoBreak::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoBreak *result = 0;
    gcDoBreak *obj = (gcDoBreak *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoBreakvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00294500, 76B
void gcDoBreak::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.End();
}

// 0x00294634, 40B
void gcDoBreak::GetText(char *buf) const {
    cStrCat(buf, "break");
}

// 0x0029465c, 124B
extern "C" {

void gcDoBreak___dtor_gcDoBreak_void(gcDoBreak *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcDoBreakvirtualtable;
        gcAction___dtor_gcAction_void(self, 0);
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
