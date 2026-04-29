class cBase;
class cMemPool;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T> T *dcast(const cBase *);

template <class T> class cHandleT {
public:
    int mIndex;
};

class eGeomTemplate;

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *,
                                 const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eWeatherSystem {
public:
    cBase *mOwner;
    void *mClassDesc;

    eWeatherSystem(cBase *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern char eWeatherSystemclassdesc[];
extern const char eWeatherSystem_base_name[];
extern const char eWeatherSystem_base_desc[];

static cType *type_base;
static cType *type_eWeatherSystem;

// ── eWeatherSystem::eWeatherSystem(cBase *) @ 0x00061954 ──
eWeatherSystem::eWeatherSystem(cBase *parent) {
    mOwner = parent;
    mClassDesc = eWeatherSystemclassdesc;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(eWeatherSystem **)((char *)this + 0x10) = this;

    float zero = 0.0f;
    *(float *)((char *)this + 0x14) = zero;
    *(float *)((char *)this + 0x18) = 45.0f;
    *(float *)((char *)this + 0x1C) = -45.0f;
    *(float *)((char *)this + 0x20) = 30.0f;
    *(float *)((char *)this + 0x24) = 5.0f;
    *(float *)((char *)this + 0x28) = zero;
    *(int *)((char *)this + 0x2C) = 0;
    *(float *)((char *)this + 0x30) = 0.5f;
    *(int *)((char *)this + 0x34) = 0;

    int x;
    int y;
    int z;
    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(z) : "f"(zero));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x40(%3)\n"
        :
        : "r"(x), "r"(y), "r"(z), "r"(this)
        : "memory");

    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(z) : "f"(zero));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x50(%3)\n"
        :
        : "r"(x), "r"(y), "r"(z), "r"(this)
        : "memory");

    *(float *)((char *)this + 0x60) = zero;
    *(float *)((char *)this + 0x64) = zero;
    *(eWeatherSystem **)0x37D31C = this;
}

// ── eWeatherSystem::AssignCopy(const cBase *) @ 0x00207e70 ──
#pragma control sched=1
void eWeatherSystem::AssignCopy(const cBase *src) {
    eWeatherSystem *other = dcast<eWeatherSystem>(src);
    const cArrayBase<cHandleT<eGeomTemplate> > &srcArr =
        *(const cArrayBase<cHandleT<eGeomTemplate> > *)((char *)other + 0x08);
    ((cArrayBase<cHandleT<eGeomTemplate> > *)((char *)this + 0x08))->operator=(
        srcArr);
    ((cBaseArray *)((char *)this + 0x0C))->operator=(
        *(const cBaseArray *)((char *)other + 0x0C));
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    *(float *)((char *)this + 0x1C) = *(float *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x20) = *(float *)((char *)other + 0x20);
    *(float *)((char *)this + 0x24) = *(float *)((char *)other + 0x24);
    *(float *)((char *)this + 0x28) = *(float *)((char *)other + 0x28);
    __asm__ volatile("" ::: "memory");
    int *dst2C = (int *)((char *)this + 0x2C);
    int *src2C = (int *)((char *)other + 0x2C);
    int v2C = *src2C;
    *dst2C = v2C;
    *(float *)((char *)this + 0x30) = *(float *)((char *)other + 0x30);
    __asm__ volatile("" ::: "memory");
    int *dst34 = (int *)((char *)this + 0x34);
    int *src34 = (int *)((char *)other + 0x34);
    int v34 = *src34;
    *dst34 = v34;
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x50) = *(v4sf_t *)((char *)other + 0x50);
    *(float *)((char *)this + 0x60) = *(float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)other + 0x64);
}
#pragma control sched=2

// ── eWeatherSystem::New(cMemPool *, cBase *) static @ 0x00207f34 ──
#pragma control sched=1
cBase *eWeatherSystem::New(cMemPool *pool, cBase *parent) {
    eWeatherSystem *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eWeatherSystem *obj = (eWeatherSystem *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eWeatherSystem(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// ── eWeatherSystem::GetType(void) const @ 0x00207fb0 ──
#pragma control sched=1
const cType *eWeatherSystem::GetType(void) const {
    if (!type_eWeatherSystem) {
        if (!type_base) {
            const char *name = eWeatherSystem_base_name;
            const char *desc = eWeatherSystem_base_desc;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            type_base = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *base = type_base;
        __asm__ volatile("" : "+r"(base));
        cBase *(*factory)(cMemPool *, cBase *) = &eWeatherSystem::New;
        __asm__ volatile("" : "+r"(factory));
        type_eWeatherSystem = cType::InitializeType(0, 0, 0x195, base,
                                                    factory, 0, 0, 0);
    }
    return type_eWeatherSystem;
}
#pragma control sched=2
