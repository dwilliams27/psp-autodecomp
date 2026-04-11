#pragma once

#include "mVec3.h"

struct mCollideHit {
    char _pad00[0x10];      // 0x00
    mVec3 normal;           // 0x10
    mVec3 point;            // 0x20
    float distance;         // 0x30
};
