// eDynamicFluidTemplate.cpp — decompiled from eAll_psp.obj
// Parent: cObject.
//
// Functions:
//   0x002051ec  eDynamicFluidTemplate::AssignCopy(const cBase *)        (112B)
//   0x0020525c  eDynamicFluidTemplate::New(cMemPool *, cBase *) static  (124B)

class cBase;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eDynamicFluidTemplate : public cObject {
public:
    eDynamicFluidTemplate(cBase *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void eDynamicFluidTemplate__eDynamicFluidTemplate_cBaseptr(void *self, cBase *parent);
}

// ── eDynamicFluidTemplate::AssignCopy(const cBase *) @ 0x002051ec ──
#pragma control sched=1
void eDynamicFluidTemplate::AssignCopy(const cBase *base) {
    eDynamicFluidTemplate *src = dcast<eDynamicFluidTemplate>(base);
    cObject::operator=(*src);
    *(float *)((char *)this + 0x44) = *(float *)((char *)src + 0x44);
    *(float *)((char *)this + 0x48) = *(float *)((char *)src + 0x48);
    *(float *)((char *)this + 0x4C) = *(float *)((char *)src + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)src + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)src + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)src + 0x58);
}
#pragma control sched=2

// ── eDynamicFluidTemplate::New(cMemPool *, cBase *) static @ 0x0020525c ──
#pragma control sched=1
cBase *eDynamicFluidTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicFluidTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDynamicFluidTemplate *obj = (eDynamicFluidTemplate *)rec->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        eDynamicFluidTemplate__eDynamicFluidTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
