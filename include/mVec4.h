#ifndef MVEC4_H
#define MVEC4_H

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mVec4 {
    SceULong128 qw;
    mVec4() {}
    mVec4(const mVec4 &);
};

#endif
