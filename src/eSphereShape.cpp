#include "eSphereShape.h"
#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mVec3.h"
#include "mOCS.h"

class eConvexHullShape;

int eSphereShape::CanSweep(void) const {
    return 1;
}

float eSphereShape::GetVolume(void) const {
    float r = mRadius;
    return r * r * r * 4.1887903f;
}

void eSphereShape::GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const {
    float r = mRadius;
    SceULong128 pos;
    __asm__ volatile(
        "mfc1 $a0, %1\n"
        "mtv $a0, S100\n"
        "lv.q %0, 0x30(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t %0, %0, C130\n"
        : "=X"(pos)
        : "f"(r), "r"(&ocs), "r"(&dir)
        : "$a0"
    );
    *(SceULong128 *)out = pos;
}

void eSphereShape::GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const {
    float dot;
    __asm__ volatile(
        "lv.q C120, 0x30(%1)\n"
        "lv.q C130, 0(%2)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a1, S100\n"
        "mtc1 $a1, %0\n"
        : "=f"(dot)
        : "r"(&ocs), "r"(&dir)
        : "$a1"
    );
    *outMin = dot - mRadius;
    *outMax = dot + mRadius;
}

void eSphereShape::GetInertialTensor(float mass, mVec3 *out) const {
    float v = mass * 0.4f * (mRadius * mRadius);
    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(v));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(v));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(v));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(a), "r"(b), "r"(c), "r"(out)
        : "memory"
    );
}

int eSphereShape::Collide(const eMultiSphereShape *ms, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMultiSphere(*this, *ms, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereCapsule(*this, *shape, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereConvexHull(*this, *shape, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereCompound(*this, *shape, b, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMesh(*this, *shape, b, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereHeightmap(*this, *shape, b, ocs1, ocs2, info);
}
