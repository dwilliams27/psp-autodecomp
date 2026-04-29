#include "eMeshShape.h"

class cBase;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" {
    void eShape___ct_eShape_cBaseptr(void *self, cBase *parent);
}

extern char eMeshShapevirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046A04;

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

const cType *eMeshShape::GetType(void) const {
    if (D_00046A04 == 0) {
        if (D_00040FE4 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00040FE4 = cType::InitializeType(
                0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00040FE4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x1F48A4;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046A04 = cType::InitializeType(
            0, 0, 0x22C, parentType, factory, 0, 0, 0);
    }
    return D_00046A04;
}
#pragma control sched=2
