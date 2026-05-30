// eSolverRow::GetEmbedVelocity (static), 0x0006eb98, 88B, leaf.
//   out = 0
//   if (a->flag(0x98) & 2)            out  = a->embedVel(0x60)
//   if (b && (b->flag(0x98) & 2))     out -= b->embedVel(0x60)
// embedVel is the quad at offset 0x60. Sibling: GetAngularVelocity @ 0x0006eb44.
//
// ODR-WARNING: minimal local declarations only. Do not include shared headers;
// this transition-zone helper is declared locally to avoid perturbing other TUs.

typedef int v4sf_t __attribute__((mode(V4SF)));
typedef unsigned int SceULong128 __attribute__((mode(TI)));

class mVec3;
struct eRigidBodyState;

class eSolverRow {
public:
    static void GetEmbedVelocity(eRigidBodyState *a, eRigidBodyState *b,
                                 const mVec3 &ra, const mVec3 &rb, mVec3 *out);
};

struct eSolverRowQuad {
    SceULong128 v;
};

#pragma control sched=1

void eSolverRow::GetEmbedVelocity(eRigidBodyState *a, eRigidBodyState *b,
                                  const mVec3 & /*ra*/, const mVec3 & /*rb*/,
                                  mVec3 *out) {
    eSolverRowQuad scratch;
    __asm__ volatile(
        "vzero.t C120\n"
        "sv.q C120, %0\n"
        "sv.q C120, %1\n"
        : "=m"(scratch), "=m"(*(SceULong128 *)out)
        :
        : "memory"
    );
    if ((*(unsigned short *)((char *)a + 0x98)) & 2) {
        v4sf_t va = *(v4sf_t *)((char *)a + 0x60);
        *(v4sf_t *)out = va;
    }
    if (b != 0) {
        if ((*(unsigned short *)((char *)b + 0x98)) & 2) {
            v4sf_t r = *(v4sf_t *)out;
            v4sf_t vb = *(v4sf_t *)((char *)b + 0x60);
            __asm__ volatile(
                "vsub.t %0, %0, %1\n"
                : "+X"(r)
                : "X"(vb)
            );
            *(v4sf_t *)out = r;
        }
    }
    (void)scratch;
}

#pragma control sched=2
