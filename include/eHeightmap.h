#ifndef EHEIGHTMAP_H
#define EHEIGHTMAP_H

class cMemPool;
class eCollisionInfo;
class mRay;
struct mCollideHit;

class eHeightmap {
public:
    void PlatformReset(cMemPool *, bool);
    void __attribute__((naked)) CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
};

#endif
