#pragma once

typedef unsigned int SceULong128 __attribute__((mode(TI)));

// mQuat: quaternion rotation (16 bytes)
// Layout: 4 floats (x, y, z, w) packed in a quadword
// Confirmed from mQuat::Mult (0x001a2f38): lwc1 at offsets 0, 4, 8, 12
struct mQuat {
    union {
        SceULong128 qw;
        struct {
            float x, y, z, w;
        };
    };

    mQuat() {}
    mQuat(const mQuat &);
    void Mult(const mQuat &, const mQuat &);
};
