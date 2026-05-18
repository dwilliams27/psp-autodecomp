// ODR-WARNING: this TU intentionally redeclares eBipedController with a
// minimal layout so adding this method does not require editing the shared
// header or perturb the canonical eBipedController.cpp translation unit.

class mVec3;

class eBipedController {
public:
    char _pad[0x30];
    void *bodyEntries;

    void GetInvMass(int, float *, mVec3 *) const;
};

#pragma control sched=2

void __attribute__((naked)) eBipedController::GetInvMass(
    int /*index*/, float * /*invMass*/, mVec3 * /*out*/) const {
    __asm__ volatile(
        ".set noreorder\n"
        "mtc1 $zero, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $t0, $f12\n"
        "mfc1 $t1, $f12\n"
        "mtv $a1, S120\n"
        "mtv $t0, S121\n"
        "mtv $t1, S122\n"
        "sv.q C120, 0($a3)\n"
        "lw $a1, 0x10($a0)\n"
        "lw $a0, 0x30($a0)\n"
        "sll $a1, $a1, 3\n"
        "addu $a1, $a0, $a1\n"
        "lw $t0, 0($a1)\n"
        "beqz $t0, 1f\n"
        " ori $a0, $zero, 0\n"
        "lw $t0, 4($a1)\n"
        "beqz $t0, 2f\n"
        " andi $a0, $a0, 0xff\n"
        "ori $a0, $zero, 1\n"
        "1:\n"
        "andi $a0, $a0, 0xff\n"
        "2:\n"
        "bnezl $a0, 3f\n"
        " lw $a0, 4($a1)\n"
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
        "lwc1 $f12, 0x20($a0)\n"
        "lui $a0, 0x3f80\n"
        "mtc1 $a0, $f13\n"
        "div.s $f12, $f13, $f12\n"
        "jr $ra\n"
        " swc1 $f12, 0($a2)\n"
        ".set reorder\n"
        ::: "memory"
    );
}
