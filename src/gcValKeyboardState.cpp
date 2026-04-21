#include "gcValKeyboardState.h"
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

extern char gcValKeyboardStatevirtualtable[];
extern char gcValKeyboardStateDerivedvtable[];

gcValKeyboardState *dcast(const cBase *);
void gcValue_Write(const gcValKeyboardState *, cFile &);
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
// Function: gcValKeyboardState::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------
cBase *gcValKeyboardState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValKeyboardState *result = 0;
    gcValKeyboardState *obj = (gcValKeyboardState *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)gcValKeyboardStatevirtualtable;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValKeyboardStateDerivedvtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0x48;
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// Function: gcValKeyboardState::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValKeyboardState::AssignCopy(const cBase *base) {
    gcValKeyboardState *other = dcast(base);
    this->pad8 = other->pad8;
    this->padC = other->padC;
}

// -----------------------------------------------------------------------------
// Function: gcValKeyboardState::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValKeyboardState::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    wb.Write(this->pad8);
    wb.Write(this->padC);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function: gcValKeyboardState::VisitReferences(...)
// -----------------------------------------------------------------------------
void gcValKeyboardState::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

// -----------------------------------------------------------------------------
// Function: gcValKeyboardState::~gcValKeyboardState(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValKeyboardState___dtor_gcValKeyboardState_void(gcValKeyboardState *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValKeyboardStatevirtualtable;
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
