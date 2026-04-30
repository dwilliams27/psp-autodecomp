// gcExternalVariable.cpp — decompiled from gcAll_psp.obj
// Parent: cObject. Single float member at offset 0x44.

#include "cBase.h"

class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F4C0;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    ~cObject();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

template <class T> T *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class gcExternalVariable : public cObject {
public:
    char _pad[0x44];
    float mField44;

    gcExternalVariable(cBase *);
    ~gcExternalVariable();
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

extern char gcExternalVariablevirtualtable[];

// gcExternalVariable::GetType(void) const @ 0x0027c130
const cType *gcExternalVariable::GetType(void) const {
    if (D_0009F4C0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F4C0 = cType::InitializeType(0, 0, 0x165, D_000385E4,
                                           &gcExternalVariable::New,
                                           (const char *)0x36D9FC,
                                           (const char *)0x36DA10,
                                           1);
    }
    return D_0009F4C0;
}

// gcExternalVariable::AssignCopy(const cBase *) @ 0x0027c06c
void gcExternalVariable::AssignCopy(const cBase *other) {
    gcExternalVariable *p = dcast<gcExternalVariable>(other);
    cObject::operator=(*p);
    mField44 = p->mField44;
}

// gcExternalVariable::Write(cFile &) const @ 0x0012f5c8
void gcExternalVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(mField44);
    wb.End();
}

// gcExternalVariable::Read(cFile &, cMemPool *) @ 0x0012f620
int gcExternalVariable::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((cObject *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x44, 4);
    return result;
}

// gcExternalVariable::~gcExternalVariable(void) @ 0x0012f72c
// Canonical C++ destructor. SNC ABI auto-generates the (this != 0) guard,
// the chain call to ~cObject() with flags=0, and the deleting-tail dispatch
// inlined from operator delete. Same pattern as eSoundData::~eSoundData.
gcExternalVariable::~gcExternalVariable() {
    *(void **)((char *)this + 4) = gcExternalVariablevirtualtable;
}
