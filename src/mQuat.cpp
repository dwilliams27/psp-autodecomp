#include "mQuat.h"

void mQuat::Mult(const mQuat &a, const mQuat &b) {
    mQuat result;
    result.x = b.w * a.x + b.x * a.w + b.y * a.z - b.z * a.y;
    result.y = b.w * a.y - b.x * a.z + b.y * a.w + b.z * a.x;
    result.z = b.w * a.z + b.x * a.y - b.y * a.x + b.z * a.w;
    result.w = b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z;
    *this = result;
}
