#include "eRayCast.h"

eRayCast::eRayCast(const mVec3 &origin, const mVec3 &end, unsigned int mask, bool flag) {
    __asm__ volatile(
        ".word 0x310800ff\n"  // andi $t0, $t0, 0xff
        ".word 0xac870000\n"  // sw $a3, 0($a0)
        ".word 0xac800004\n"  // sw $zero, 4($a0)
        ".word 0x34070001\n"  // li $a3, 1
        ".word 0xa0870018\n"  // sb $a3, 24($a0)
        ".word 0xa0800019\n"  // sb $zero, 25($a0)
        ".word 0x3c077f7f\n"  // lui $a3, 0x7f7f
        ".word 0x34e7ffff\n"  // ori $a3, $a3, 0xffff
        ".word 0x44876000\n"  // mtc1 $a3, $f12
        ".word 0xe48c0020\n"  // swc1 $f12, 32($a0)
        ".word 0x44806000\n"  // mtc1 $zero, $f12
        ".word 0x44076000\n"  // mfc1 $a3, $f12
        ".word 0x44096000\n"  // mfc1 $t1, $f12
        ".word 0x440a6000\n"  // mfc1 $t2, $f12
        ".word 0x48e70006\n"  // mtv $a3, S120
        ".word 0x48e90026\n"  // mtv $t1, S121
        ".word 0x48ea0046\n"  // mtv $t2, S122
        ".word 0xf8860030\n"  // sv.q C120, 0x30($a0)
        ".word 0x44076000\n"  // mfc1 $a3, $f12
        ".word 0x44096000\n"  // mfc1 $t1, $f12
        ".word 0x3c0a3f80\n"  // lui $t2, 0x3f80
        ".word 0x448a6800\n"  // mtc1 $t2, $f13
        ".word 0x440a6800\n"  // mfc1 $t2, $f13
        ".word 0x48e70006\n"  // mtv $a3, S120
        ".word 0x48e90026\n"  // mtv $t1, S121
        ".word 0x48ea0046\n"  // mtv $t2, S122
        ".word 0xf8860040\n"  // sv.q C120, 0x40($a0)
        ".word 0x2407ffff\n"  // li $a3, -1
        ".word 0xac870050\n"  // sw $a3, 80($a0)
        ".word 0xac870054\n"  // sw $a3, 84($a0)
        ".word 0xac800058\n"  // sw $zero, 88($a0)
        ".word 0xac80005c\n"  // sw $zero, 92($a0)
        ".word 0xd8a60000\n"  // lv.q C120, 0x0($a1)
        ".word 0xf8860060\n"  // sv.q C120, 0x60($a0)
        ".word 0xd8c70000\n"  // lv.q C130, 0x0($a2)
        ".word 0x60868706\n"  // vsub.t C120, C130, C120
        ".word 0xf8860070\n"  // sv.q C120, 0x70($a0)
        ".word 0x64868604\n"  // vdot.t S100, C120, C120
        ".word 0x6c000408\n"  // vcmp.s ez, S100
        ".word 0xd0110424\n"  // vrsq.s S101, S100
        ".word 0xd0160444\n"  // vsqrt.s S102, S100
        ".word 0xdc0010e5\n"  // vpfxs 1, Y, Z, W
        ".word 0xd2a00424\n"  // vcmovt.s S101, S100, 0
        ".word 0x65248606\n"  // vscl.t C120, C120, S101
        ".word 0xf8860070\n"  // sv.q C120, 0x70($a0)
        ".word 0x48650044\n"  // mfv $a1, S102
        ".word 0x44856000\n"  // mtc1 $a1, $f12
        ".word 0xe48c0080\n"  // swc1 $f12, 0x80($a0)
        ".word 0xa0880090\n"  // sb $t0, 144($a0)
    );
}
