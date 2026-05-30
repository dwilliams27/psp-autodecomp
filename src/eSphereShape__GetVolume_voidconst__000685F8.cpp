#include "eSphereShape.h"

#pragma control sched=1
float eSphereShape::GetVolume(void) const {
    float r = mRadius;
    float cube = r * r;
    cube = cube * r;
    __asm__ volatile("" ::: "memory");
    return cube * 4.1887903f;
}
