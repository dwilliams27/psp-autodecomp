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

// eMeshShape::Collide(const eShape *, ...) @ 0x0004fdf8
struct CollideDispatchEntry {
    short offset;
    short pad;
    int (*fn)(void *, const eMeshShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *);
};

int eMeshShape::Collide(const eShape *other, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    char *type = ((char **)other)[1];
    CollideDispatchEntry *rec = (CollideDispatchEntry *)(type + 0x110);
    if (rec->fn((char *)other + rec->offset, this, b, a, ocs2, ocs1, info)) {
        int i = 0;
        if (i < *(int *)((char *)info + 0x14)) {
            char *p = (char *)info + 0x20;
            do {
                __asm__ volatile(
                    "lv.q C120, 0(%0)\n"
                    "vneg.t C120, C120\n"
                    "sv.q C120, 0(%0)\n"
                    :: "r"(p) : "memory"
                );
                i++;
                p += 0x40;
            } while (i < *(int *)((char *)info + 0x14));
        }
        return 1;
    }
    return 0;
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
