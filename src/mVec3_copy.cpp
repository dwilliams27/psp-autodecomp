#include "mVec3.h"

mVec3::mVec3(const mVec3 &src) {
    __asm__ volatile(
        ".word 0xd8a60000\n"  /* lv.q C600, 0(a1) */
        ".word 0xf8860000\n"  /* sv.q C600, 0(a0) */
    );
}
