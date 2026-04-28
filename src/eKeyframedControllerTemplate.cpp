#include "cBase.h"
#include "cFile.h"

class eMesh;

inline void *operator new(unsigned int, void *p) {
    return p;
}

template <class T>
class cHandleT {
public:
    int mIndex;
};

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cBaseArray {
public:
    int mCount;
    cBase *mOwner;
    cBaseArray &operator=(const cBaseArray &);
};

class ePhysicsControllerTemplate {
public:
    ePhysicsControllerTemplate(cBase *);
    ~ePhysicsControllerTemplate();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern char eKeyframedControllerTemplateclassdesc[];

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mIndex;
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class eKeyframedControllerTemplate : public ePhysicsControllerTemplate {
public:
    eKeyframedControllerTemplate(cBase *);
    ~eKeyframedControllerTemplate();
    void SetMesh(cHandleT<eMesh>);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static eKeyframedControllerTemplate *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = (cMemPool *)cMemPool::GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    }
};

// ── SetMesh (already matched) ──
void eKeyframedControllerTemplate::SetMesh(cHandleT<eMesh>) {
}

#pragma control sched=1

// ── AssignCopy (sched=1) ──

void eKeyframedControllerTemplate::AssignCopy(const cBase *base) {
    eKeyframedControllerTemplate *other = dcast<eKeyframedControllerTemplate>(base);
    const cBaseArray &srcArr1 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr1);
    ((cBaseArray *)((char *)this + 16))->operator=(*(const cBaseArray *)((char *)other + 16));
    ((cBaseArray *)((char *)this + 24))->operator=(*(const cBaseArray *)((char *)other + 24));
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 32) = *(cHandle *)((char *)other + 32);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 36) = *(cHandle *)((char *)other + 36);
}

// ── New (sched=2) ──

eKeyframedControllerTemplate *eKeyframedControllerTemplate::New(cMemPool *pool, cBase *parent) {
    eKeyframedControllerTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eKeyframedControllerTemplate *obj =
        (eKeyframedControllerTemplate *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        new (obj) eKeyframedControllerTemplate(parent);
        result = obj;
    }
    return result;
}

// ── Constructor (sched=1) ──

eKeyframedControllerTemplate::eKeyframedControllerTemplate(cBase *b)
    : ePhysicsControllerTemplate(b)
{
    ((void **)this)[1] = eKeyframedControllerTemplateclassdesc;
}

// ── Write (sched=1) ──

void eKeyframedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsControllerTemplate *)this)->Write(file);
    wb.End();
}

// ── Read (sched=1) ──

int eKeyframedControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->ePhysicsControllerTemplate::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── Destructor (sched=1) ──

eKeyframedControllerTemplate::~eKeyframedControllerTemplate() {
    ((void **)this)[1] = eKeyframedControllerTemplateclassdesc;
}
