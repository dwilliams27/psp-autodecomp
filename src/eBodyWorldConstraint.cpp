// eBodyWorldConstraint — joint constraint that pins a rigid body to a fixed
// world transform. Inherits from eSimulatedConstraint (8-byte base: parent +
// classdesc). Body fields begin at offset 8 — three int slots, two scalar
// floats, then two 16-byte (V4SF) blocks at 0x20 and 0x30.
//
// Functions matched here:
//   eBodyWorldConstraint::eBodyWorldConstraint(cBase *)   @ 0x0006b440  (eAll_psp.obj)
//   eBodyWorldConstraint::Read(cFile &, cMemPool *)        @ 0x0006b384  (eAll_psp.obj)
//   eBodyWorldConstraint::Write(cFile &) const           @ 0x0006b338  (eAll_psp.obj)
//   eBodyWorldConstraint::Initialize(ePhysicsConstraintConfig *, eSimulatedController *) @ 0x0006b518 (eAll_psp.obj)
//   eBodyWorldConstraint::AssignCopy(const cBase *)      @ 0x0020992c  (eAll_psp.obj)
//   eBodyWorldConstraint::~eBodyWorldConstraint(void)    @ 0x0006b49c  (eAll_psp.obj)
//   eBodyWorldConstraint::OnPositionChanged(void)        @ 0x0006b5b0  (eAll_psp.obj)
//   eBodyWorldConstraint::New(cMemPool *, cBase *)       @ 0x0020998c  (eAll_psp.obj)

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;
class cType;
class ePhysicsConstraintConfig;
class eSimulatedController;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char eBodyWorldConstraintvirtualtable[];
extern cType *D_000385DC;
extern cType *D_00046BC4;
extern cType *D_00046BC8;

// cWriteBlock — RAII block-tag helper used by Write().
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

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct InitRecord {
    short offset;
    short pad;
    void (*fn)(void *, int, void *, void *);
};

// Pool delete-slot record — same shape as in other shape destructors.
struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

// dcast<T>(const cBase *) — linker-resolved per-type template instantiation
// that downcasts via classdesc lookup.
template <class T> T *dcast(const cBase *);

// eSimulatedConstraint — 8-byte base (parent + classdesc). Locally redeclared
// here so that eBodyWorldConstraint can inherit and so we can call the base
// Write() through a (const eSimulatedConstraint *) cast.
class eSimulatedConstraint {
public:
    char _pad[8];
    eSimulatedConstraint(cBase *);
    ~eSimulatedConstraint();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eBodyWorldConstraint : public eSimulatedConstraint {
public:
    eBodyWorldConstraint(cBase *);
    ~eBodyWorldConstraint();

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Initialize(ePhysicsConstraintConfig *, eSimulatedController *);
    void AssignCopy(const cBase *);
    void OnPositionChanged(void);
    const cType *GetType(void) const;

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

extern "C" {
    void eBodyWorldConstraint__eBodyWorldConstraint_cBaseptr(void *self, cBase *parent);
}

// -- eBodyWorldConstraint::eBodyWorldConstraint(cBase *) --  @ 0x0006b440, 92B
eBodyWorldConstraint::eBodyWorldConstraint(cBase *parent)
    : eSimulatedConstraint(parent) {
    *(void **)((char *)this + 4) = eBodyWorldConstraintvirtualtable;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x10) = 0;

    float z = 0.0f;
    int x;
    int y;
    int w;
    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(z));
    __asm__ volatile("mfc1 %0, %1" : "=r"(w) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x30(%3)\n"
        :
        : "r"(x), "r"(y), "r"(w), "r"(this)
        : "memory");
}

// -- eBodyWorldConstraint::Initialize(ePhysicsConstraintConfig *, eSimulatedController *) --  @ 0x0006b518, 152B
void eBodyWorldConstraint::Initialize(ePhysicsConstraintConfig *cfg, eSimulatedController *ctrl) {
    *(eSimulatedController **)((char *)this + 0x0C) = ctrl;
    *(ePhysicsConstraintConfig **)((char *)this + 0x08) = cfg;

    char *jointBase = *(char **)((char *)ctrl + 0x38);
    int index = *(int *)((char *)cfg + 0x54);
    char *joint = jointBase + index * 0x30;
    *(char **)((char *)this + 0x10) = joint;

    char *bodyMat;
    __asm__ volatile(
        "lw %0, 0x20(%1)\n"
        "lv.q C120, 0x30(%0)\n"
        "sv.q C120, 0x30(%2)\n"
        : "=r"(bodyMat)
        : "r"(joint), "r"(this)
        : "memory");

    char *cfgPos = (char *)cfg + 0x30;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "lw %0, 0x20(%2)\n"
        "lv.q C000, 0(%0)\n"
        "lv.q C010, 0x10(%0)\n"
        "lv.q C020, 0x20(%0)\n"
        "lv.q C030, 0x30(%0)\n"
        "vtfm3.t C130, E000, C120\n"
        : "=r"(bodyMat)
        : "r"(cfgPos), "r"(joint)
        : "memory");
    v4sf_t out = *(v4sf_t *)((char *)this + 0x30);
    __asm__ volatile("vadd.t %0, %0, C130" : "+v"(out));
    *(v4sf_t *)((char *)this + 0x30) = out;

    InitRecord *rec = (InitRecord *)(*(char **)((char *)ctrl + 4) + 0xA0);
    short off = rec->offset;
    char *target = (char *)ctrl + off;
    int callIndex = *(int *)((char *)cfg + 0x54);
    void *arg2 = (char *)this + 0x18;
    void *arg3 = (char *)this + 0x20;
    void (*fn)(void *, int, void *, void *) = rec->fn;
    fn(target, callIndex, arg2, arg3);
}

// ── eBodyWorldConstraint::Read(cFile &, cMemPool *) ──  @ 0x0006b384, 188B
#pragma control sched=1
int eBodyWorldConstraint::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eSimulatedConstraint::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
#pragma control sched=2

// ── eBodyWorldConstraint::GetType(void) const ──  @ 0x00209a08, 216B
#pragma control sched=1
const cType *eBodyWorldConstraint::GetType(void) const {
    if (D_00046BC8 == 0) {
        if (D_00046BC4 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00046BC4 = cType::InitializeType(0, 0, 0x25E, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00046BC4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20998C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BC8 = cType::InitializeType(0, 0, 0x25F, parentType, factory, 0, 0, 0);
    }
    return D_00046BC8;
}
#pragma control sched=2

// ── eBodyWorldConstraint::Write(cFile &) const ──  @ 0x0006b338, 76B
#pragma control sched=1
void eBodyWorldConstraint::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eSimulatedConstraint *)this)->Write(file);
    wb.End();
}
#pragma control sched=2

// ── eBodyWorldConstraint::AssignCopy(const cBase *) ──  @ 0x0020992c, 96B
//
// 56-byte field copy from the dcast result starting at offset 8:
//   0x08..0x10  — three int words (lw/sw)
//   0x14..0x18  — two FPU scalars (lwc1/swc1)
//   0x1C        — gap (not copied)
//   0x20, 0x30  — two V4SF quads (lv.q/sv.q)
#pragma control sched=1
void eBodyWorldConstraint::AssignCopy(const cBase *src) {
    eBodyWorldConstraint *other = dcast<eBodyWorldConstraint>(src);
    *(int *)((char *)this + 0x08) = *(int *)((char *)other + 0x08);
    *(int *)((char *)this + 0x0C) = *(int *)((char *)other + 0x0C);
    *(int *)((char *)this + 0x10) = *(int *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
}
#pragma control sched=2

// ── eBodyWorldConstraint::OnPositionChanged(void) ──  @ 0x0006b5b0, 68B
#pragma control sched=1
void eBodyWorldConstraint::OnPositionChanged(void) {
    char *base = *(char **)((char *)this + 0x10);
    char *firstMat;
    __asm__ volatile(
        "lw %0, 0x20(%1)\n"
        "lv.q C120, 0x30(%0)\n"
        "sv.q C120, 0x30(%2)\n"
        : "=r"(firstMat)
        : "r"(base), "r"(this)
        : "memory");
    char *bodyPos = *(char **)((char *)this + 0x08) + 0x30;
    char *mat;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "lw %0, 0x20(%2)\n"
        "lv.q C000, 0(%0)\n"
        "lv.q C010, 0x10(%0)\n"
        "lv.q C020, 0x20(%0)\n"
        "lv.q C030, 0x30(%0)\n"
        "vtfm3.t C130, E000, C120\n"
        : "=r"(mat)
        : "r"(bodyPos), "r"(base)
        : "memory");
    v4sf_t out = *(v4sf_t *)((char *)this + 0x30);
    __asm__ volatile("vadd.t %0, %0, C130" : "+v"(out));
    *(v4sf_t *)((char *)this + 0x30) = out;
}
#pragma control sched=2

// ── eBodyWorldConstraint::~eBodyWorldConstraint(void) ──  @ 0x0006b49c, 124B
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~eSimulatedConstraint(), and the deleting-tail dispatch
// via operator delete. The body just sets the classdesc pointer at offset 4
// to eBodyWorldConstraint's own table (the parent dtor will then overwrite
// it on the way down the chain).
#pragma control sched=1
eBodyWorldConstraint::~eBodyWorldConstraint() {
    *(void **)((char *)this + 4) = eBodyWorldConstraintvirtualtable;
}
#pragma control sched=2

// ── eBodyWorldConstraint::New(cMemPool *, cBase *) static ──  @ 0x0020998c, 124B
#pragma control sched=1
cBase *eBodyWorldConstraint::New(cMemPool *pool, cBase *parent) {
    eBodyWorldConstraint *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBodyWorldConstraint *obj = (eBodyWorldConstraint *)rec->fn(base, 0x40, 0x10, 0, 0);
    if (obj != 0) {
        eBodyWorldConstraint__eBodyWorldConstraint_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
