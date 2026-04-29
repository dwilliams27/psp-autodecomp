#include "eShape.h"

class mVec3;
class cBase;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eKeyframedController {
public:
    const cType *GetType(void) const;
};

class eKeyframedControllerTemplate {
public:
    const cType *GetType(void) const;
};

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
    const cType *GetType(void) const;

    static eConvexHullShape *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_000469D8;
extern cType *D_000469F4;
extern cType *D_00046BE0;
extern cType *D_00046BEC;
extern cType *D_00046BF4;

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

// eConvexHullShape::GetType(void) const @ 0x0020b0a0
#pragma control sched=1
const cType *eConvexHullShape::GetType(void) const {
    if (D_00046BE0 == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x20B024;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BE0 = cType::InitializeType(
            0, 0, 0x22E, parentType, factory, 0, 0, 0);
    }
    return D_00046BE0;
}

// eKeyframedController::GetType(void) const @ 0x0020ded8
const cType *eKeyframedController::GetType(void) const {
    if (D_00046BEC == 0) {
        if (D_000469D8 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469D8 = cType::InitializeType(
                0, 0, 0x232, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_000469D8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20DE5C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BEC = cType::InitializeType(
            0, 0, 0x23A, parentType, factory, 0, 0, 0);
    }
    return D_00046BEC;
}

// eKeyframedControllerTemplate::GetType(void) const @ 0x0020e258
const cType *eKeyframedControllerTemplate::GetType(void) const {
    if (D_00046BF4 == 0) {
        if (D_000469F4 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469F4 = cType::InitializeType(
                0, 0, 0x22F, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_000469F4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20E1DC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BF4 = cType::InitializeType(
            0, 0, 0x239, parentType, factory, 0, 0, 2);
    }
    return D_00046BF4;
}
#pragma control sched=2
