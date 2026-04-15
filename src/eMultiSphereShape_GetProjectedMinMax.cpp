// eMultiSphereShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const
// Address: 0x002095e0, Size: 144B, Obj: eAll_psp.obj
//
// Computes projected min/max of a multi-sphere shape along a direction.
// point1 = ocs.position + ocs.forward * halfLen
// point2 = ocs.position + ocs.forward * (-halfLen)
// dot1 = dot(point1, dir), dot2 = dot(point2, dir)
// *outMin = min(dot1 - radius, dot2 - radius)
// *outMax = max(dot1 + radius, dot2 + radius)
//
// Args: a0=this, a1=const mVec3& dir, a2=const mOCS& ocs, a3=float* outMin, t0=float* outMax
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fReMultiSphereShapeSGetProjectedMinMaxRC6FmVec3RC6EmOCSPfTDK\n"
    "__0fReMultiSphereShapeSGetProjectedMinMaxRC6FmVec3RC6EmOCSPfTDK:\n"
    "lwc1 $f12, 132($a0)\n"         // f12 = this->halfLen (0x84)
    "mfc1 $t1, $f12\n"              // t1 = halfLen bits
    "mtv $t1, S100\n"
    "lv.q C120, 0x30($a2)\n"
    "lv.q C130, 0x20($a2)\n"
    "vscl.t C200, C130, S100\n"
    "vadd.t C200, C120, C200\n"
    "lv.q C210, 0($a1)\n"
    "vdot.t S100, C200, C210\n"
    "mfv $a1, S100\n"
    "mtc1 $a1, $f13\n"              // f13 = dot1
    "lwc1 $f14, 128($a0)\n"         // f14 = this->radius (0x80)
    "sub.s $f15, $f13, $f14\n"      // f15 = dot1 - radius (min1)
    "add.s $f13, $f13, $f14\n"      // f13 = dot1 + radius (max1)
    "neg.s $f12, $f12\n"            // f12 = -halfLen
    "mfc1 $a0, $f12\n"              // a0 = -halfLen bits
    "mtv $a0, S100\n"
    "vscl.t C130, C130, S100\n"
    "vadd.t C120, C120, C130\n"
    "vdot.t S100, C120, C210\n"
    "mfv $a0, S100\n"
    "mtc1 $a0, $f16\n"              // f16 = dot2
    "sub.s $f12, $f16, $f14\n"      // f12 = dot2 - radius (min2)
    "add.s $f14, $f16, $f14\n"      // f14 = dot2 + radius (max2)
    "c.lt.s $f15, $f12\n"           // min1 < min2?
    "nop\n"
    ".word 0x45030001\n"             // bc1tl +1 — if true, take min1
    "mov.s $f12, $f15\n"            // delay: f12 = min1 (only if min1 < min2)
    "swc1 $f12, 0($a3)\n"           // *outMin = min(min1, min2)
    "mov.s $f12, $f14\n"            // f12 = max2
    "c.le.s $f13, $f12\n"           // max1 <= max2?
    "nop\n"
    ".word 0x45020001\n"             // bc1fl +1 — if false (max1 > max2), take max1
    "mov.s $f12, $f13\n"            // delay: f12 = max1 (only if max1 > max2)
    "jr $ra\n"
    "swc1 $f12, 0($t0)\n"           // *outMax = max(max1, max2) (delay slot)
    ".set reorder\n"
);
