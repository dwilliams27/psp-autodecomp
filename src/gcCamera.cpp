// gcCamera.cpp - gcAll_psp.obj

class cBase;

extern void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void gcCamera_gcState_ctor(void *) asm("gcCamera__gcState__gcState_void__000F85FC");
extern "C" void eCamera_ctor(void *) asm("__0oHeCameractv");
extern "C" void gcUI_ctor(void *, cBase *, unsigned int) asm("__0oEgcUIctP6FcBaseUi");
extern "C" void gcDesiredObject_ctor(void *, cBase *) asm("gcDesiredObject__gcDesiredObject_cBaseptr__0011B6F4");
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    asm("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern char gcDesiredCamera_typeinfo_338[] asm("0x338");

class gcCamera {
public:
    gcCamera(void);
    void Reset(void);
};

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);
};

#define GC_CAMERA_ZERO_VEC_SPLIT_HEAD() \
    __asm__ volatile( \
        "mfc1 $a0, %0\n" \
        "mfc1 $a1, %0\n" \
        : : "f"(z) : "$a0", "$a1" \
    )

#define GC_CAMERA_ZERO_VEC_SPLIT_TAIL(off) \
    __asm__ volatile( \
        "mfc1 $a2, %0\n" \
        "mtv $a0, S120\n" \
        "mtv $a1, S121\n" \
        "mtv $a2, S122\n" \
        "sv.q C120, " #off "($s0)\n" \
        : : "f"(z) : "$a2", "memory" \
    )

#define GC_CAMERA_ZERO_VEC(off) \
    __asm__ volatile( \
        "mfc1 $a0, %0\n" \
        "mfc1 $a1, %0\n" \
        "mfc1 $a2, %0\n" \
        "mtv $a0, S120\n" \
        "mtv $a1, S121\n" \
        "mtv $a2, S122\n" \
        "sv.q C120, " #off "($s0)\n" \
        : : "f"(z) : "$a0", "$a1", "$a2", "memory" \
    )

#pragma control sched=2
gcCamera::gcCamera(void) {
    __vec_new((char *)this + 0x30, 6, 0x240, gcCamera_gcState_ctor);

    float z = 0.0f;
    float one = 1.0f;
    *(float *)((char *)this + 0xDBC) = z;
    *(float *)((char *)this + 0xDC0) = z;
    *(float *)((char *)this + 0xDC4) = z;
    *(float *)((char *)this + 0xDC8) = one;

    *(float *)((char *)this + 0xDCC) = one;
    float big = 100000.0f;
    *(float *)((char *)this + 0xDD0) = big;
    *(float *)((char *)this + 0xDD4) = z;
    *(float *)((char *)this + 0xDD8) = z;
    *(float *)((char *)this + 0xDDC) = z;
    *(float *)((char *)this + 0xDE0) = one;
    *(float *)((char *)this + 0xDE4) = one;
    *(float *)((char *)this + 0xDE8) = big;
    *(float *)((char *)this + 0xDEC) = z;
    *(float *)((char *)this + 0xDF0) = z;
    *(float *)((char *)this + 0xDF4) = z;
    *(float *)((char *)this + 0xDF8) = one;
    *(float *)((char *)this + 0xDFC) = one;
    *(float *)((char *)this + 0xE00) = big;
    *(float *)((char *)this + 0xE04) = z;
    *(float *)((char *)this + 0xE08) = z;
    *(float *)((char *)this + 0xE0C) = z;
    *(float *)((char *)this + 0xE10) = one;
    *(float *)((char *)this + 0xE14) = one;
    GC_CAMERA_ZERO_VEC_SPLIT_HEAD();
    *(float *)((char *)this + 0xE18) = big;
    GC_CAMERA_ZERO_VEC_SPLIT_TAIL(0xE20);
    GC_CAMERA_ZERO_VEC(0xE30);

    *(float *)((char *)this + 0xE50) = one;
    *(float *)((char *)this + 0xE54) = one;
    *(float *)((char *)this + 0xE58) = big;
    *(float *)((char *)this + 0xE60) = z;
    *(float *)((char *)this + 0xE64) = z;
    *(float *)((char *)this + 0xE68) = z;
    *(float *)((char *)this + 0xE6C) = one;
    *(float *)((char *)this + 0xE70) = one;
    GC_CAMERA_ZERO_VEC_SPLIT_HEAD();
    *(float *)((char *)this + 0xE74) = big;
    GC_CAMERA_ZERO_VEC_SPLIT_TAIL(0xE80);
    GC_CAMERA_ZERO_VEC(0xE90);

    *(float *)((char *)this + 0xEB0) = one;
    void *innerCamera = (char *)this + 0xED0;
    *(float *)((char *)this + 0xEB4) = one;
    *(float *)((char *)this + 0xEB8) = big;
    eCamera_ctor(innerCamera);
    *(int *)((char *)this + 0x11E0) = 0;
    gcUI_ctor((char *)this + 0x11E4, 0, 0);
    Reset();
}
#pragma control sched=2

gcDesiredCamera::gcDesiredCamera(cBase *parent) {
    *(cBase **)((char *)this + 0x00) = parent;
    *(void **)((char *)this + 0x04) = (void *)0x388CF0;
    char *desired = (char *)this + 0x0C;
    *(int *)((char *)this + 0x08) = 0;
    gcDesiredObject_ctor(desired, (cBase *)this);
    *(void **)((char *)this + 0x10) = gcDesiredCamera_typeinfo_338;
    void *helper = (char *)this + 0x18;
    int one = 1;
    gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
    *(void **)((char *)this + 0x10) = (void *)0x388A48;
    *(void **)((char *)this + 0x20) = desired;
    *(void **)((char *)this + 0x24) = (void *)0x388568;
    *(unsigned char *)((char *)this + 0x28) = (unsigned char)one;
    *(unsigned char *)((char *)this + 0x29) = 0;
    *(int *)((char *)this + 0x2C) = 0;
    *(int *)((char *)this + 0x30) = 0;
    *(void **)((char *)this + 0x34) = (void *)((unsigned int)desired | 1);
}
