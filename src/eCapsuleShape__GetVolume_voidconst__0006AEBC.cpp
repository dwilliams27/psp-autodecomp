#include "eCapsuleShape.h"

// Volume = pi*r^2*(2h) + (4/3)*pi*r^3  (capsule = cylinder + sphere caps).
// sched=1 + the barrier before the height load reproduce SNC's serial FPU
// schedule and delay-slot fill. The radius local name (v49116) is load-bearing:
// it sets the allocator tie-break that emits the commutative cube multiply as
// `rsq*r` (f13*f12) rather than `r*rsq`; any other name regresses to 2/72.
// Name selected by the register-aware permuter (ADR-013).
#pragma control sched=1
float eCapsuleShape::GetVolume(void) const {
    float v49116 = radius;
    float rsq = v49116 * v49116;
    float a = rsq * 3.1415927f;
    __asm__ volatile("" ::: "memory");
    float h2 = halfHeight * 2.0f;
    float term1 = a * h2;
    float term2 = (rsq * v49116) * 4.1887903f;
    return term1 + term2;
}
