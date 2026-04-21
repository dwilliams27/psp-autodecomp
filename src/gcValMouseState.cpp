#include "gcValMouseState.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

extern char gcValMouseStatevirtualtable[];
extern char gcValMouseStateDerivedvtable[];

gcValMouseState *dcast(const cBase *);
void gcValue_Write(const gcValMouseState *, cFile &);
void *cMemPool_GetPoolFromPtr(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// -----------------------------------------------------------------------------
// Function: gcValMouseState::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------
cBase *gcValMouseState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValMouseState *result = 0;
    gcValMouseState *obj = (gcValMouseState *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)gcValMouseStatevirtualtable;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValMouseStateDerivedvtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 1;
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// Function: gcValMouseState::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValMouseState::AssignCopy(const cBase *base) {
    gcValMouseState *other = dcast(base);
    this->pad8 = other->pad8;
    this->padC = other->padC;
}

// -----------------------------------------------------------------------------
// Function: gcValMouseState::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValMouseState::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    wb.Write(this->pad8);
    wb.Write(this->padC);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function: gcValMouseState::VisitReferences(...)
// -----------------------------------------------------------------------------
void gcValMouseState::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

// -----------------------------------------------------------------------------
// Function: gcValMouseState::~gcValMouseState(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValMouseState___dtor_gcValMouseState_void(gcValMouseState *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValMouseStatevirtualtable;
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
