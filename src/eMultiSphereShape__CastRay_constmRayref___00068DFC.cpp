// eMultiSphereShape::CastRay(const mRay &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const
// @ 0x00068dfc
// Split-TU local redeclarations for the targeted method only (ODR-warning).

#pragma control sched=1

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 q;
};

struct mSphere {
    SceULong128 qw;
};

struct mRayVolHit {
    SceULong128 point;   // 0x00
    SceULong128 normal;  // 0x10
    float t;             // 0x20
};

class eCollisionInfo;

class mRay {
public:
    int Intersect(const mSphere &sph, mRayVolHit *hit, bool *embedded) const;
};

class eMultiSphereShape {
public:
    char _pad[0x88];
    int mCount;            // 0x88
    char _pad2[0x90 - 0x8c];
    mSphere mSpheres[1];   // 0x90
    int CastRay(const mRay &ray, const eCollisionInfo &info,
                mVec3 *outPoint, mVec3 *outNormal, float *outT) const;
};

int eMultiSphereShape::CastRay(const mRay &ray, const eCollisionInfo &info,
                               mVec3 *outPoint, mVec3 *outNormal, float *outT) const {
    int hitAny = 0;
    __asm__ volatile("" ::: "memory");
    *outT = 3.4028234663852886e38f;
    int i = 0;
    if (i < this->mCount) {
        const mSphere *sph = this->mSpheres;
        mRayVolHit hit;
        do {
            bool embedded = false;
            if (ray.Intersect(*sph, &hit, &embedded) && hit.t < *outT) {
                *outT = hit.t;
                outPoint->q = hit.point;
                outNormal->q = hit.normal;
                hitAny = 1;
            }
            i++;
            sph++;
        } while (i < this->mCount);
    }
    return hitAny;
}
