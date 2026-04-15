#pragma once

#include "mPlane.h"

struct mSphere;

// mFrustum: view frustum as 6 clip planes (96 bytes, 0x60)
// Confirmed from mFrustum::GetOrtho (0x001a19cc): reads float at offsets
//   0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C — the d component of each plane
//   Plane stepping is 0x10 (16 bytes each)
// Confirmed from mFrustum::Clip (0x001a1d3c): loops from this to this+0x60,
//   stepping by 0x10, accessing quadword at +0x00 and float at +0x0C per plane
// Each plane is (nx, ny, nz, d) stored as a quadword
struct mFrustum {
    mPlane planes[6];  // 6 planes, each 16 bytes: (nx, ny, nz, d)

    void GetOrtho(float *, float *, float *, float *, float *, float *) const;
    void GetPerspective(float *, float *, float *, float *, float *, float *) const;
    int Clip(const mSphere &, unsigned int) const;
};
