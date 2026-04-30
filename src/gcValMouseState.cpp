#include "gcValMouseState.h"
#include "cBase.h"

class cFile;
class cMemPool;
struct cFileHandle;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

extern char gcValMouseStatevirtualtable[];
extern char gcValMouseStateDerivedvtable[];
extern char cBaseclassdesc[];                                // @ 0x37E6A8

class gcValNavMeshDirection {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

gcValMouseState *dcast(const cBase *);
void gcValue_Write(const gcValMouseState *, cFile &);
int  gcValue_Read(gcValMouseState *, cFile &, cMemPool *);

class gcValMouseState_cMemPoolNS {
public:
    static gcValMouseState_cMemPoolNS *GetPoolFromPtr(const void *);
};

struct gcValMouseState_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcValMouseState_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValMouseState;
static cType *type_gcValNavMeshDirection;

// -----------------------------------------------------------------------------
// Function: gcValMouseState::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------
cBase *gcValMouseState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((gcValMouseState_PoolBlock *)block)->allocTable;
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

const cType *gcValMouseState::GetType(void) const {
    if (!type_gcValMouseState) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValMouseState = cType::InitializeType(0, 0, 0x20E, type_value,
                                                     gcValMouseState::New,
                                                     0, 0, 0);
    }
    return type_gcValMouseState;
}

const cType *gcValNavMeshDirection::GetType(void) const {
    if (!type_gcValNavMeshDirection) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValNavMeshDirection = cType::InitializeType(
            0, 0, 0xFA, type_value, gcValNavMeshDirection::New, 0, 0, 0);
    }
    return type_gcValNavMeshDirection;
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
// Function: gcValMouseState::Read(cFile &, cMemPool *)  @ 0x00350738, 228B
// -----------------------------------------------------------------------------
int gcValMouseState::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcValue_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    cFileSystem::Read((cFileHandle *)*(void **)rb._data[0], (char *)this + 8, 4);
    cFileSystem::Read((cFileHandle *)*(void **)rb._data[0], (char *)this + 12, 4);
    return result;
}

// -----------------------------------------------------------------------------
// Function: gcValMouseState::~gcValMouseState(void)  @ 0x00350b24, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard and
// the deleting-tail dispatch through operator delete on (flag & 1). The body
// resets the classdesc pointer at offset 4 to the parent (cBase) classdesc.
// -----------------------------------------------------------------------------
inline void gcValMouseState::operator delete(void *p) {
    gcValMouseState_cMemPoolNS *pool =
        gcValMouseState_cMemPoolNS::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    gcValMouseState_DeleteRecord *rec =
        (gcValMouseState_DeleteRecord *)
        (((gcValMouseState_PoolBlock *)block)->allocTable + 0x30);
    rec->fn(block + rec->offset, p);
}

gcValMouseState::~gcValMouseState(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
