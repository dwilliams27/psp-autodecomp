// gcExternalVariable.cpp — decompiled from gcAll_psp.obj
// Parent: cObject. Single float member at offset 0x44.

#include "cBase.h"

class cFile;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    ~cObject();
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

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
    void Write(cFile &) const;
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcExternalVariablevirtualtable[];

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

// gcExternalVariable::~gcExternalVariable(void) @ 0x0012f72c
// Canonical C++ destructor. SNC ABI auto-generates the (this != 0) guard,
// the chain call to ~cObject() with flags=0, and the deleting-tail dispatch
// inlined from operator delete. Same pattern as eSoundData::~eSoundData.
gcExternalVariable::~gcExternalVariable() {
    *(void **)((char *)this + 4) = gcExternalVariablevirtualtable;
}
