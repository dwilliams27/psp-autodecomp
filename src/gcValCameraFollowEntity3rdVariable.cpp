#include "gcValCameraFollowEntity3rdVariable.h"
#include "gcValCameraValue.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, int);
    ~cReadBlock(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern char gcValCameraFollowEntity3rdVariablevirtualtable[];
extern char gcValCameraFollowEntity3rdVariableDerivedvtable[];
extern char cBaseclassdesc[];                           // @ 0x37E6A8

extern const char gcValCameraFollowEntity3rdVariable_text[];
extern const char gcValCameraFollowEntity3rdVariable_base_name[];
extern const char gcValCameraFollowEntity3rdVariable_base_desc[];

gcValCameraFollowEntity3rdVariable *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValCameraFollowEntity3rdVariable *, cFile &);
void cFile_SetCurrentPos(void *, unsigned int);
int gcLValue_Read(void *, cFile &, cMemPool *);

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// Pool-block layout used by the New allocator path.
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
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
// Function: gcValCameraFollowEntity3rdVariable::Evaluate(void) const
// -----------------------------------------------------------------------------
float gcValCameraFollowEntity3rdVariable::Evaluate(void) const {
    return 0.0f;
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::Set(float)
// -----------------------------------------------------------------------------
void gcValCameraFollowEntity3rdVariable::Set(float) {
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::Read(cFile &, cMemPool *)
// -----------------------------------------------------------------------------
int gcValCameraFollowEntity3rdVariable::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 2, true);
    if (rb._data[3] != 2) goto fail;
    if (gcLValue_Read(this, file, pool) == 0) goto fail;
    return result;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::GetType(void) const
// -----------------------------------------------------------------------------
static cType *gcValCFE3V_type_base;
static cType *gcValCFE3V_type_6A;
static cType *gcValCFE3V_type_6C;
static cType *gcValCFE3V_type_6D;
static cType *gcValCFE3V_type_self;

const cType *gcValCameraFollowEntity3rdVariable::GetType(void) const {
    if (!gcValCFE3V_type_self) {
        if (!gcValCFE3V_type_6D) {
            if (!gcValCFE3V_type_6C) {
                if (!gcValCFE3V_type_6A) {
                    if (!gcValCFE3V_type_base) {
                        gcValCFE3V_type_base = cType::InitializeType(gcValCameraFollowEntity3rdVariable_base_name, gcValCameraFollowEntity3rdVariable_base_desc, 1, 0, 0, 0, 0, 0);
                    }
                    gcValCFE3V_type_6A = cType::InitializeType(0, 0, 0x6A, gcValCFE3V_type_base, 0, 0, 0, 0);
                }
                gcValCFE3V_type_6C = cType::InitializeType(0, 0, 0x6C, gcValCFE3V_type_6A, 0, 0, 0, 0x80);
            }
            gcValCFE3V_type_6D = cType::InitializeType(0, 0, 0x6D, gcValCFE3V_type_6C, 0, 0, 0, 0);
        }
        gcValCFE3V_type_self = cType::InitializeType(0, 0, 0xCA, gcValCFE3V_type_6D, gcValCameraFollowEntity3rdVariable::New, 0, 0, 0);
    }
    return gcValCFE3V_type_self;
}

// -----------------------------------------------------------------------------
// Function: gcValCameraValue::Evaluate(void) const
// -----------------------------------------------------------------------------
float gcValCameraValue::Evaluate(void) const {
    return 0.0f;
}

// -----------------------------------------------------------------------------
// Function: gcValCameraFollowEntity3rdVariable::~gcValCameraFollowEntity3rdVariable(void)
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain, and the deleting-tail dispatch through
// operator delete on (flag & 1). Body just resets the classdesc pointer at
// offset 4 to the parent (cBase) classdesc.
// -----------------------------------------------------------------------------
gcValCameraFollowEntity3rdVariable::~gcValCameraFollowEntity3rdVariable(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
