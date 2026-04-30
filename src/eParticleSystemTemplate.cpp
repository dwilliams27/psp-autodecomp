class cBase;
class cMemPool;
class cType;
class eParticle;

typedef int v4sf_t __attribute__((mode(V4SF)));

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject(cBase *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T>
class cArrayBase {
public:
    bool SetSize(int, const cArrayBase<T> &);
};

class eParticleSystem {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eParticleSystemTemplate : public cObject {
public:
    eParticleSystemTemplate(cBase *);
    void Reset(cMemPool *, bool);
    const cType *GetType(void) const;
    const cType *GetInstanceType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct ParticleArrayDefault {
    char _pad[0x20];
    float f20;
    int i24;
    int i28;
    float f2C;
    float f30;
    int i34;
    short h38;
    char _tail[6];
};

struct WordValue {
    int value;
};

struct QuadValue {
    v4sf_t value;
};

struct eParticleSystemTemplateLayout {
    char _pad0[0x4C];
    unsigned char enabled;
    unsigned char dirty;
    char _pad4E[2];
    int particleCount;
    WordValue word54;
    char _pad58[0x48];
    QuadValue quadA0;
    char _padB0[0x94];
    cArrayBase<eParticle> particles;
    char _pad145[0xB];
    int field150;
    WordValue word154;
    WordValue word158;
    WordValue word15C;
    int field160;
    char _pad164[0xC];
    QuadValue quad170;
};

static cType *type_cBase;
static cType *type_eGeom;
static cType *type_eDynamicGeom;
static cType *type_eParticleSystem;

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046C40;

// -- eParticleSystemTemplate::eParticleSystemTemplate(cBase *) @ 0x000b6c70 --
eParticleSystemTemplate::eParticleSystemTemplate(cBase *parent) : cObject(parent) {
    *(float *)((char *)this + 0x44) = 1000.0f;
    *(void **)((char *)this + 4) = (void *)0x00384988;
    *(float *)((char *)this + 0x48) = 0.0f;
    *(unsigned char *)((char *)this + 0x4C) = 0;
    *(unsigned char *)((char *)this + 0x4D) = 0;
    *(int *)((char *)this + 0x50) = 10;
    *(int *)((char *)this + 0x54) = 0;
    __asm__ volatile(
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0x60($s0)"
        ::: "a0", "a1", "a2", "memory");
    *(float *)((char *)this + 0x70) = 0.0f;
    *(float *)((char *)this + 0x74) = 0.0f;
    *(int *)((char *)this + 0x78) = 0;
    *(unsigned char *)((char *)this + 0x7C) = 0;
    *(unsigned char *)((char *)this + 0x7D) = 0;
    *(int *)((char *)this + 0x80) = 1;
    *(int *)((char *)this + 0x84) = 1;
    *(int *)((char *)this + 0x88) = 1;
    *(int *)((char *)this + 0x8C) = 0;
    *(int *)((char *)this + 0x90) = 0;
    *(unsigned char *)((char *)this + 0x94) = 0;
    *(float *)((char *)this + 0x98) = 0.0f;
    __asm__ volatile(
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0xa0($s0)"
        ::: "a0", "a1", "a2", "memory");
    *(int *)((char *)this + 0xB0) = 0;
    *(unsigned char *)((char *)this + 0xB4) = 0;
    *(float *)((char *)this + 0xB8) = 90.0f;
    *(float *)((char *)this + 0xBC) = 1.0f;
    *(float *)((char *)this + 0xC0) = 0.1f;
    *(float *)((char *)this + 0xC4) = 10.0f;
    *(int *)((char *)this + 0xC8) = *(int *)0x36C7FC * 2;
    *(float *)((char *)this + 0xCC) = 0.0f;
    *(int *)((char *)this + 0xD0) = 0x1E;
    *(float *)((char *)this + 0xD4) = 0.0f;
    *(float *)((char *)this + 0xD8) = 180.0f;
    *(float *)((char *)this + 0xDC) = 90.0f;
    *(float *)((char *)this + 0xE0) = 1.0f;
    *(float *)((char *)this + 0xE4) = 1.0f;
    *(float *)((char *)this + 0xE8) = 0.0f;
    *(float *)((char *)this + 0xEC) = 1.0f;
    *(unsigned char *)((char *)this + 0xF0) = 0;
    *(float *)((char *)this + 0xF4) = 0.0f;
    *(float *)((char *)this + 0xF8) = 0.0f;
    *(int *)((char *)this + 0xFC) = 2;
    *(int *)((char *)this + 0x100) = 0;
    *(int *)((char *)this + 0x104) = 0;
    *(int *)((char *)this + 0x108) = -1;
    *(int *)((char *)this + 0x10C) = -1;
    *(float *)((char *)this + 0x110) = 1.0f;
    *(float *)((char *)this + 0x114) = 1.0f;
    *(float *)((char *)this + 0x118) = 0.0f;
    *(float *)((char *)this + 0x11C) = 1.0f;
    *(int *)((char *)this + 0x120) = 0;
    *(int *)((char *)this + 0x124) = 0;
    *(unsigned char *)((char *)this + 0x128) = 0;
    __asm__ volatile(
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0x130($s0)"
        ::: "a0", "a1", "a2", "memory");
    *(int *)((char *)this + 0x140) = 0;
    *(int *)((char *)this + 0x144) = 0;
    *(int *)((char *)this + 0x150) = 0;
    *(int *)((char *)this + 0x154) = 0;
    *(int *)((char *)this + 0x158) = 0;
    *(int *)((char *)this + 0x15C) = 0;
    *(int *)((char *)this + 0x160) = 0;
    __asm__ volatile(
        "lui $a0, 0xbf80\n"
        "mtc1 $a0, $f13\n"
        "swc1 $f13, 0x180($s0)"
        ::: "a0", "memory");
    *(int *)((char *)this + 0x184) = 0;
    *(float *)((char *)this + 0x44) = 300.0f;
}

// -- eParticleSystemTemplate::Reset(cMemPool *, bool) @ 0x0007b578 --
#pragma control sched=1
void eParticleSystemTemplate::Reset(cMemPool *, bool) {
    eParticleSystemTemplateLayout *self = (eParticleSystemTemplateLayout *)this;
    cArrayBase<eParticle> *particles = &self->particles;
    int count = 0;
    __asm__ volatile("" : "+r"(count));
    if (self->enabled != 0) {
        count = self->particleCount;
    }
    int size = count;

    ParticleArrayDefault def;
    float zero = 0.0f;
    def.f20 = zero;
    __asm__ volatile("" ::: "memory");
    def.i24 = 0;
    def.i28 = 0;
    def.f2C = zero;
    def.f30 = zero;
    def.i34 = 0;
    def.h38 = 0;
    __asm__ volatile("" : "+m"(def));
    particles->SetSize(size, *(cArrayBase<eParticle> *)&def);

    self->field150 = 0;
    WordValue word;
    word.value = 0;
    self->word154 = word;
    word.value = 0;
    self->word158 = word;
    word.value = 0;
    self->word15C = word;
    __asm__ volatile("sw $0, 0x160(%0)" : : "r"(self) : "memory");

    __asm__ volatile(
        "lv.q C120, 0xa0($16)\n"
        "sv.q C120, 0x170($16)"
        :
        :
        : "memory");

    if (self->enabled != 0) {
        WordValue reset;
        reset.value = 0;
        self->word54 = reset;
        __asm__ volatile("sb %0, 0x4d(%1)" : : "r"(1), "r"(self) : "memory");
    }
}

// -- eParticleSystemTemplate::GetInstanceType(void) const @ 0x0007b634 --
const cType *eParticleSystemTemplate::GetInstanceType(void) const {
    if (type_eParticleSystem == 0) {
        if (type_eDynamicGeom == 0) {
            if (type_eGeom == 0) {
                if (type_cBase == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                }
                type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase,
                                                   0, 0, 0, 0);
            }
            type_eDynamicGeom = cType::InitializeType(0, 0, 0x17, type_eGeom,
                                                      0, 0, 0, 0);
        }
        const cType *parentType = type_eDynamicGeom;
        cBase *(*factory)(cMemPool *, cBase *) = eParticleSystem::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eParticleSystem = cType::InitializeType(0, 0, 0x30, parentType,
                                                     factory, 0, 0, 0);
    }
    return type_eParticleSystem;
}

// -- eParticleSystemTemplate::GetType(void) const @ 0x00214150 --
const cType *eParticleSystemTemplate::GetType(void) const {
    if (D_00046C40 == 0) {
        if (D_000469E0 == 0) {
            if (D_000469A8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CE2C;
                const char *kindDesc = (const char *)0x36CE3C;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_000469A8 = cType::InitializeType(
                    0, 0, 0x20, parentType, 0, kindName, kindDesc, 5);
            }
            D_000469E0 = cType::InitializeType(0, 0, 0x22, D_000469A8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469E0;
        cBase *(*factory)(cMemPool *, cBase *) = &eParticleSystemTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C40 = cType::InitializeType(0, 0, 0x31, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C40;
}

// -- eParticleSystemTemplate::New(cMemPool *, cBase *) static @ 0x002140d4 --
cBase *eParticleSystemTemplate::New(cMemPool *pool, cBase *parent) {
    eParticleSystemTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eParticleSystemTemplate *obj =
        (eParticleSystemTemplate *)rec->fn(base, 0x190, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eParticleSystemTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
