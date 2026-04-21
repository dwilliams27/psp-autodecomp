#include "gcValCameraFollowEntity3rdVariable.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char gcValCameraFollowEntity3rdVariablevirtualtable[];
extern char gcValCameraFollowEntity3rdVariableDerivedvtable[];

extern const char gcValCameraFollowEntity3rdVariable_text[];

gcValCameraFollowEntity3rdVariable *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValCameraFollowEntity3rdVariable *, cFile &);
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
// Function: gcValCameraFollowEntity3rdVariable::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValCameraFollowEntity3rdVariable::AssignCopy(const cBase *base) {
    dcast(base);
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::GetText(char *) const
// -----------------------------------------------------------------------------
void gcValCameraFollowEntity3rdVariable::GetText(char *buf) const {
    cStrAppend(buf, gcValCameraFollowEntity3rdVariable_text);
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValCameraFollowEntity3rdVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue_Write(this, file);
    wb.End();
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------
cBase *gcValCameraFollowEntity3rdVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValCameraFollowEntity3rdVariable *result = 0;
    gcValCameraFollowEntity3rdVariable *obj = (gcValCameraFollowEntity3rdVariable *)entry->fn(base, 0x8, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)gcValCameraFollowEntity3rdVariablevirtualtable;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValCameraFollowEntity3rdVariableDerivedvtable;
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::~gcValCameraFollowEntity3rdVariable(void)
// -----------------------------------------------------------------------------
extern "C" {

void gcValCameraFollowEntity3rdVariable___dtor_gcValCameraFollowEntity3rdVariable_void(gcValCameraFollowEntity3rdVariable *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcValCameraFollowEntity3rdVariablevirtualtable;
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
