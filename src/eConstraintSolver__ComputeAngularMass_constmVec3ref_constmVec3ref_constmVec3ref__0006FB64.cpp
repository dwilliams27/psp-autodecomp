// eConstraintSolver::ComputeAngularMass, 0x0006fb64, 56B, leaf.
//   r = dot( c, cross( (cross(a,b) * d_componentwise) , a ) )
// where a=arg0(a1), b=arg1(a2), d=arg2(a3). c is the same as b (loaded twice).
//
// vcrsp.t (3-element cross product), vmul.t (componentwise multiply) and vdot.t
// (3-element dot) have no C/C++ equivalent, so they are inline asm; the result is
// a scalar float returned in $f0 via mfv/mtc1.
//
// SNC keeps working vectors as stack-resident mVec3 locals, so the original emits
// `lv.q C120, 0($sp)` reading the (uninitialized) stack home before the first
// vcrsp.t overwrites it, with a 32-byte stack frame (addiu sp,sp,-32 / +32 in the
// jr delay slot). Reproduced via two 16-byte-aligned v4sf_t stack locals (forces
// the compiler to reserve the 32-byte frame and emit the home load + frame teardown
// in the delay slot). The whole VFPU sequence is one asm block so the compiler emits
// no vmov.q/vzero.q glue between ops. Byte-exact (56B, 0 diff).
//
// ODR-WARNING: minimal local declarations only. Do not include shared headers; this
// transition-zone helper is declared locally to avoid perturbing other TUs.

typedef int v4sf_t __attribute__((mode(V4SF)));

class mVec3;

class eConstraintSolver {
public:
    float ComputeAngularMass(const mVec3 &a, const mVec3 &b, const mVec3 &c);
};

#pragma control sched=1

float eConstraintSolver::ComputeAngularMass(const mVec3 &a, const mVec3 &b,
                                            const mVec3 &c) {
    v4sf_t home __attribute__((aligned(16)));
    v4sf_t spill __attribute__((aligned(16)));
    float result;
    __asm__ volatile(
        "lv.q C120, 0(%4)\n"
        "lv.q C130, 0(%1)\n"
        "lv.q C200, 0(%2)\n"
        "vcrsp.t C120, C130, C200\n"
        "lv.q C200, 0(%3)\n"
        "vmul.t C200, C120, C200\n"
        "vcrsp.t C120, C200, C130\n"
        "lv.q C130, 0(%2)\n"
        "vdot.t S100, C130, C120\n"
        "mfv %0, S100\n"
        : "=r"(result)
        : "r"(&a), "r"(&b), "r"(&c), "r"(&home), "r"(&spill)
    );
    return result;
}

#pragma control sched=2
