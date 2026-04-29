// eStaticLight.cpp - decompiled from eAll_psp.obj
// Functions:
//   0x0005eddc eStaticLight::eStaticLight(cBase *)
//   0x0005ee48 eStaticLight::~eStaticLight(void)
//   0x0005ebd0 eStaticLight::Write(cFile &) const
//   0x00205ccc eStaticLight::GetType(void) const
//   0x00205de8 eStaticLight::GetNumStratifiedSamples(void) const

class cBase;
class cFile;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
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
    void Write(cFile &) const;
};

class eStaticLight : public cObject {
public:
    eStaticLight(cBase *);
    ~eStaticLight(void);
    void Write(cFile &) const;
    const cType *GetType(void) const;
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

struct eStaticLightTypeSlot {
    char _pad[0x6B30];
    cType *value;
};

extern char eStaticLightvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B30;

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

#pragma control sched=1
void eStaticLight::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    cObject::Write(file);
    wb.Write(*(const float *)((const char *)this + 0x44));
    wb.Write(*(const unsigned int *)((const char *)this + 0x48));
    wb.Write(3, (const float *)((const char *)this + 0x80));
    wb.Write(3, (const float *)((const char *)this + 0x50));
    wb.Write(3, (const float *)((const char *)this + 0x60));
    wb.Write(3, (const float *)((const char *)this + 0x70));
    wb.End();
}

const cType *eStaticLight::GetType(void) const {
    eStaticLightTypeSlot *base3 = (eStaticLightTypeSlot *)0x40000;
    if (base3->value == 0) {
        cType **base2 = (cType **)0x40000;
        if (base2[-0x1E87] == 0) {
            cType **base1 = (cType **)0x40000;
            if (base1[-0x1E88] == 0) {
                cType **base0 = (cType **)0x40000;
                if (base0[-0x1E89] == 0) {
                    base0[-0x1E89] = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                base1[-0x1E88] = cType::InitializeType(
                    0, 0, 2, base0[-0x1E89],
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                    0, 0, 0);
            }
            base2[-0x1E87] = cType::InitializeType(
                0, 0, 3, base1[-0x1E88], 0, 0, 0, 0);
        }
        base3->value = cType::InitializeType(
            0, 0, 0x4A, base2[-0x1E87],
            0, (const char *)0x36CEE0, (const char *)0x36CEEC, 0);
    }
    return base3->value;
}
#pragma control sched=2

int eStaticLight::GetNumStratifiedSamples(void) const {
    return 1;
}
