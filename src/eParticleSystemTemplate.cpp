class cBase;
class cMemPool;
class cType;
class eParticle;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

class eParticleSystemTemplate {
public:
    void Reset(cMemPool *, bool);
    const cType *GetInstanceType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void eParticleSystemTemplate__eParticleSystemTemplate_cBaseptr(void *, cBase *);

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
        eParticleSystemTemplate__eParticleSystemTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
