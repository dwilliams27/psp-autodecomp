// gcEntity::GetVelocity(mVec3 *) const @ 0x001174b4, gcAll_psp.obj
// symbol: __0fIgcEntityLGetVelocityP6FmVec3K
//
// Leaf: out = *body where body = this->[0x1F8] if non-null, else a local
// zero mVec3. SNC builds the zero quad with vzero.t C120 + sv.q to the stack
// local, then copies the selected source quad to *out with lv.q/sv.q.
//
// Split-TU: gcEntity is redeclared locally (header untouched).

#include "mVec3.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class gcEntity {
public:
    void GetVelocity(mVec3 *) const;
};

#pragma control sched=2

void gcEntity::GetVelocity(mVec3 *out) const {
    register const mVec3 *body __asm__("$4") =
        *(const mVec3 *const *)((const char *)this + 0x1F8);
    if (body == 0) {
        mVec3 zero;
        __asm__ volatile(
            "vzero.t C120\n"
            "sv.q C120, 0(%1)\n"
            "or %0, %1, $0\n"
            : "=r"(body) : "r"(&zero) : "memory");
        *(v4sf_t *)&out->qw = *(const v4sf_t *)&body->qw;
        return;
    }
    *(v4sf_t *)&out->qw = *(const v4sf_t *)&body->qw;
}
