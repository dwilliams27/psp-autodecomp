#include "mVec3.h"

mVec3::mVec3(const mVec3 &src) {
    __asm__ volatile(
        "lv.q C120, 0($a1)\n"
        "sv.q C120, 0($a0)\n"
    );
}
