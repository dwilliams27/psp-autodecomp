// src/eStaticPointLight.cpp
// Functions:
//   0x0005eec4  eStaticPointLight::Write(cFile &) const                       eAll_psp.obj
//   0x0005ef10  eStaticPointLight::Read(cFile &, cMemPool *)                  eAll_psp.obj
//   0x0005efcc  eStaticPointLight::eStaticPointLight(cBase *)                 eAll_psp.obj
//   0x0005f000  eStaticPointLight::~eStaticPointLight(void)                    eAll_psp.obj
//   0x0005f15c  eStaticPointLight::GetSampleRay(mRay*, mVec3*, const mVec3&, const mVec3&) const   eAll_psp.obj
//   0x00205df0  eStaticPointLight::AssignCopy(const cBase *)                  eAll_psp.obj
//   0x00205e68  eStaticPointLight::New(cMemPool *, cBase *) static            eAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;
class mRay;
class mVec3;

typedef int v4sf_t __attribute__((mode(V4SF)));

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
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class eStaticLight {
public:
    eStaticLight(cBase *);
    ~eStaticLight();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eStaticPointLight : public eStaticLight {
public:
    eStaticPointLight(cBase *);
    ~eStaticPointLight();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    void GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);

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

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mIndex;
};

extern char eStaticPointLightvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B30;
extern cType *D_00046B34;

#pragma control sched=1
// ── 0x0005eec4 — Write(cFile &) const ──
void eStaticPointLight::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eStaticLight *)this)->Write(file);
    wb.End();
}

// ── 0x0005ef10 — Read(cFile &, cMemPool *) ──
int eStaticPointLight::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eStaticLight::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── 0x0005efcc — eStaticPointLight(cBase *) ──
eStaticPointLight::eStaticPointLight(cBase *parent)
    : eStaticLight(parent) {
    *(void **)((char *)this + 4) = eStaticPointLightvirtualtable;
}

// ── 0x0005f000 — ~eStaticPointLight(void) ──
eStaticPointLight::~eStaticPointLight() {
    *(void **)((char *)this + 4) = eStaticPointLightvirtualtable;
}

// ── 0x00205df0 — AssignCopy(const cBase *) ──
void eStaticPointLight::AssignCopy(const cBase *base) {
    eStaticPointLight *other = dcast<eStaticPointLight>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 68) = *(const float *)((char *)other + 68);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 72) = *(const cHandle *)((char *)other + 72);
    __asm__ volatile("" ::: "memory");
    *(v4sf_t *)((char *)this + 0x80) = *(const v4sf_t *)((char *)other + 0x80);
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((char *)other + 0x50);
    *(v4sf_t *)((char *)this + 0x60) = *(const v4sf_t *)((char *)other + 0x60);
    *(v4sf_t *)((char *)this + 0x70) = *(const v4sf_t *)((char *)other + 0x70);
}

// ── 0x0005f15c — GetSampleRay(...) const ──
void eStaticPointLight::GetSampleRay(mRay *ray, mVec3 *, const mVec3 &p1, const mVec3 &) const {
    __asm__ volatile(
        ".word 0xd8860080\n"  /* lv.q     C120, 0x80($a0)        */
        ".word 0xd8e70000\n"  /* lv.q     C130, 0x0($a3)         */
        ".word 0x60878606\n"  /* vsub.t   C120, C120, C130       */
        ".word 0xf8a60010\n"  /* sv.q     C120, 0x10($a1)        */
        ".word 0x64868604\n"  /* vdot.t   S100, C120, C120       */
        ".word 0x6c000408\n"  /* vcmp.s   ez, S100               */
        ".word 0xd0110424\n"  /* vrsq.s   S101, S100             */
        ".word 0xd0160444\n"  /* vsqrt.s  S102, S100             */
        ".word 0xdc0010e5\n"  /* vpfxs    1, Y, Z, W             */
        ".word 0xd2a00424\n"  /* vcmovt.s S101, S100, 0          */
        ".word 0x65248606\n"  /* vscl.t   C120, C120, S101       */
        ".word 0xf8a60010\n"  /* sv.q     C120, 0x10($a1)        */
        ".word 0x48640044\n"  /* mfv      $a0, S102              */
        ".word 0x44846000\n"  /* mtc1     $a0, $f12              */
        ".word 0xe4ac0020\n"  /* swc1     $f12, 32($a1)          */
        ::: "memory"
    );
    *(v4sf_t *)ray = *(v4sf_t *)&p1;
}
#pragma control sched=2

// ── 0x00205e68 — New(cMemPool *, cBase *) static ──
#pragma control sched=1
cBase *eStaticPointLight::New(cMemPool *pool, cBase *parent) {
    eStaticPointLight *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStaticPointLight *obj = (eStaticPointLight *)rec->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eStaticPointLight(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eStaticPointLight::GetType(void) const {
    if (D_00046B34 == 0) {
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
                    cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
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
        cBase *(*factory)(cMemPool *, cBase *) = &eStaticPointLight::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B34 = cType::InitializeType(0, 0, 0x4B, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B34;
}
#pragma control sched=2
