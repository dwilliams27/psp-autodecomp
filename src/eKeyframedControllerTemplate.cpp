#include "cBase.h"
#include "cFile.h"
#include "eKeyframedControllerTemplate.h"

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

class cBaseArray {
public:
    int mCount;
    cBase *mOwner;
    cBaseArray &operator=(const cBaseArray &);
};

extern "C" void ePhysicsControllerTemplate_ePhysicsControllerTemplate(void *, cBase *);
extern "C" void ePhysicsControllerTemplate___dtor_ePhysicsControllerTemplate_void(void *, int);
extern "C" void eKeyframedControllerTemplate_eKeyframedControllerTemplate(void *, cBase *);

class ePhysicsControllerTemplate {
public:
    void Write(cFile &) const;
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
        eKeyframedControllerTemplate_eKeyframedControllerTemplate(obj, parent);
        result = obj;
    }
    return result;
}

// ── Constructor (sched=1) ──

eKeyframedControllerTemplate::eKeyframedControllerTemplate(cBase *b) {
    ePhysicsControllerTemplate_ePhysicsControllerTemplate(this, b);
    ((void **)this)[1] = eKeyframedControllerTemplateclassdesc;
}

// ── Write (sched=1) ──

void eKeyframedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsControllerTemplate *)this)->Write(file);
    wb.End();
}

// ── Destructor (sched=1) ──

extern "C" void eKeyframedControllerTemplate___dtor_eKeyframedControllerTemplate_void(
    void *self, int flags)
{
    if (self != 0) {
        ((void **)self)[1] = eKeyframedControllerTemplateclassdesc;
        ePhysicsControllerTemplate___dtor_ePhysicsControllerTemplate_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            __asm__ volatile("" ::: "memory");
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}
