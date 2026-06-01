// eTriangleShape::CastRay(const mRay &, const eCollisionInfo &, mVec3 *, mVec3 *, float *) const
// @ 0x0006fd54
// Split-TU local redeclarations for the targeted method only (ODR-warning).

#pragma control sched=1

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 q;
};

struct mTriangle {
    mVec3 v0;   // 0x00
    mVec3 v1;   // 0x10
    mVec3 v2;   // 0x20
    mVec3 normal; // 0x30
    mVec3 edge;   // 0x40
};

struct mRayTriHit {
    mVec3 point;   // 0x00
    mVec3 normal;  // 0x10
    float t;       // 0x20
};

class eCollisionInfo;

class mRay {
public:
    int Intersect(const mTriangle *tri, int a, bool b, mRayTriHit *hit) const;
};

class eTriangleShape {
public:
    char _pad[0x80];
    mVec3 mVerts[3];   // 0x80
    int CastRay(const mRay &ray, const eCollisionInfo &info,
                mVec3 *outPoint, mVec3 *outNormal, float *outT) const;
};

int eTriangleShape::CastRay(const mRay &ray, const eCollisionInfo &info,
                            mVec3 *outPoint, mVec3 *outNormal, float *outT) const {
    __asm__ volatile("" ::: "memory");
    *outT = 3.4028234663852886e38f;

    mTriangle tri;
    tri.v0 = this->mVerts[0];
    tri.v1 = this->mVerts[1];
    tri.v2 = this->mVerts[2];

    mRayTriHit hit;
    if (ray.Intersect(&tri, 1, true, &hit)) {
        outPoint->q = hit.point.q;
        outNormal->q = hit.normal.q;
        __asm__ volatile("" ::: "memory");
        *outT = hit.t;
        return 1;
    }
    return 0;
}
