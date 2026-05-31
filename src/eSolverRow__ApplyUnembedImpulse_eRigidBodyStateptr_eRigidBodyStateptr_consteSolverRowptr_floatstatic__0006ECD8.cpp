// eSolverRow::ApplyUnembedImpulse (static), 0x0006ecd8, 220B, leaf.
//   For body a (if a->flag(0x98) & 2):
//     P    = a->[0x94]
//     sub  = P->[4]
//     desc = sub + 0xD0          (short offset at [0], fn ptr at [4])
//     this = P + *(short*)desc
//     scaled = scale(arg2.quad[0], +impulse)   (VFPU vscl.t to stack home)
//     desc->fn(this, a->[0x9a], &scaled)
//   For body b (if b && b->flag(0x98) & 2): same with arg2.quad[0x10], -impulse.
//
// The VFPU mtv/vscl.t/lv.q/sv.q sequence has no C equivalent (per-element scale
// of a packed triple by a scalar), so it is emitted as inline asm around a real
// C++ body that does the pointer chasing and the indirect call.
//
// ODR-WARNING: minimal local declarations only. Do not include shared headers;
// this transition-zone helper is declared locally to avoid perturbing other TUs.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

class mVec3;
struct eRigidBodyState;
struct eSolverRow;

struct urbDesc {
    short off;
    short pad;
    void (*fn)(void *self, unsigned char arg, void *vec);
};

struct urbBase {
    char pad0[4];
    urbDesc *sub;
};

class eSolverRow {
public:
    static void ApplyUnembedImpulse(eRigidBodyState *a, eRigidBodyState *b,
                                    const eSolverRow *row, float impulse);
};

struct eSolverRowQuad {
    SceULong128 v;
};

#pragma control sched=1

void eSolverRow::ApplyUnembedImpulse(eRigidBodyState *a, eRigidBodyState *b,
                                     const eSolverRow *row, float impulse) {
    eSolverRowQuad q[2];
    __asm__ volatile("" ::: "memory");

    if ((*(unsigned short *)((char *)a + 0x98)) & 2) {
        urbBase *P = *(urbBase **)((char *)a + 0x94);
        urbDesc *desc = (urbDesc *)((char *)P->sub + 0xD0);
        void *self = (char *)P + desc->off;
        unsigned char arg = *(unsigned char *)((char *)a + 0x9a);
        __asm__ volatile(
            "mtv %1, S100\n"
            "lv.q C120, 0(%2)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, %0\n"
            : "=m"(q[0])
            : "f"(impulse), "r"(row)
            : "memory"
        );
        desc->fn(self, arg, &q[0]);
    }
    if (b != 0) {
        if ((*(unsigned short *)((char *)b + 0x98)) & 2) {
            urbBase *P = *(urbBase **)((char *)b + 0x94);
            urbDesc *desc = (urbDesc *)((char *)P->sub + 0xD0);
            void *self = (char *)P + desc->off;
            unsigned char arg = *(unsigned char *)((char *)b + 0x9a);
            float nimpulse = -impulse;
            __asm__ volatile(
                "mtv %1, S100\n"
                "lv.q C120, 0x10(%2)\n"
                "vscl.t C120, C120, S100\n"
                "sv.q C120, 0x10(%3)\n"
                : "=m"(q[1])
                : "f"(nimpulse), "r"(row), "r"(&q[0])
                : "memory"
            );
            desc->fn(self, arg, &q[1]);
        }
    }
}

#pragma control sched=2
