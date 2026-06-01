// gcCamera::LookFromPoint(cTimeValue, const mVec3 &, const mVec3 &) @ 0x000f8fd8
// Symbol: __0fIgcCameraNLookFromPoint6KcTimeValueRC6FmVec3TC
// Split-TU: gcCamera declared locally to avoid touching shared headers.

class cTimeValue {
public:
    int mTime;
    cTimeValue() {}
    cTimeValue(int t) : mTime(t) {}
};

template <class T>
class cHandleT {
public:
    unsigned int mIndex;
};

class gcEntity;

class mVec3 {
public:
    float x, y, z, w;
};

class gcCamera {
public:
    void LookFromPoint(cTimeValue, const mVec3 &, const mVec3 &);
};

// Sibling callee in same TU group. Relocation masked by compare_func.
extern "C" void *gcCamera_NewState(void *, int, cTimeValue, cHandleT<gcEntity>)
    asm("__0fIgcCameraINewState6Q2_6gcCamera6gcMode6KcTimeValue9cHandleT16gcEntity");

void gcCamera::LookFromPoint(cTimeValue dt, const mVec3 &from, const mVec3 &at)
{
    // State block base: this + this->[0x20]*0x240 - 0x210
    char *state = (char *)this
        + (*(unsigned int *)((char *)this + 0x20)) * 0x240 - 0x210;

    cHandleT<gcEntity> handle;
    int mask;

    if (dt.mTime < ((int *)state)[0x15]) goto makeNew;
    if (((int *)state)[0] != 1) goto makeNew;
    __asm__ volatile(
        "lv.q C120, 0x10(%1)\n\t"
        "lv.q C130, 0(%2)\n\t"
        "vcmp.t eq, C120, C130\n\t"
        "vnop\n\t"
        "mfvc %0, $131"
        : "=r"(mask) : "r"(state), "r"(&from));
    if (!(((mask & 0x20) != 0) & 0xff)) goto makeNew;
    __asm__ volatile(
        "lv.q C120, 0x20(%1)\n\t"
        "lv.q C130, 0(%2)\n\t"
        "vcmp.t eq, C120, C130\n\t"
        "vnop\n\t"
        "mfvc %0, $131"
        : "=r"(mask) : "r"(state), "r"(&at));
    if (((mask & 0x20) != 0) & 0xff) return;

makeNew:
    handle.mIndex = 0;
    void *st = gcCamera_NewState(this, 1, dt, handle);
    __asm__ volatile(
        "lv.q C120, 0(%0)\n\t"
        "sv.q C120, 0x10(%1)\n\t"
        "lv.q C120, 0(%2)\n\t"
        "sv.q C120, 0x20(%1)"
        :: "r"(&from), "r"(st), "r"(&at) : "memory");
}
