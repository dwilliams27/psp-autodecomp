class ePoint;

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT();
};

template <class T>
cHandleT<T>::cHandleT() : mIndex(0) {}

struct LookAtEntry {
    float v0;
    float v4;
    float v8;
    float vC;
    float heading;        // 0x10
    float pitch;          // 0x14
    float roll;           // 0x18
    unsigned short flags; // 0x1C
    short pad1E;
};

struct LookAtParent {
    char pad0[0x152];
    unsigned char b152;       // 0x152
    char pad1[0x1AC - 0x153];
    LookAtEntry *array;       // 0x1AC
};

struct LookAtRef {
    char pad0[0x14];
    signed char idx;          // 0x14
};

struct gcLookAtController_layout {
    LookAtParent *parent;     // 0x00
    char pad04[4];            // 0x04
    LookAtRef *ref;           // 0x08
    char pad0C[0x40 - 0x0C];
};

class gcLookAtController {
public:
    LookAtParent *m_parent;       // 0x00
    int m_pad04;                  // 0x04
    LookAtRef *m_ref;             // 0x08
    char m_pad0C[0x38 - 0x0C];    // 0x0C..0x37
    cHandleT<ePoint> m_target;    // 0x38
    cHandleT<ePoint> m_target2;   // 0x3C  (separate handle)
    short m_state;                // 0x40

    void SetHPR(float heading, float pitch, float roll, bool snap);
    void LookAt(cHandleT<ePoint> p);
    float GetHeading() const;
    float GetPitch() const;
    float GetRoll() const;
    void OnDeactivated();
};

void gcLookAtController::LookAt(cHandleT<ePoint> p) {
    m_state = 2;
    m_target = p;
    cHandleT<ePoint> *p2 = &m_target2;
    p2->mIndex = 0;
}

float gcLookAtController::GetHeading() const {
    return m_parent->array[m_ref->idx].heading;
}

float gcLookAtController::GetPitch() const {
    return m_parent->array[m_ref->idx].pitch;
}

float gcLookAtController::GetRoll() const {
    return m_parent->array[m_ref->idx].roll;
}

void gcLookAtController::OnDeactivated() {
    LookAtParent *parent = m_parent;
    LookAtEntry *e = &parent->array[m_ref->idx];
    unsigned short newFlags = (unsigned short)(e->flags & ~1);
    e->flags = newFlags;
    parent->b152 = (unsigned char)(parent->b152 | 0x10);
}

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
