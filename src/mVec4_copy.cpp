#include "mVec4.h"

mVec4::mVec4(const mVec4 &src) {
    __asm__ volatile(
        "lv.q C120, 0($a1)\n"
        "sv.q C120, 0($a0)\n"
    );
}
