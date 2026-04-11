#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mRay {
    SceULong128 origin;       // 0x00
    SceULong128 direction;    // 0x10
    float t;                  // 0x20
};
