// eStaticSkyLight::GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const
// Address: 0x0005f388, Size: 184B, Obj: eAll_psp.obj
//
// Computes Lambertian direct lighting: dot(ray.direction, normal) * intensity * color.
// If dot <= 0 (surface faces away), outputs zero vector.
// Heavy VFPU/scalar interleaving for dot product and vector scaling.

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fPeStaticSkyLightOGetDirectLightP6FmVec3RC6FmVec3TCRC6EmRayTCK\n"
    "__0fPeStaticSkyLightOGetDirectLightP6FmVec3RC6FmVec3TCRC6EmRayTCK:\n"
    // Prologue
    "addiu $sp, $sp, -32\n"
    // VFPU: dot3(ray.direction, normal)
    "lv.q C120, 0x10($t0)\n"
    "lv.q C130, 0($a3)\n"
    "vdot.t S100, C120, C130\n"
    "mfv $a2, S100\n"
    // Move dot result to FPU, compare with 0
    "mtc1 $6, $f13\n"
    "mtc1 $0, $f12\n"
    "c.le.s $f13, $f12\n"
    "nop\n"
    "bc1t 1f\n"                  // if dot <= 0, branch to zero path
    "lui $6, 0x3b80\n"          // delay slot: begin 1/255 constant
    // Positive path: convert RGB bytes to floats, scale by dot*intensity
    "ori $6, $6, 0x8081\n"
    "mtc1 $6, $f12\n"           // $f12 = 1/255
    "lbu $6, 74($4)\n"          // R byte (offset 0x4A)
    "mtc1 $6, $f14\n"
    "cvt.s.w $f14, $f14\n"
    "mul.s $f14, $f14, $f12\n"
    "swc1 $f14, 0($sp)\n"
    "lbu $6, 73($4)\n"          // G byte (offset 0x49)
    "mtc1 $6, $f14\n"
    "cvt.s.w $f14, $f14\n"
    "mul.s $f14, $f14, $f12\n"
    "swc1 $f14, 4($sp)\n"
    "lbu $6, 72($4)\n"          // B byte (offset 0x48)
    "mtc1 $6, $f14\n"
    "cvt.s.w $f14, $f14\n"
    "mul.s $f12, $f14, $f12\n"  // reuse $f12 (scale no longer needed)
    "swc1 $f12, 8($sp)\n"
    "lwc1 $f12, 68($4)\n"       // intensity (offset 0x44)
    "mul.s $f12, $f13, $f12\n"  // factor = dot * intensity
    "mfc1 $4, $f12\n"
    // VFPU: scale color by factor, store to outColor
    "mtv $a0, S100\n"
    "lv.q C120, 0($sp)\n"
    "vscl.t C120, C120, S100\n"
    "sv.q C120, 0($a1)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 32\n"
    // Zero path: dot <= 0, output zero vector
    "1:\n"
    "mfc1 $4, $f12\n"
    "mfc1 $6, $f12\n"
    "mfc1 $7, $f12\n"
    "mtv $a0, S120\n"
    "mtv $a2, S121\n"
    "mtv $a3, S122\n"
    "sv.q C120, 0($a1)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 32\n"
    ".set reorder\n"
);
