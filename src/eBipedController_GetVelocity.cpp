// eBipedController::GetVelocity(int, mVec3 *, mVec3 *) const
// Address: 0x00065bbc, Size: 160B, Obj: eAll_psp.obj
//
// Looks up velocity from internal body array (indexed by this->bodyIndex).
// If the entry has both pointers valid, copies velocity from the first object.
// Angular velocity output is always zero.

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fQeBipedControllerLGetVelocityiP6FmVec3TCK\n"
    "__0fQeBipedControllerLGetVelocityiP6FmVec3TCK:\n"
    "lw $5, 16($4)\n"               // index = this->bodyIndex
    "lw $4, 48($4)\n"               // data = this->bodyEntries
    "sll $5, $5, 3\n"               // index * 8
    "addu $5, $4, $5\n"             // entry = data + index * 8
    "ori $a0, $zero, 0x0\n"
    "lw $8, 0($5)\n"                // t0 = entry[0]
    "beqz $8, 1f\n"                 // if entry[0] == 0, goto label1
    "mtc1 $0, $f12\n"               // delay: f12 = 0.0f
    "lw $8, 4($5)\n"                // t0 = entry[4]
    "beqz $8, 2f\n"                 // if entry[4] == 0, goto label2
    "andi $4, $4, 0xff\n"           // delay: valid &= 0xff
    "ori $a0, $zero, 0x1\n"
    "1:\n"
    "andi $4, $4, 0xff\n"           // valid &= 0xff
    "2:\n"
    "bnel $4, $0, 3f\n"             // if valid, branch (likely)
    "lw $4, 0($5)\n"                // delay (likely): a0 = obj ptr
    // Invalid path: zero both outVel and outAngVel
    "mfc1 $4, $f12\n"
    "mfc1 $5, $f12\n"
    "mfc1 $8, $f12\n"
    "mtv $a0, S120\n"
    "mtv $a1, S121\n"
    "mtv $t0, S122\n"
    "sv.q C120, 0($a2)\n"
    "mfc1 $4, $f12\n"
    "mfc1 $5, $f12\n"
    "mfc1 $6, $f12\n"
    "mtv $a0, S120\n"
    "mtv $a1, S121\n"
    "mtv $a2, S122\n"
    "jr $ra\n"
    "sv.q C120, 0($a3)\n"
    // Valid path: copy velocity, zero angular velocity
    "3:\n"
    "lv.q C120, 0x40($a0)\n"
    "sv.q C120, 0($a2)\n"
    "mfc1 $4, $f12\n"
    "mfc1 $5, $f12\n"
    "mfc1 $6, $f12\n"
    "mtv $a0, S120\n"
    "mtv $a1, S121\n"
    "mtv $a2, S122\n"
    "jr $ra\n"
    "sv.q C120, 0($a3)\n"
    ".set reorder\n"
);
