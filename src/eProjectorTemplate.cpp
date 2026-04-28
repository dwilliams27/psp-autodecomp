#include "cObject.h"

class cBase;
class cMemPool;
class eMaterial;

inline void *operator new(unsigned int, void *p) {
    return p;
}

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eProjectorTemplate : public cObject {
public:
    eProjectorTemplate(cBase *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// -- eProjectorTemplate::eProjectorTemplate(cBase *) @ 0x0007d17c --
#pragma control sched=1
eProjectorTemplate::eProjectorTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x00384B20;
    *(void **)((char *)this + 0x48) = 0;
    *(unsigned char *)((char *)this + 0x4C) = 0;
    *(float *)((char *)this + 0x50) = 45.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x54) = 0.1f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x58) = 4.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x5C) = 1.0f;
    *(float *)((char *)this + 0x60) = 1.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x44) = 20.0f;
    __asm__ volatile("" ::: "memory");
}
#pragma control sched=2

// -- eProjectorTemplate::AssignCopy(const cBase *) @ 0x00214e18 --
#pragma control sched=1
void eProjectorTemplate::AssignCopy(const cBase *base) {
    eProjectorTemplate *other = dcast<eProjectorTemplate>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->operator=(
        *(const cArrayBase<cHandleT<eMaterial> > *)((const char *)other + 0x48));
    *(unsigned char *)((char *)this + 0x4C) = *(const unsigned char *)((const char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(const float *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((const char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((const char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(const float *)((const char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(const float *)((const char *)other + 0x60);
}
#pragma control sched=2

// -- eProjectorTemplate::New(cMemPool *, cBase *) static @ 0x00214e9c --
#pragma control sched=1
cBase *eProjectorTemplate::New(cMemPool *pool, cBase *parent) {
    eProjectorTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eProjectorTemplate *obj = (eProjectorTemplate *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        new (obj) eProjectorTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
