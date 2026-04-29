#pragma once

#include "eDynamicGeom.h"

class cFile;
class cMemPool;

class cTimeValue {
public:
    int mTime;
};

class eShadow : public eDynamicGeom {
public:
    struct _DelRec { short offset; short pad; void (*fn)(void *, void *); };

    eShadow(cBase *);
    ~eShadow();

    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        _DelRec *rec = (_DelRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void Reset(cMemPool *, bool);
    void Update(cTimeValue);
};
