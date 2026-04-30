// Decompiled functions:
//   0x0027de84 gcVariable::AssignCopy(const cBase *)
//   0x001303b8 gcVariable::Write(cFile &) const
//   0x0013051c gcVariable::~gcVariable(void)
//   0x0027decc gcVariable::New(cMemPool *, cBase *) static
//   0x00130410 gcVariable::Read(cFile &, cMemPool *)
//
// Sibling of gcConstant — same shape (single float at offset 0x44, identical
// codegen patterns). See src/gcConstant.cpp for design notes.

#include "cBase.h"

class cFile;
class cFileHandle;
class cMemPool;
class cType;

// ─── helper class declarations ────────────────────────────────────────────
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

template <class T> T *dcast(const cBase *);

// ─── pool-block layout used by ::New() and operator delete ────────────────
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

extern char gcVariableclassdesc[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F4D4;

// ============================================================================
// gcVariable — inherits cObject; mValue is a float at offset 0x44.
// ============================================================================
class gcVariable : public cObject {
public:
    char _pad[0x44];
    float mValue;             // 0x44

    gcVariable(cBase *);
    ~gcVariable();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

// ── gcVariable::AssignCopy(const cBase *) @ 0x0027de84 ──
void gcVariable::AssignCopy(const cBase *src) {
    gcVariable *other = dcast<gcVariable>(src);
    cObject::operator=(*other);
    mValue = other->mValue;
}

// ── gcVariable::GetType(void) const @ 0x0027df48 ──
const cType *gcVariable::GetType(void) const {
    if (D_0009F4D4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F4D4 = cType::InitializeType(0, 0, 0x79, D_000385E4,
                                           &gcVariable::New,
                                           (const char *)0x36DA24,
                                           (const char *)0x36DA30, 4);
    }
    return D_0009F4D4;
}

// ── gcVariable::Write(cFile &) const @ 0x001303b8 ──
void gcVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(mValue);
    wb.End();
}

// ── gcVariable::~gcVariable() @ 0x0013051c ──
gcVariable::~gcVariable() {
    ((void **)this)[1] = gcVariableclassdesc;
}

// ── gcVariable::New(cMemPool *, cBase *) static @ 0x0027decc ──
extern "C" void gcVariable_construct(void *self, cBase *parent);

cBase *gcVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcVariable *result = 0;
    gcVariable *obj = (gcVariable *)entry->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        gcVariable_construct(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcVariable::Read(cFile &, cMemPool *) @ 0x00130410 ──
int gcVariable::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile ("ori %0,$0,1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && cObject::Read(file, pool)) goto success;
    ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
    return 0;
success:
    cFileSystem::Read(*(cFileHandle **)rb._data[0], &mValue, 4);
    return result;
}
