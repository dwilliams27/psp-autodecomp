#include "mVec4.h"

mVec4::mVec4(const mVec4 &src) {
    __asm__ volatile(
        ".word 0xd8a60000\n"  /* lv.q C600, 0(a1) */
        ".word 0xf8860000\n"  /* sv.q C600, 0(a0) */
    );
}
