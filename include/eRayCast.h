#pragma once

#include "mVec3.h"

class eWorld;
class eRoom;

// eRayCast: ray cast query structure.
// Constructor (0x1e8a0c) inits from (const mVec3& origin, const mVec3& dir, unsigned int mask, bool flag):
//   +0x00: int = a3 (mask/flags from arg)
//   +0x04: int = 0
//   +0x18: byte = 1 (enabled)
//   +0x19: byte = 0
//   +0x20: float = FLT_MAX (max distance)
//   +0x30: mVec3 = {0,0,0} (16 bytes, zeroed via VFPU)
//   +0x40: mVec3 = {0,0,1.0,0} (16 bytes, direction init)
//   +0x50: int = -1
//   +0x54: int = -1
//   +0x58: int = 0
//   +0x5C: int = 0
//   +0x60: mVec3 copied from origin arg (16 bytes)
//   +0x70: mVec3 computed from origin+dir, then normal stored (16 bytes)
//   +0x80: float (computed from VFPU dot product)
//   +0x90: byte = bool arg (flag)
// Struct size: at least 0x94 (148 bytes)
struct eRayCast {
    int mCollisionMask;         // 0x00
    int mField04;               // 0x04
    char _pad08[0x10];          // 0x08
    unsigned char mEnabled;     // 0x18
    unsigned char mField19;     // 0x19
    char _pad1A[6];             // 0x1A
    float mMaxDist;             // 0x20
    char _pad24[0x0C];          // 0x24
    mVec3 mStartZero;          // 0x30
    mVec3 mDirDefault;         // 0x40
    int mHitIndex0;             // 0x50
    int mHitIndex1;             // 0x54
    int mField58;               // 0x58
    int mField5C;               // 0x5C
    mVec3 mOrigin;             // 0x60
    mVec3 mDirection;          // 0x70
    float mDotProduct;          // 0x80
    char _pad84[0x0C];         // 0x84
    unsigned char mCastFlag;    // 0x90

    eRayCast(const mVec3 &, const mVec3 &, unsigned int, bool);
    void Intersect(const eWorld *, const eRoom *);
};
