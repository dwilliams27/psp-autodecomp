typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mCapsule {
    SceULong128 start;
    SceULong128 end;
    float radius;
};

class eSweptMultiSphereCollisionHandler {
public:
    void Set(const mCapsule &capsule);
};

void eSweptMultiSphereCollisionHandler::Set(const mCapsule &capsule) {
    volatile SceULong128 direction;
    volatile SceULong128 pad;
    __asm__ volatile("" : : "m"(pad));
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "lv.q C120, 0x0($a1)\n"
        "sv.q C120, 0x4960($a0)\n"
        "lv.q C120, 0x10($a1)\n"
        "sv.q C120, 0x4970($a0)\n"
        "lwc1 $f13, 0x20($a1)\n"
        "swc1 $f13, 0x4980($a0)\n"
        "lv.q C120, 0x4970($a0)\n"
        "lv.q C130, 0x4960($a0)\n"
        "vsub.t C120, C120, C130\n"
        "vdot.t S100, C120, C120\n"
        "vcmp.s ez, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        "vpfxs 1, Y, Z, W\n"
        "vcmovt.s S101, S100, 0\n"
        "vscl.t C120, C120, S101\n"
        "sv.q C120, %0\n"
        "mfv $a1, S102\n"
        "mtc1 $a1, $f12\n"
        "lui $a1, 0x4000\n"
        "mtc1 $a1, $f14\n"
        "mul.s $f13, $f13, $f14\n"
        "add.s $f15, $f12, $f13\n"
        "lui $a1, 0x3f19\n"
        "ori $a1, $a1, 0x999a\n"
        "mtc1 $a1, $f16\n"
        "mul.s $f13, $f13, $f16\n"
        "div.s $f13, $f15, $f13\n"
        "trunc.w.s $f13, $f13\n"
        "mfc1 $a2, $f13\n"
        "slti $a3, $a2, 3\n"
        "bnez $a3, 1f\n"
        "ori $a1, $zero, 2\n"
        "move $a1, $a2\n"
        "slti $a2, $a1, 4\n"
        "beqzl $a2, 1f\n"
        "ori $a1, $zero, 4\n"
        "1:\n"
        "sw $a1, 0x4950($a0)\n"
        "move $a2, $a1\n"
        "addiu $a1, $a2, -1\n"
        "mtc1 $a1, $f13\n"
        "cvt.s.w $f13, $f13\n"
        "div.s $f12, $f12, $f13\n"
        "ori $a1, $zero, 0\n"
        "slt $a2, $a1, $a2\n"
        "beqz $a2, 3f\n"
        "nop\n"
        "lv.q C120, %0\n"
        "addiu $a2, $a0, 0x4990\n"
        "ori $a3, $zero, 0\n"
        "addu $a2, $a2, $a3\n"
        "2:\n"
        "mtc1 $a1, $f13\n"
        "cvt.s.w $f13, $f13\n"
        "mul.s $f13, $f12, $f13\n"
        "mfc1 $a3, $f13\n"
        "mtv $a3, S100\n"
        "lv.q C130, 0x4960($a0)\n"
        "vscl.t C200, C120, S100\n"
        "vadd.t C130, C130, C200\n"
        "lwc1 $f14, 0x4980($a0)\n"
        "sv.q C130, 0x0($a2)\n"
        "swc1 $f14, 0xc($a2)\n"
        "addiu $a1, $a1, 1\n"
        "lw $a3, 0x4950($a0)\n"
        "slt $a3, $a1, $a3\n"
        "bnez $a3, 2b\n"
        "addiu $a2, $a2, 0x10\n"
        "3:\n"
        ".set pop\n"
        : "+m"(direction)
        :
        : "memory", "$a1", "$a2", "$a3", "$f12", "$f13", "$f14", "$f15",
          "$f16");
}
