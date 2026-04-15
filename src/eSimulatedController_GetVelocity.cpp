// eSimulatedController::GetVelocity(int, const mVec3 &, mVec3 *) const
// Address: 0x0006c490, Size: 180B, Obj: eAll_psp.obj
//
// Looks up a body entry from this->bodies (offset 0x38, stride 0x30). If the entry
// has both pointer fields valid (offsets 0x20 and 0x24), computes velocity from
// angular velocity cross (pos - origin) + linear velocity. Otherwise outputs zero.

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
    "ori $a0, $zero, 0x0\n"
    "lw $t0, 36($a1)\n"             // t0 = entry->ptr1 (offset 0x24)
    "beqz $t0, 2f\n"                // if ptr1 == 0, goto label2
    "andi $a0, $a0, 0xff\n"         // delay: valid &= 0xff
    "ori $a0, $zero, 0x1\n"
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
    "mtv $a0, S120\n"
    "mtv $a1, S121\n"
    "mtv $a2, S122\n"
    "sv.q C120, 0($a3)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 48\n"          // delay: epilogue
    // Valid path: compute velocity
    "3:\n"
    "lv.q C120, 0x50($a0)\n"
    "lv.q C000, 0($a0)\n"
    "lv.q C010, 0x10($a0)\n"
    "lv.q C020, 0x20($a0)\n"
    "lv.q C030, 0x30($a0)\n"
    "vtfm3.t C130, E000, C120\n"
    "sv.q C130, 0($sp)\n"
    "lv.q C120, 0($a2)\n"
    "lv.q C200, 0x30($a0)\n"
    "vsub.t C120, C120, C200\n"
    "sv.q C120, 0x10($sp)\n"
    "lv.q C200, 0x20($sp)\n"
    "vcrsp.t C200, C130, C120\n"
    "sv.q C200, 0x20($sp)\n"
    "lv.q C120, 0x40($a0)\n"
    "vadd.t C120, C120, C200\n"
    "sv.q C120, 0($a3)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 48\n"          // delay: epilogue
    ".set reorder\n"
);
