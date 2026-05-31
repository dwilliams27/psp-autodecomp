// eSimulatedState::UpdateSleeping(void) - 0x0006bae4 (eAll_psp.obj)
// Split-TU: eSimulatedState has no shared header; declare locally with only
// the members needed. VFPU vector-length idiom (lv.q/vdot.t/vsqrt.s/mfv)
// has no C equivalent, so it stays inline asm inside a real C++ body, exactly
// as the matched sibling eBoxShape::Read does.

struct mVec3 { float x, y, z, w; };

// shape descriptor located at table+0xA0: short offset, then callback fn.
struct eSimShape {
    short mShapeOffset;   // +0x00 (table+0xA0)
    short _pad02;
    void (*mFn)(void *, unsigned char, int *, int *); // +0x04 (table+0xA4)
};

// dispatch table base; mShape sits at +0xA0.
struct eSimTable {
    char _pad00[0xA0];
    eSimShape mShape;     // 0xA0
};

// object reached via container+0x94: [4] -> table base.
struct eSimDispatch {
    void *_p0;            // 0x00
    eSimTable *mTable;    // 0x04
};

// container at eSimulatedState+0x20.
struct eSimBody {
    char _pad00[0x40];
    mVec3 mLinearVelocity;   // 0x40
    mVec3 mAngularVelocity;  // 0x50
    char _pad60[0x34];
    eSimDispatch *mDispatch; // 0x94
    unsigned short mFlags;   // 0x98
    unsigned char mByte;     // 0x9A
};

class eSimulatedState {
public:
    char _pad00[0x20];
    eSimBody *mBody;     // 0x20
    int _pad24;          // 0x24
    int mSleepCount;     // 0x28

    void UpdateSleeping(void);
};

void eSimulatedState::UpdateSleeping(void) {
    eSimBody *body = this->mBody;
    eSimDispatch *dispatch = body->mDispatch;
    eSimShape *shape = &dispatch->mTable->mShape;
    void *self = (char *)dispatch + shape->mShapeOffset;
    unsigned char b = body->mByte;

    int stack_a[8];
    shape->mFn(self, b, &stack_a[0], &stack_a[4]);

    eSimBody *body2 = this->mBody;

    float lin, ang;
    __asm__ volatile(
        "addiu $a0, %1, 0x40\n"
        "lv.q C120, 0($a0)\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, %0\n"
        : "=f"(lin) : "r"(body2) : "$a0", "memory"
    );
    __asm__ volatile(
        "addiu $a0, %1, 0x50\n"
        "lv.q C120, 0($a0)\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, %0\n"
        : "=f"(ang) : "r"(body2) : "$a0", "memory"
    );

    int count = 0;
    if (lin < 0.05f && ang < 0.02f && !(body2->mFlags & 0x1000)) {
        count = this->mSleepCount + 1;
    }
    this->mSleepCount = count;
}
