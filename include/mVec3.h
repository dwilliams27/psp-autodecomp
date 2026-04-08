#ifndef MVEC3_H
#define MVEC3_H

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec3 {
    SceULong128 qw;
    mVec3() {}
    mVec3(const mVec3 &);
};

#endif
