#include "eShape.h"

class mVec3;
class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eConvexHullShape : public eShape {
public:
    eConvexHullShape(cBase *);

    void GetInertialTensor(float, mVec3 *) const;
    void GetInertialTensorAndVolume(float, mVec3 *, float *) const;
    float GetVolume(void) const;

    static eConvexHullShape *New(cMemPool *, cBase *);
};

#pragma control sched=1

// eConvexHullShape::eConvexHullShape(cBase *) @ 0x000707d4
eConvexHullShape::eConvexHullShape(cBase *parent) : eShape(parent) {
    *(void **)((char *)this + 4) = (void *)0x383898;
    *(int *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0;
    *(int *)((char *)this + 0x88) = 0;
    *(int *)((char *)this + 0x8C) = 0;
    *(int *)((char *)this + 0x90) = 0;
    *(int *)((char *)this + 0x94) = 0;
    *(int *)((char *)this + 0x98) = 0;
    *(int *)((char *)this + 0x9C) = 0;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0xA0) = 0.0f;
}

// eConvexHullShape::GetVolume(void) const @ 0x00071688
float eConvexHullShape::GetVolume(void) const {
    float tmp[4];
    if (*(float *)((char *)this + 0xA0) == 0.0f) {
        float mass = 1.0f;
        __asm__ volatile("" : "+f"(mass));
        GetInertialTensorAndVolume(mass, (mVec3 *)tmp, (float *)((char *)this + 0xA0));
    }
    return *(float *)((char *)this + 0xA0);
}

// eConvexHullShape::GetInertialTensor(float, mVec3 *) const @ 0x00071640
// Trivial wrapper around GetInertialTensorAndVolume that conditionally
// caches the volume back into this+0xA0 if it's currently zero.
void eConvexHullShape::GetInertialTensor(float mass, mVec3 *out) const {
    float vol;
    GetInertialTensorAndVolume(mass, out, &vol);
    if (*(float *)((char *)this + 0xA0) == 0.0f) {
        *(float *)((char *)this + 0xA0) = vol;
    }
}

#pragma control sched=2

// eConvexHullShape::New(cMemPool *, cBase *) static @ 0x0020b024
#pragma control sched=1
eConvexHullShape *eConvexHullShape::New(cMemPool *pool, cBase *parent) {
    eConvexHullShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    void *(*fn)(void *, int, int, int, int) = rec->fn;
    eConvexHullShape *obj = (eConvexHullShape *)fn(base, 0xB0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eConvexHullShape(parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2
