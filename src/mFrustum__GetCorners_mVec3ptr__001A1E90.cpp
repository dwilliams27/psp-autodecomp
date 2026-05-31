// mFrustum::GetCorners(mVec3 *) @ 0x001a1e90
// Computes the 8 frustum corners as intersections of triples of clip planes.
// Split-TU: GetCorners and mPlane::Intersection are not declared in the
// shared headers, so the class and the static helper are declared locally.

#include "mVec3.h"

struct mPlane {
    float x, y, z, w;
};

struct mFrustum {
    mPlane planes[6];
    void GetCorners(mVec3 *corners);
};

// External static helper: intersection point of three planes.
// (mPlane::Intersection static; declared as a free function so the call
//  becomes an undefined external reference — the relocation is masked.)
void Intersection(const mPlane &, const mPlane &, const mPlane &, mVec3 *);

void mFrustum::GetCorners(mVec3 *corners) {
    Intersection(planes[0], planes[4], planes[2], &corners[2]);
    Intersection(planes[0], planes[4], planes[3], &corners[3]);
    Intersection(planes[0], planes[5], planes[2], &corners[0]);
    Intersection(planes[0], planes[5], planes[3], &corners[1]);
    Intersection(planes[1], planes[4], planes[2], &corners[6]);
    Intersection(planes[1], planes[4], planes[3], &corners[7]);
    Intersection(planes[1], planes[5], planes[2], &corners[4]);
    Intersection(planes[1], planes[5], planes[3], &corners[5]);
}
