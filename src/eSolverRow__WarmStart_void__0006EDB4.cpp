// eSolverRow::WarmStart(void), 0x0006edb4, 248B, leaf.
//   self = this
//   Body a: P = this->[0x8c]
//           sub  = P->[0x94]              (urbBase)
//           desc = sub->[4] + 0xC0        (short off at [0], fn ptr at [4])
//           self = sub->[4] + *(short*)desc
//           q0 = scale(this->quad[0x00], +this->[0x80])  (VFPU vscl.t -> sp+0x00)
//           q1 = scale(this->quad[0x20], +this->[0x80])  (VFPU vscl.t -> sp+0x10)
//           desc->fn(self, P->[0x9a], &q0, &q1)
//   Body b: B = this->[0x90]; if B != 0:
//           P2   = B->[0x94]
//           desc2= P2->[4] + 0xC0
//           self2= P2->[4] + *(short*)desc2
//           q2 = scale(this->quad[0x10], -this->[0x80])  -> sp+0x20
//           q3 = scale(this->quad[0x30], -this->[0x80])  -> sp+0x30
//           desc2->fn(self2, B->[0x9a], &q2, &q3)
//
// The mtv/lv.q/vscl.t/sv.q sequence (per-element scale of a packed triple by a
// scalar) has no C equivalent and is emitted as inline asm around a real C++
// body that does the pointer chasing and the two indirect calls. Modeled on the
// matched sibling eSolverRow::ApplyUnembedImpulse (0x0006ecd8).
//
// ODR-WARNING: minimal local declarations only. Do not include shared headers;
// this transition-zone method is declared locally to avoid perturbing other TUs.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct urbDesc {
    short off;
    short pad;
    void (*fn)(void *self, unsigned char arg, void *vec0, void *vec1);
};

struct urbBase {
    char pad0[4];
    urbDesc *sub;
};

class eSolverRow {
public:
    void WarmStart();
};

struct eSolverRowQuad {
    SceULong128 v;
};

#pragma control sched=1

void eSolverRow::WarmStart() {
    eSolverRowQuad q[4];
    __asm__ volatile("" ::: "memory");

    urbBase *body_a = *(urbBase **)((char *)this + 0x8c);
    {
        urbBase *Pa = *(urbBase **)((char *)body_a + 0x94);
        urbDesc *desc = (urbDesc *)((char *)Pa->sub + 0xC0);
        void *self = (char *)Pa + desc->off;
        unsigned char arg = *(unsigned char *)((char *)body_a + 0x9a);
        float *sp80 = *(float **)((char *)this + 0x80);
        float s = *sp80;
        __asm__ volatile(
            "mtv %1, S100\n"
            "lv.q C120, 0(%2)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, %0\n"
            : "=m"(q[0])
            : "f"(s), "r"(this)
            : "memory"
        );
        float s2 = *sp80;
        __asm__ volatile(
            "mtv %1, S100\n"
            "lv.q C120, 0x20(%2)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0x10(%3)\n"
            : "=m"(q[1])
            : "f"(s2), "r"(this), "r"(&q[0])
            : "memory"
        );
        desc->fn(self, arg, &q[0], &q[1]);
    }

    urbBase *Pb = *(urbBase **)((char *)this + 0x90);
    if (Pb != 0) {
        urbBase *P2 = *(urbBase **)((char *)Pb + 0x94);
        urbDesc *desc2 = (urbDesc *)((char *)P2->sub + 0xC0);
        void *self2 = (char *)P2 + desc2->off;
        unsigned char arg2 = *(unsigned char *)((char *)Pb + 0x9a);
        float *bp80 = *(float **)((char *)this + 0x80);
        float ns = -*bp80;
        __asm__ volatile(
            "mtv %1, S100\n"
            "lv.q C120, 0x10(%2)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0x20(%3)\n"
            : "=m"(q[2])
            : "f"(ns), "r"(this), "r"(&q[0])
            : "memory"
        );
        void *q2 = &q[2];
        __asm__ volatile("" : : "r"(q2) : "memory");
        float ns2 = -*bp80;
        __asm__ volatile(
            "mtv %1, S100\n"
            "lv.q C120, 0x30(%2)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0x30(%3)\n"
            : "=m"(q[3])
            : "f"(ns2), "r"(this), "r"(&q[0])
            : "memory"
        );
        desc2->fn(self2, arg2, &q[2], &q[3]);
    }
}

#pragma control sched=2
