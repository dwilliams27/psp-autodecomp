#include "mBasis.h"
#include "mQuat.h"

void mBasis::SetQuat(const mQuat &q) {
    float y = q.y;
    float z = q.z;
    float two = 2.0f;
    float x = q.x;

    float yy2 = y * two;
    float w = q.w;
    float zz2 = z * two;
    float one = 1.0f;

    float yysq = y * yy2;
    float zzsq = z * zz2;
    float xy2  = x * yy2;
    float xx2  = x * two;
    float xz2  = x * zz2;
    float wz2  = w * zz2;
    float wy2  = w * yy2;
    float yy_zz = yysq + zzsq;
    float xxsq  = x * xx2;

    float r01 = xy2 - wz2;
    float r02 = xz2 + wy2;
    float r00 = one - yy_zz;
    float wx2  = w * xx2;
    float yz2  = y * zz2;

    // Row 0: mfc1 -> mtv -> (interleave add.s) -> sv.q
    __asm__ volatile("mfc1 $a2, %0\n" :: "f"(r01) : "$a2");
    __asm__ volatile("mfc1 $a1, %0\n" :: "f"(r00) : "$a1");
    __asm__ volatile("mfc1 $a3, %0\n" :: "f"(r02) : "$a3");

    __asm__ volatile(
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        :::
    );

    float xx_zz = xxsq + zzsq;

    __asm__ volatile(
        "sv.q C120, 0(%0)\n"
        :: "r"(this)
        : "memory"
    );

    // Row 1
    float r10 = xy2 + wz2;
    float r12 = yz2 - wx2;
    float r11 = one - xx_zz;

    __asm__ volatile("mfc1 $a1, %0\n" :: "f"(r10) : "$a1");
    __asm__ volatile("mfc1 $a3, %0\n" :: "f"(r12) : "$a3");
    __asm__ volatile("mfc1 $a2, %0\n" :: "f"(r11) : "$a2");

    __asm__ volatile(
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        :::
    );

    float xx_yy = xxsq + yysq;

    __asm__ volatile(
        "sv.q C120, 0x10(%0)\n"
        :: "r"(this)
        : "memory"
    );

    // Row 2
    float r20 = xz2 - wy2;
    float r21 = wx2 + yz2;
    float r22 = one - xx_yy;

    __asm__ volatile("mfc1 $a1, %0\n" :: "f"(r20) : "$a1");
    __asm__ volatile("mfc1 $a2, %0\n" :: "f"(r21) : "$a2");
    __asm__ volatile("mfc1 $a3, %0\n" :: "f"(r22) : "$a3");

    __asm__ volatile(
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        ".word 0x03e00008\n"
        "sv.q C120, 0x20(%0)\n"
        :: "r"(this)
        : "memory"
    );
}
