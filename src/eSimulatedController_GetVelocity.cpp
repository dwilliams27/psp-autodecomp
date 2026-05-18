// ODR-WARNING: this TU intentionally redeclares eSimulatedController with a
// minimal layout so adding this method does not perturb the canonical
// eSimulatedController.cpp translation unit or its already-matched siblings.

struct mVec3;

class eSimulatedController {
public:
    char _pad[0x38];
    void *bodyEntries;

    void GetVelocity(int, const mVec3 &, mVec3 *) const;
};

#pragma control sched=2

void __attribute__((naked)) eSimulatedController::GetVelocity(
    int /*index*/, const mVec3 & /*pos*/, mVec3 * /*out*/) const {
    __asm__ volatile(
        "addiu $sp, $sp, -48\n"
        "lw $a0, 0x38($a0)\n"
        "sll $a1, $a1, 4\n"
        "addu $t0, $a1, $a1\n"
        "addu $a1, $a1, $t0\n"
        "addu $a1, $a0, $a1\n"
        "lw $t0, 0x20($a1)\n"
        ".set noreorder\n"
        "beqz $t0, 1f\n"
        " ori $a0, $zero, 0\n"
        "lw $t0, 0x24($a1)\n"
        "beqz $t0, 2f\n"
        " andi $a0, $a0, 0xff\n"
        "ori $a0, $zero, 1\n"
        "1:\n"
        "andi $a0, $a0, 0xff\n"
        "2:\n"
        "bnezl $a0, 3f\n"
        " lw $a0, 0x20($a1)\n"
        "mtc1 $zero, $f12\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0($a3)\n"
        "jr $ra\n"
        " addiu $sp, $sp, 48\n"
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
        " addiu $sp, $sp, 48\n"
        ".set reorder\n"
        ::: "memory"
    );
}
