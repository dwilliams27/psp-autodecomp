// ODR-WARNING: split translation unit local redeclarations for targeted method only.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 q;
};

struct mCollideHit {
    SceULong128 point;   // 0x00
    SceULong128 _gap;    // 0x10
    SceULong128 normal;  // 0x20
    float t;             // 0x30
};

class mCollideInfo;
class eCollisionInfo;

struct mSphere {
    char _pad[0x10];
    int Collide(const mCollideInfo &info, const mSphere &other,
                mCollideHit *hit) const;
};

class eMultiSphereShape {
public:
    char _pad[0x88];
    int count;          // 0x88
    char _pad2[0x4];    // 0x8c
    mSphere spheres[1]; // 0x90
    int CastSphere(const mSphere &sphere, const mCollideInfo &info,
                   const eCollisionInfo &cinfo, mVec3 *outPoint,
                   mVec3 *outNormal, float *outT) const;
};

#pragma control sched=1
int eMultiSphereShape::CastSphere(const mSphere &sphere, const mCollideInfo &info,
                                  const eCollisionInfo &cinfo, mVec3 *outPoint,
                                  mVec3 *outNormal, float *outT) const {
    int hit = 0;
    __asm__ volatile("" ::: "memory");
    *outT = 3.4028235e38f;

    int i = 0;
    if (i < this->count) {
        const mSphere *s = this->spheres;
        do {
            mCollideHit ch;
            if (sphere.Collide(info, *s, &ch) && ch.t < *outT) {
                *outT = ch.t;
                ((mVec3 *)outPoint)->q = ch.point;
                ((mVec3 *)outNormal)->q = ch.normal;
                hit = 1;
            }
            ++i;
            ++s;
        } while (i < this->count);
    }
    return hit;
}
