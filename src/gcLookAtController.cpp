class ePoint;
class cBase;
class cMemPool;
class cFile;
struct LookAtParent;
struct LookAtRef;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End();
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class gcPartialEntityController {
public:
    LookAtParent *m_parent;     // 0x00
    void *m_classDesc;          // 0x04
    LookAtRef *m_ref;           // 0x08
    char m_basePad0C[0x38 - 0x0C];

    gcPartialEntityController(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern char gcLookAtControllervirtualtable[];

template <class T>
class cHandleT {
public:
    int mIndex;
};

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

class gcLookAtController : public gcPartialEntityController {
public:
    cHandleT<ePoint> m_target;    // 0x38
    cHandleT<ePoint> m_target2;   // 0x3C  (separate handle)
    short m_state;                // 0x40
    short m_pad42;                // 0x42
    char m_arr44[8];              // 0x44

    gcLookAtController(cBase *);
    int Read(cFile &, cMemPool *);
    void SetHPR(float heading, float pitch, float roll, bool snap);
    void LookAt(cHandleT<ePoint> p);
    float GetHeading() const;
    float GetPitch() const;
    float GetRoll() const;
    void OnDeactivated();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

gcLookAtController::gcLookAtController(cBase *parent) : gcPartialEntityController(parent) {
    *(void **)((char *)this + 4) = gcLookAtControllervirtualtable;
    m_target.mIndex = 0;
    m_target2.mIndex = 0;
    m_state = 0;
    m_pad42 = -1;
    __vec_new((char *)this + 0x44, 1, 6, (void (*)(void *))0x24400C);
}

int gcLookAtController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 &&
        gcPartialEntityController::Read(file, pool)) goto success;
    ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
    return 0;
success:
    return result;
}

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

// gcLookAtController::Write(cFile &) const — 0x00146f9c
void gcLookAtController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcPartialEntityController *)this)->Write(file);
    wb.End();
}

// gcLookAtController::New(cMemPool *, cBase *) static — 0x002c6cfc
cBase *gcLookAtController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcLookAtController *result = 0;
    gcLookAtController *obj = (gcLookAtController *)rec->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLookAtController(parent);
        result = obj;
    }
    return (cBase *)result;
}
