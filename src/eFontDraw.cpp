typedef int v4sf_t __attribute__((mode(V4SF)));

union mVec4 {
    v4sf_t v;
    struct {
        float x;
        float y;
        float z;
        float w;
    };
};

struct mOCS {
    mVec4 row0;
    mVec4 row1;
    mVec4 row2;
    mVec4 row3;
};

extern mOCS D_00038578;
extern int D_000385B8;

struct eFontDrawShortCursor {
    char pad[0x48];
    short value;
};

class eFontDraw {
public:
    eFontDraw(void);
};

eFontDraw::eFontDraw(void) {
    *(int *)((char *)this + 0x00) = 0;
    *(int *)((char *)this + 0x04) = 0;
    *(int *)((char *)this + 0x08) = 7;
    *(int *)((char *)this + 0x0C) = -1;
    *(int *)((char *)this + 0x10) = 0xFF000000;

    float zero = 0.0f;
    float one = 1.0f;

    *(int *)((char *)this + 0x18) = 0;
    *(float *)((char *)this + 0x14) = 2048.0f;
    *(int *)((char *)this + 0x1C) = 0;
    *(int *)((char *)this + 0x20) = 0;
    *(int *)((char *)this + 0x24) = -1;
    *(int *)((char *)this + 0x28) = 0;
    *(float *)((char *)this + 0x2C) = zero;
    *(float *)((char *)this + 0x30) = zero;
    *(float *)((char *)this + 0x34) = zero;
    *(float *)((char *)this + 0x38) = zero;
    *(float *)((char *)this + 0x3C) = zero;
    *(float *)((char *)this + 0x40) = zero;
    *(int *)((char *)this + 0x44) = 0;

    eFontDrawShortCursor *values = (eFontDrawShortCursor *)this;
    for (int i = 0; i < 4; i++) {
        values->value = 0;
        values = (eFontDrawShortCursor *)((char *)values + 2);
    }

    if (D_000385B8 == 0) {
        D_000385B8 = 1;

        mOCS identity;
        identity.row0.x = one;
        identity.row0.y = zero;
        identity.row0.z = zero;
        identity.row0.w = zero;
        identity.row1.x = zero;
        identity.row1.y = one;
        identity.row1.z = zero;
        identity.row1.w = zero;
        identity.row2.x = zero;
        identity.row2.y = zero;
        identity.row2.z = one;
        identity.row2.w = zero;
        identity.row3.x = zero;
        identity.row3.y = zero;
        identity.row3.z = zero;
        identity.row3.w = zero;
        D_00038578 = identity;
    }

    __asm__ volatile("" ::: "memory");
    mOCS local = D_00038578;
    *(mOCS *)((char *)this + 0x50) = local;

    *(float *)((char *)this + 0x90) = one;
    *(float *)((char *)this + 0x94) = one;
    *(float *)((char *)this + 0x98) = one;
    *(float *)((char *)this + 0x9C) = one;
    *(short *)((char *)this + 0xA0) = 0x2A;
}
