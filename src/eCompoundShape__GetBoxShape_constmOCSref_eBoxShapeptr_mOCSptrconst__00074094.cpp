// eCompoundShape::GetBoxShape(const mOCS &, eBoxShape *, mOCS *) const
// Address: 0x00074094, Size: 80 bytes, obj: eAll_psp.obj
//
// Copies the compound shape's cached box-shape half-extents (this+0xa0) into the
// destination eBoxShape (boxShape+0x80), then builds the destination mOCS:
//   - basis rows copied verbatim from the input mOCS (src)
//   - position = src.position + (transform of this+0x90 by src basis)
//
// The SceULong128 (mode(TI)) copies auto-vectorize to lv.q/sv.q and the final
// store fills the jr $ra delay slot. The vtfm3.t / vadd.t have no C equivalent,
// so they are emitted as inline asm (the only VFPU compute in this function).

#include "eCompoundShape.h"
#include "mOCS.h"

void eCompoundShape::GetBoxShape(const mOCS &src, eBoxShape *boxShape, mOCS *out) const {
    *(SceULong128 *)((char *)boxShape + 0x80) = *(SceULong128 *)((char *)this + 0xa0);

    __asm__ volatile(
        "lv.q   C120, 0x90(%0)\n"
        "lv.q   C000, 0x0(%1)\n"
        "lv.q   C010, 0x10(%1)\n"
        "lv.q   C020, 0x20(%1)\n"
        "lv.q   C030, 0x30(%1)\n"
        "vtfm3.t C130, E000, C120\n"
        :
        : "r"(this), "r"(&src)
        : "memory"
    );

    out->position = src.position;
    out->row0 = src.row0;
    out->row1 = src.row1;
    out->row2 = src.row2;

    SceULong128 p = out->position;
    __asm__ volatile(
        "vadd.t %0, %0, C130\n"
        : "+X"(p)
        :
        : "memory"
    );
    out->position = p;
}
