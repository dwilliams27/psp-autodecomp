// ODR-WARNING: split-TU local redeclaration for one gcLookAtController method.
// Keep this class layout minimal so adding SetPitch does not perturb siblings.

struct LookAtParent;
struct LookAtRef;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class ePoint;

struct LookAtEntry {
    float v0;
    float v4;
    float v8;
    float vC;
    float heading;
    float pitch;
    float roll;
    unsigned short flags;
    short pad1E;
};

struct LookAtParent {
    char pad0[0x1AC];
    LookAtEntry *array;
};

struct LookAtRef {
    char pad0[0x14];
    signed char idx;
};

class gcLookAtController {
public:
    LookAtParent *m_parent;
    void *m_classDesc;
    LookAtRef *m_ref;
    char m_basePad0C[0x38 - 0x0C];
    cHandleT<ePoint> m_target;
    cHandleT<ePoint> m_target2;
    short m_state;

    void SetPitch(float pitch);
};

static inline bool isNewLookAtEntry(int flags) {
    return (flags & 4) != 0;
}

void gcLookAtController::SetPitch(float pitch) {
    volatile int z1[1];
    z1[0] = 0;
    m_state = 0;
    int zero = z1[0];
    int *target2 = &m_target2.mIndex;
    *target2 = zero;

    volatile int z2[1];
    z2[0] = 0;
    zero = z2[0];
    int *target = &m_target.mIndex;
    *target = zero;

    LookAtParent *parent = m_parent;
    LookAtRef *ref = m_ref;
    LookAtEntry *array = parent->array;
    int off = (int)ref->idx << 5;
    LookAtEntry *entry = (LookAtEntry *)((char *)array + off);
    float value;

    if (isNewLookAtEntry(entry->flags)) {
        value = 0.0f;
    } else {
        value = entry->v0;
    }
    entry->v0 = value;

    float *pitchPtr = (float *)((char *)parent->array + off + 4);
    *pitchPtr = pitch;

    entry = (LookAtEntry *)((char *)parent->array + off);
    int newEntry = isNewLookAtEntry(entry->flags);
    if (newEntry) {
        value = 0.0f;
    } else {
        newEntry = (int)((char *)entry + 8);
        value = *(float *)newEntry;
    }
    __asm__ volatile("" : "+r"(entry) :: "memory");
    char *entryBytes = (char *)entry;
    entryBytes += 8;
    *(float *)entryBytes = value;

    entry = (LookAtEntry *)((char *)parent->array + off);
    int flags = entry->flags;
    flags = (unsigned short)(flags & ~4);
    entry->flags = (unsigned short)flags;
}
