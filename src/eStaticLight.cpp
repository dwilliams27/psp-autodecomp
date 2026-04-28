// eStaticLight.cpp - decompiled from eAll_psp.obj
// Functions:
//   0x0005eddc eStaticLight::eStaticLight(cBase *)
//   0x0005ee48 eStaticLight::~eStaticLight(void)
//   0x00205de8 eStaticLight::GetNumStratifiedSamples(void) const

class cBase;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    ~cObject(void);
};

class eStaticLight : public cObject {
public:
    eStaticLight(cBase *);
    ~eStaticLight(void);
    int GetNumStratifiedSamples(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eStaticLightvirtualtable[];

#pragma control sched=1
eStaticLight::eStaticLight(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eStaticLightvirtualtable;
    *(float *)((char *)this + 0x44) = 1.0f;
    *(int *)((char *)this + 0x48) = -1;
    __asm__ volatile(
        "vmidt.q M000\n"
        "vmov.q C120, C000\n"
        "vmov.q C130, C010\n"
        "vmov.q C200, C020\n"
        "vmov.q C210, C030\n"
        "sv.q C120, 0x50(%0)\n"
        "sv.q C130, 0x60(%0)\n"
        "sv.q C200, 0x70(%0)\n"
        "sv.q C210, 0x80(%0)\n"
        : : "r"(this) : "memory"
    );
}

eStaticLight::~eStaticLight() {
    *(void **)((char *)this + 4) = eStaticLightvirtualtable;
}
#pragma control sched=2

int eStaticLight::GetNumStratifiedSamples(void) const {
    return 1;
}
