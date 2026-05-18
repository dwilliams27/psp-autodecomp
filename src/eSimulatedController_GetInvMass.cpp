// ODR-WARNING: this TU intentionally redeclares eSimulatedController with a
// minimal layout so adding this method does not require editing the shared
// header or perturb the canonical eSimulatedController.cpp translation unit.

struct mVec3;

class eSimulatedController {
public:
    char _pad[0x38];
    void *bodyEntries;

    void GetInvMass(int, float *, mVec3 *) const;
};

#pragma control sched=2

void __attribute__((naked)) eSimulatedController::GetInvMass(
    int /*index*/, float * /*invMass*/, mVec3 * /*out*/) const {
    __asm__ volatile(
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
        " lw $a0, 0x24($a1)\n"
        "mtc1 $zero, $f12\n"
        "swc1 $f12, 0($a2)\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "jr $ra\n"
        " sv.q C120, 0($a3)\n"
        "3:\n"
        "lwc1 $f12, 0x44($a0)\n"
        "swc1 $f12, 0($a2)\n"
        "lv.q C120, 0x50($a0)\n"
        "jr $ra\n"
        " sv.q C120, 0($a3)\n"
        ".set reorder\n"
        ::: "memory"
    );
}
