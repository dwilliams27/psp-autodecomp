#ifndef EHEIGHTMAP_H
#define EHEIGHTMAP_H

#include "eDynamicGeom.h"

class cBase;
class cMemPool;
class cType;
class eCollisionInfo;
class eContactCollector;
class mCollideInfo;
class mRay;
class mSphere;
struct mCollideHit;

class cFile;

class eHeightmap : public eGeom {
public:
    struct _DelRec { short offset; short pad; void (*fn)(void *, void *); };
    eHeightmap(cBase *);
    ~eHeightmap(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        char *allocTable = *(char **)((char *)block + 0x1C);
        _DelRec *rec = (_DelRec *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = (char *)block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
    static eHeightmap *New(cMemPool *, cBase *);
    void PlatformReset(cMemPool *, bool);
    void PlatformFree(void);
    const cType *GetType(void) const;
    int GetSurface(int) const;
    void GetSweptContacts(const eCollisionInfo &, int, const mSphere *, const mCollideInfo *, eContactCollector *) const;
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *, eContactCollector *) const;
    void __attribute__((naked)) CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
    void CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
};

#endif
