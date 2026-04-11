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
    ".word 0x34040000\n"             // li $4, 0 (ori encoding)
    "lw $8, 0($5)\n"                // t0 = entry[0]
    "beqz $8, 1f\n"                 // if entry[0] == 0, goto label1
    "mtc1 $0, $f12\n"               // delay: f12 = 0.0f
    "lw $8, 4($5)\n"                // t0 = entry[4]
    "beqz $8, 2f\n"                 // if entry[4] == 0, goto label2
    "andi $4, $4, 0xff\n"           // delay: valid &= 0xff
    ".word 0x34040001\n"             // li $4, 1 (ori encoding)
    "1:\n"
    "andi $4, $4, 0xff\n"           // valid &= 0xff
    "2:\n"
    "bnel $4, $0, 3f\n"             // if valid, branch (likely)
    "lw $4, 0($5)\n"                // delay (likely): a0 = obj ptr
    // Invalid path: zero both outVel and outAngVel
    "mfc1 $4, $f12\n"
    "mfc1 $5, $f12\n"
    "mfc1 $8, $f12\n"
    ".word 0x48e40006\n"             // mtv $a0, S120
    ".word 0x48e50026\n"             // mtv $a1, S121
    ".word 0x48e80046\n"             // mtv $t0, S122
    ".word 0xf8c60000\n"             // sv.q C120, 0($a2) - zero outVel
    "mfc1 $4, $f12\n"
    "mfc1 $5, $f12\n"
    "mfc1 $6, $f12\n"
    ".word 0x48e40006\n"             // mtv $a0, S120
    ".word 0x48e50026\n"             // mtv $a1, S121
    ".word 0x48e60046\n"             // mtv $a2, S122
    ".word 0x03e00008\n"             // jr $ra
    ".word 0xf8e60000\n"             // sv.q C120, 0($a3) - zero outAngVel
    // Valid path: copy velocity, zero angular velocity
    "3:\n"
    ".word 0xd8860040\n"             // lv.q C120, 0x40($a0) - load velocity
    ".word 0xf8c60000\n"             // sv.q C120, 0($a2) - store to outVel
    "mfc1 $4, $f12\n"
    "mfc1 $5, $f12\n"
    "mfc1 $6, $f12\n"
    ".word 0x48e40006\n"             // mtv $a0, S120
    ".word 0x48e50026\n"             // mtv $a1, S121
    ".word 0x48e60046\n"             // mtv $a2, S122
    ".word 0x03e00008\n"             // jr $ra
    ".word 0xf8e60000\n"             // sv.q C120, 0($a3) - zero outAngVel
    ".set reorder\n"
);
