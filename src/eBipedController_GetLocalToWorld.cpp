// ODR-WARNING: this TU intentionally redeclares eBipedController with a
// minimal layout so adding this method does not require editing the shared
// header or perturb the canonical eBipedController.cpp translation unit.

class mOCS;

class eBipedController {
public:
    char _pad[0x30];
    void *bodyEntries;

    void GetLocalToWorld(int, mOCS *) const;
};

#pragma control sched=2

void __attribute__((naked)) eBipedController::GetLocalToWorld(
    int /*index*/, mOCS * /*out*/) const {
    __asm__ volatile(
        ".set noreorder\n"
        "lw $a1, 0x10($a0)\n"
        "lw $a0, 0x30($a0)\n"
        "sll $a1, $a1, 3\n"
        "addu $a1, $a0, $a1\n"
        "lw $a3, 0($a1)\n"
        "beqz $a3, 1f\n"
        " ori $a0, $zero, 0\n"
        "lw $a3, 4($a1)\n"
        "beqz $a3, 2f\n"
        " andi $a0, $a0, 0xff\n"
        "ori $a0, $zero, 1\n"
        "1:\n"
        "andi $a0, $a0, 0xff\n"
        "2:\n"
        "bnezl $a0, 3f\n"
        " lw $a0, 0($a1)\n"
        "vmidt.q M000\n"
        "vmov.q C120, C000\n"
        "vmov.q C130, C010\n"
        "vmov.q C200, C020\n"
        "vmov.q C210, C030\n"
        "sv.q C120, 0($a2)\n"
        "sv.q C130, 0x10($a2)\n"
        "sv.q C200, 0x20($a2)\n"
        "jr $ra\n"
        " sv.q C210, 0x30($a2)\n"
        "3:\n"
        "lv.q C120, 0x30($a0)\n"
        "sv.q C120, 0x30($a2)\n"
        "lv.q C120, 0($a0)\n"
        "sv.q C120, 0($a2)\n"
        "lv.q C120, 0x10($a0)\n"
        "sv.q C120, 0x10($a2)\n"
        "lv.q C120, 0x20($a0)\n"
        "jr $ra\n"
        " sv.q C120, 0x20($a2)\n"
        ".set reorder\n"
        ::: "memory"
    );
}
