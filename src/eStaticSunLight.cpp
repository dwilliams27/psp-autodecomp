// Functions:
//   0x0005f9f4 eStaticSunLight::Write(cFile &) const                eAll_psp.obj
//   0x0005fbb4 eStaticSunLight::~eStaticSunLight(void)              eAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class mVec3;
class mRay;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool_helper {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

template <class T> T *dcast(const cBase *);

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

// ============================================================
// eStaticSunLight class declaration
// ============================================================

class eStaticLight {
public:
    eStaticLight(cBase *);
    ~eStaticLight();
    void Write(cFile &) const;
};

class eStaticSunLight : public eStaticLight {
public:
    eStaticSunLight(cBase *);
    ~eStaticSunLight();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void GetDirectLight(mVec3 *, const mVec3 &, const mVec3 &, const mRay &, const mVec3 &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_helper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eStaticSunLightvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B30;
extern cType *D_00046B40;

extern "C" void eStaticSunLight_eStaticSunLight(eStaticSunLight *, cBase *);

// ============================================================
// 0x0005f9f4 — eStaticSunLight::Write(cFile &) const
// 0x0005fbb4 — eStaticSunLight::~eStaticSunLight(void)
// ============================================================

#pragma control sched=1
void eStaticSunLight::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eStaticLight::Write(file);
    wb.Write(*(float *)((char *)this + 0x90));
    wb.Write(*(float *)((char *)this + 0x94));
    ((const cHandle *)((char *)this + 0x98))->Write(wb);
    wb.End();
}

eStaticSunLight::eStaticSunLight(cBase *parent) : eStaticLight(parent) {
    *(void **)((char *)this + 4) = eStaticSunLightvirtualtable;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x90) = 0.0f;
    *(float *)((char *)this + 0x94) = 0.0f;
    *(int *)((char *)this + 0x98) = 0;
    *(unsigned char *)((char *)this + 0x9C) = 1;
}

eStaticSunLight::~eStaticSunLight() {
    *(void **)((char *)this + 4) = eStaticSunLightvirtualtable;
}

const cType *eStaticSunLight::GetType(void) const {
    if (D_00046B40 == 0) {
        if (D_00046B30 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385DC;
                    cBase *(*factory)(cMemPool *, cBase *) =
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                    __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                    D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                       factory, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_000385E4;
            const char *kindName = (const char *)0x36CEE0;
            const char *kindDesc = (const char *)0x36CEEC;
            __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
            D_00046B30 = cType::InitializeType(0, 0, 0x4A, parentType,
                                               0, kindName, kindDesc, 0);
        }
        const cType *parentType = D_00046B30;
        cBase *(*factory)(cMemPool *, cBase *) = &eStaticSunLight::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B40 = cType::InitializeType(0, 0, 0x4F, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B40;
}
#pragma control sched=2

void eStaticSunLight::GetDirectLight(mVec3 *out, const mVec3 &, const mVec3 &normal, const mRay &ray, const mVec3 &) const {
    volatile float color[8];

    float zero, dot;
    __asm__ volatile(
        "lv.q C120, 0x10(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a2, S100\n"
        "mtc1 $a2, %1\n"
        "mtc1 $zero, %0\n"
        : "=f"(zero), "=f"(dot)
        : "r"(&ray), "r"(&normal)
        : "$a2"
    );

    if (dot > zero) {
        float scale = 1.0f / 255.0f;
        color[0] = (float)*(const unsigned char *)((const char *)this + 0x4A) * scale;
        color[1] = (float)*(const unsigned char *)((const char *)this + 0x49) * scale;
        color[2] = (float)*(const unsigned char *)((const char *)this + 0x48) * scale;
        float factor = dot * *(const float *)((const char *)this + 0x44);

        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mtv $a0, S100\n"
            "lv.q C120, 0($sp)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(factor), "r"(out)
            : "$a0", "memory"
        );
        return;
    }

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a2, %0\n"
        "mfc1 $a3, %0\n"
        "mtv $a0, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(zero), "r"(out)
        : "$a0", "$a2", "$a3", "memory"
    );
}

#pragma control sched=1

void eStaticSunLight::AssignCopy(const cBase *base) {
    eStaticSunLight *other = dcast<eStaticSunLight>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    *(cHandle *)((char *)this + 0x48) = *(const cHandle *)((const char *)other + 0x48);
    __asm__ volatile(
        "lv.q C120, 0x80(%1)\n"
        "sv.q C120, 0x80(%0)\n"
        "lv.q C120, 0x50(%1)\n"
        "sv.q C120, 0x50(%0)\n"
        "lv.q C120, 0x60(%1)\n"
        "sv.q C120, 0x60(%0)\n"
        "lv.q C120, 0x70(%1)\n"
        "sv.q C120, 0x70(%0)\n"
        :: "r"(this), "r"(other) : "memory"
    );
    *(float *)((char *)this + 0x90) = *(const float *)((const char *)other + 0x90);
    *(float *)((char *)this + 0x94) = *(const float *)((const char *)other + 0x94);
    *(cHandle *)((char *)this + 0x98) = *(const cHandle *)((const char *)other + 0x98);
    *(unsigned char *)((char *)this + 0x9C) = *(const unsigned char *)((const char *)other + 0x9C);
}

cBase *eStaticSunLight::New(cMemPool *pool, cBase *parent) {
    eStaticSunLight *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStaticSunLight *obj = (eStaticSunLight *)entry->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eStaticSunLight_eStaticSunLight(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
