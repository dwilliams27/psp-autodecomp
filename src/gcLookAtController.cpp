class gcLookAtController {
public:
    void SetHPR(float heading, float pitch, float roll, bool snap);
};

static inline bool isNewEntry(int flags) {
    return (flags & 4) != 0;
}

void gcLookAtController::SetHPR(float heading, float pitch, float roll, bool snap) {
    int z1[1] = {0};
    *(short *)((char *)this + 0x40) = 0;
    *(int *)((char *)this + 0x3C) = z1[0];
    int z2[1] = {0};
    *(int *)((char *)this + 0x38) = z2[0];

    char *parent;
    char *ref;
    int off;
    char *e;
    int flags;
    float v;

    // --- Set Heading ---
    parent = *(char **)(char *)this;
    ref = *(char **)((char *)this + 0x08);
    e = *(char **)(parent + 0x1AC) + ((int)*(signed char *)(ref + 0x14) << 5);
    *(float *)(*(char **)(parent + 0x1AC) + off) = heading;
    __asm__ volatile("" ::: "memory");
    off = (int)*(signed char *)(ref + 0x14) << 5;
    __asm__ volatile("" ::: "memory");

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    __asm__ volatile("" ::: "memory");
    if (isNewEntry(flags)) {
        v = 0.0f;
    } else {
        v = *(float *)(e + 4);
    }
    *(float *)(e + 4) = v;

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    if (isNewEntry(flags)) {
        v = 0.0f;
    } else {
        v = *(float *)(e + 8);
    }
    *(float *)(e + 8) = v;

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    *(unsigned short *)(e + 0x1C) = (unsigned short)(flags & ~4);

    // --- Set Pitch ---
    parent = *(char **)(char *)this;
    ref = *(char **)((char *)this + 0x08);
    off = (int)*(signed char *)(ref + 0x14) << 5;
    __asm__ volatile("" ::: "memory");
    e = *(char **)(parent + 0x1AC) + off;

    flags = *(unsigned short *)(e + 0x1C);
    if (isNewEntry(flags)) {
        v = 0.0f;
    } else {
        v = *(float *)e;
    }
    *(float *)e = v;

    *(float *)(*(char **)(parent + 0x1AC) + off + 4) = pitch;

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    if (isNewEntry(flags)) {
        v = 0.0f;
    } else {
        v = *(float *)(e + 8);
    }
    *(float *)(e + 8) = v;

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    *(unsigned short *)(e + 0x1C) = (unsigned short)(flags & ~4);

    // --- Set Roll ---
    parent = *(char **)(char *)this;
    ref = *(char **)((char *)this + 0x08);
    e = *(char **)(parent + 0x1AC) + off;
    off = (int)*(signed char *)(ref + 0x14) << 5;

    flags = *(unsigned short *)(e + 0x1C);
    if (isNewEntry(flags)) {
        v = 0.0f;
    } else {
        v = *(float *)e;
    }
    *(float *)e = v;

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    if (isNewEntry(flags)) {
        v = 0.0f;
    } else {
        v = *(float *)(e + 4);
    }
    *(float *)(e + 4) = v;

    *(float *)(*(char **)(parent + 0x1AC) + off + 8) = roll;

    e = *(char **)(parent + 0x1AC) + off;
    flags = *(unsigned short *)(e + 0x1C);
    *(unsigned short *)((int)e + 0x1C) = (unsigned short)(flags & ~4);

    // --- Snap ---
    if (snap) {
        parent = *(char **)(char *)this;
        ref = *(char **)((char *)this + 0x08);
        e = *(char **)(parent + 0x1AC) + off;
        off = (int)*(signed char *)(ref + 0x14) << 5;

        *(float *)(e + 0x10) = *(float *)e;
        e = *(char **)(parent + 0x1AC) + off;
        *(float *)(e + 0x14) = *(float *)(e + 4);
        e = *(char **)(parent + 0x1AC) + off;
        *(float *)(e + 0x18) = *(float *)(e + 8);

        e = *(char **)(parent + 0x1AC) + off;
        flags = *(unsigned short *)(e + 0x1C);
        *(unsigned short *)((int)e + 0x1C) = (unsigned short)(flags & ~4);
    }
}
