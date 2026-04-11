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
    ".word 0xd8a70000\n"         // lv.q C130, 0($a1) - load direction
    ".word 0xd8c60020\n"         // lv.q C120, 0x20($a2) - load ocs.forward
    ".word 0x64868704\n"         // vdot.t S100, C130, C120
    ".word 0x48680004\n"         // mfv $t0, S100
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
    ".word 0x48e80004\n"         // mtv $t0, S100
    ".word 0xd8c70030\n"         // lv.q C130, 0x30($a2) - ocs.position
    ".word 0x65048606\n"         // vscl.t C120, C120, S100 - forward * halfLen
    ".word 0x60068706\n"         // vadd.t C120, C130, C120 - pos + forward*halfLen
    ".word 0xf8e60000\n"         // sv.q C120, 0($a3) - store intermediate result
    "lwc1 $f13, 128($a0)\n"     // f13 = this->radius (0x80)
    "mfc1 $a0, $f13\n"          // a0 = radius bits
    ".word 0x48e40004\n"         // mtv $a0, S100
    ".word 0xd8a70000\n"         // lv.q C130, 0($a1) - reload direction
    ".word 0x65048707\n"         // vscl.t C130, C130, S100 - dir * radius
    ".word 0x60078606\n"         // vadd.t C120, C120, C130 - result + dir*radius
    ".word 0x03e00008\n"         // jr $ra
    ".word 0xf8e60000\n"         // sv.q C120, 0($a3) - store final result (delay)
    ".set reorder\n"
);

// eSimulatedController::GetVelocity(int, const mVec3 &, mVec3 *) const
// Address: 0x0006c490, Size: 180B, Obj: eAll_psp.obj
//
// Looks up a body entry from this->bodies (stride 0x30). If the entry has both
// pointer fields valid (offsets 0x20 and 0x24), computes velocity from angular
// velocity cross (pos - origin) + linear velocity. Otherwise outputs zero.

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fUeSimulatedControllerLGetVelocityiRC6FmVec3P6FmVec3K\n"
    "__0fUeSimulatedControllerLGetVelocityiRC6FmVec3P6FmVec3K:\n"
    "addiu $sp, $sp, -48\n"
    "lw $a0, 56($a0)\n"             // a0 = this->bodies (offset 0x38)
    "sll $a1, $a1, 4\n"             // a1 = index * 16
    "addu $t0, $a1, $a1\n"          // t0 = index * 32
    "addu $a1, $a1, $t0\n"          // a1 = index * 48 (0x30)
    "addu $a1, $a0, $a1\n"          // a1 = bodies + index*48 = entry
    "lw $t0, 32($a1)\n"             // t0 = entry->ptr0 (offset 0x20)
    "beqz $t0, 1f\n"                // if ptr0 == 0, goto label1
    ".word 0x34040000\n"             // li $a0, 0 (ori encoding)
    "lw $t0, 36($a1)\n"             // t0 = entry->ptr1 (offset 0x24)
    "beqz $t0, 2f\n"                // if ptr1 == 0, goto label2
    "andi $a0, $a0, 0xff\n"         // delay: valid &= 0xff
    ".word 0x34040001\n"             // li $a0, 1 (ori encoding)
    "1:\n"
    "andi $a0, $a0, 0xff\n"         // valid &= 0xff
    "2:\n"
    "bnel $a0, $0, 3f\n"            // if valid != 0, branch likely
    "lw $a0, 32($a1)\n"             // delay (likely): a0 = entry->ptr0
    // Zero path: output zero vector
    "mtc1 $0, $f12\n"               // f12 = 0.0
    "mfc1 $a0, $f12\n"
    "mfc1 $a1, $f12\n"
    "mfc1 $a2, $f12\n"
    ".word 0x48e40006\n"             // mtv $a0, S120
    ".word 0x48e50026\n"             // mtv $a1, S121
    ".word 0x48e60046\n"             // mtv $a2, S122
    ".word 0xf8e60000\n"             // sv.q C120, 0($a3)
    ".word 0x03e00008\n"             // jr $ra
    "addiu $sp, $sp, 48\n"          // delay: epilogue
    // Valid path: compute velocity
    "3:\n"
    ".word 0xd8860050\n"             // lv.q C120, 0x50($a0) - angular velocity
    ".word 0xd8800000\n"             // lv.q C000, 0x00($a0) - matrix row 0
    ".word 0xd8810010\n"             // lv.q C010, 0x10($a0) - matrix row 1
    ".word 0xd8820020\n"             // lv.q C020, 0x20($a0) - matrix row 2
    ".word 0xd8830030\n"             // lv.q C030, 0x30($a0) - matrix row 3
    ".word 0xf106a007\n"             // vtfm3.t C130, E000, C120 - transform angular vel
    ".word 0xfba70000\n"             // sv.q C130, 0($sp) - save transformed angular vel
    ".word 0xd8c60000\n"             // lv.q C120, 0($a2) - load position
    ".word 0xd8880030\n"             // lv.q C200, 0x30($a0) - load origin
    ".word 0x60888606\n"             // vsub.t C120, C120, C200 - pos - origin
    ".word 0xfba60010\n"             // sv.q C120, 0x10($sp)
    ".word 0xdba80020\n"             // lv.q C200, 0x20($sp)
    ".word 0xf2868708\n"             // vcrsp.t C200, C130, C120 - cross product
    ".word 0xfba80020\n"             // sv.q C200, 0x20($sp)
    ".word 0xd8860040\n"             // lv.q C120, 0x40($a0) - load linear velocity
    ".word 0x60088606\n"             // vadd.t C120, C120, C200 - linvel + cross
    ".word 0xf8e60000\n"             // sv.q C120, 0($a3) - store result
    ".word 0x03e00008\n"             // jr $ra
    "addiu $sp, $sp, 48\n"          // delay: epilogue
    ".set reorder\n"
);
