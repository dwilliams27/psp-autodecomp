// eSimpleMotorConfig — derives from ePhysicsMotorConfig (size 0x40).
//
// Functions matched here:
//   eSimpleMotorConfig::~eSimpleMotorConfig(void)       @ 0x00076494  124B
//   eSimpleMotorConfig::New(cMemPool *, cBase *) static @ 0x0020e758  124B

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

template <class T> T *dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class ePhysicsMotorConfig {
public:
    char _pad[0x40];
    ePhysicsMotorConfig(cBase *);
    ~ePhysicsMotorConfig();
    void Write(cFile &) const;
};

extern char eSimpleMotorConfigvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000469F0;
extern cType *D_00046C00;

class eSimpleMotorConfig : public ePhysicsMotorConfig {
public:
    eSimpleMotorConfig(cBase *);
    ~eSimpleMotorConfig();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;

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

#pragma control sched=1

// ── eSimpleMotorConfig::~eSimpleMotorConfig(void) @ 0x00076494 ──
eSimpleMotorConfig::~eSimpleMotorConfig() {
    *(void **)((char *)this + 4) = eSimpleMotorConfigvirtualtable;
}

// ── eSimpleMotorConfig::New(cMemPool *, cBase *) static @ 0x0020e758 ──
cBase *eSimpleMotorConfig::New(cMemPool *pool, cBase *parent) {
    eSimpleMotorConfig *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSimpleMotorConfig *obj = (eSimpleMotorConfig *)rec->fn(base, 0x40, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eSimpleMotorConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eSimpleMotorConfig::GetType(void) const @ 0x0020e7d4 ──
const cType *eSimpleMotorConfig::GetType(void) const {
    if (D_00046C00 == 0) {
        if (D_000469F0 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469F0 = cType::InitializeType(
                0, 0, 0x260, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_000469F0;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20E758;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C00 = cType::InitializeType(
            0, 0, 0x263, parentType, factory, 0, 0, 0);
    }
    return D_00046C00;
}

// ── eSimpleMotorConfig::AssignCopy(const cBase *) @ 0x0020e6c8 ──
void eSimpleMotorConfig::AssignCopy(const cBase *src) {
    eSimpleMotorConfig &other = *dcast<eSimpleMotorConfig>(src);
    int *dst = (int *)((char *)this + 8);
    int *from = (int *)((char *)&other + 8);
    int v0 = from[0];
    int v1 = from[1];
    int v2 = from[2];
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(v2));
    dst[0] = v0;
    dst[1] = v1;
    dst[2] = v2;
    v0 = from[3];
    v1 = from[4];
    v2 = from[5];
    __asm__ volatile("" : "+r"(v0), "+r"(v1), "+r"(v2));
    dst[3] = v0;
    dst[4] = v1;
    dst[5] = v2;
    *(v4sf_t *)((char *)this + 0x20) =
        *(v4sf_t *)((char *)&other + 0x20);
    *(float *)((char *)this + 0x30) = *(float *)((char *)&other + 0x30);
    *(float *)((char *)this + 0x34) = *(float *)((char *)&other + 0x34);
    *(int *)((char *)this + 0x38) = *(int *)((char *)&other + 0x38);
    *(unsigned char *)((char *)this + 0x3C) =
        *(unsigned char *)((char *)&other + 0x3C);
    *(unsigned char *)((char *)this + 0x3D) =
        *(unsigned char *)((char *)&other + 0x3D);
}

// ── eSimpleMotorConfig::Write(cFile &) const @ 0x000761f8 ──
void eSimpleMotorConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ePhysicsMotorConfig::Write(file);
    wb.Write(3, (const float *)((const char *)this + 0x20));
    wb.Write(*(const float *)((const char *)this + 0x30));
    wb.Write(*(const float *)((const char *)this + 0x34));
    wb.Write(*(const int *)((const char *)this + 0x38));
    wb.Write(*(const bool *)((const char *)this + 0x3C));
    wb.Write(*(const bool *)((const char *)this + 0x3D));
    wb.End();
}

#pragma control sched=2
