// eCapsuleShape::GetSupport(const mVec3 &, const mOCS &, mVec3 *) const
// Address: 0x0006afbc, Size: 104B, Obj: eAll_psp.obj
//
// Capsule support function: finds the furthest point on the capsule in a given direction.
// result = ocs.position + ocs.forward * (dot(dir, forward) > 0 ? halfLen : -halfLen) + dir * radius
// Uses VFPU for vector ops, FPU for scalar comparison and conditional negation.

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fNeCapsuleShapeKGetSupportRC6FmVec3RC6EmOCSP6FmVec3K\n"
    "__0fNeCapsuleShapeKGetSupportRC6FmVec3RC6EmOCSP6FmVec3K:\n"
    "lv.q C130, 0($a1)\n"
    "lv.q C120, 0x20($a2)\n"
    "vdot.t S100, C130, C120\n"
    "mfv $t0, S100\n"
    "mtc1 $t0, $f13\n"          // f13 = dot result
    "mtc1 $0, $f14\n"           // f14 = 0.0
    "c.le.s $f13, $f14\n"       // dot <= 0?
    "nop\n"
    "bc1f 1f\n"                 // if dot > 0, skip negation
    "lwc1 $f12, 132($a0)\n"     // delay: f12 = this->halfLength (0x84)
    "b 1f\n"                    // unconditional branch
    "neg.s $f12, $f12\n"        // delay: negate halfLength
    "1:\n"
    "mfc1 $t0, $f12\n"          // t0 = halfLength (possibly negated)
    "mtv $t0, S100\n"
    "lv.q C130, 0x30($a2)\n"
    "vscl.t C120, C120, S100\n"
    "vadd.t C120, C130, C120\n"
    "sv.q C120, 0($a3)\n"
    "lwc1 $f13, 128($a0)\n"     // f13 = this->radius (0x80)
    "mfc1 $a0, $f13\n"          // a0 = radius bits
    "mtv $a0, S100\n"
    "lv.q C130, 0($a1)\n"
    "vscl.t C130, C130, S100\n"
    "vadd.t C120, C120, C130\n"
    "jr $ra\n"
    "sv.q C120, 0($a3)\n"
    ".set reorder\n"
);
