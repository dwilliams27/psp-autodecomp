#include "eMeshShape.h"

class cBase;

extern "C" {
    void eShape___ct_eShape_cBaseptr(void *self, cBase *parent);
}

extern char eMeshShapevirtualtable[];

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

eMeshShape::eMeshShape(cBase *parent) {
    eShape___ct_eShape_cBaseptr(this, parent);
    *(void **)((char *)this + 4) = eMeshShapevirtualtable;
    *(int *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0;
    *(int *)((char *)this + 0x88) = 0;
    *(int *)((char *)this + 0x8C) = 0;
    *(int *)((char *)this + 0x90) = 0;
    *(int *)((char *)this + 0x94) = 0;
    *(short *)((char *)this + 0x98) = 0;
}

#pragma control sched=1
eMeshShape *eMeshShape::New(cMemPool *pool, cBase *parent) {
    eMeshShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eMeshShape *obj = (eMeshShape *)rec->fn(base, 0xC0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eMeshShape(parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2
