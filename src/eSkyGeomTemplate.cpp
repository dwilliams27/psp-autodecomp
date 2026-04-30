#include "mVec3.h"

// eSkyGeomTemplate — sky-geometry template object. 48-byte object derived
// from cBase. Payload starts at offset 8: a 4-byte handle/index field,
// a byte flag, a V4SF quad, and three FPU scalars.
//
// Functions matched here:
//   eSkyGeomTemplate::~eSkyGeomTemplate(void)             @ 0x0005cbfc
//   eSkyGeomTemplate::AssignCopy(const cBase *)           @ 0x00204250
//   eSkyGeomTemplate::New(cMemPool *, cBase *) static     @ 0x002042b0

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;
class cType;

// dcast<T>(const cBase *) — linker-resolved per-type template instantiation
// that downcasts via classdesc lookup.
template <class T> T *dcast(const cBase *);

// External constructor wrapper used by the already-matched ::New body.
extern "C" void eSkyGeomTemplate_eSkyGeomTemplate(void *self, cBase *parent);

// Pool-block layout (mirrors other class destructors / ::New).
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *,
                                 const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class eSkyGeomTemplate {
public:
    char _pad[0x30];

    eSkyGeomTemplate(cBase *);
    ~eSkyGeomTemplate();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = (void (*)(void *, void *))rec->fn;
        fn(base, p);
    }
};

extern const char eSkyGeomTemplate_base_name[];
extern const char eSkyGeomTemplate_base_desc[];
extern char eSkyGeomTemplateclassdesc[];

static cType *type_base;
static cType *type_eSkyGeomTemplate;

// ── eSkyGeomTemplate ──  @ 0x0005cba8, 84B
#pragma control sched=1
eSkyGeomTemplate::eSkyGeomTemplate(cBase *parent) {
    *(cBase **)this = parent;
    *(void **)((char *)this + 4) = eSkyGeomTemplateclassdesc;
    *(int *)((char *)this + 8) = 0;
    *(unsigned char *)((char *)this + 0x0C) = 0;

    float zero = 0.0f;
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
        "sv.q C120, 0x10(%3)\n"
        :
        : "r"(x), "r"(y), "r"(z), "r"(this));

    *(float *)((char *)this + 0x20) = zero;
    *(float *)((char *)this + 0x24) = zero;
    *(float *)((char *)this + 0x28) = 1.0f;
}
#pragma control sched=2

// ── ~eSkyGeomTemplate ──  @ 0x0005cbfc, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through operator delete on (flag & 1). The body just resets
// the classdesc pointer at offset 4 to the parent (cBase) classdesc address.
#pragma control sched=1
extern char cBaseclassdesc[];
eSkyGeomTemplate::~eSkyGeomTemplate() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
#pragma control sched=2

// ── AssignCopy ──  @ 0x00204250, 96B
//
// Field-wise copy from the dcast result starting at offset 8:
//   0x08      — int word (treated as a single-int object so SNC emits the
//               canonical struct-assign addiu/lw 0(a0)/sw 0(a1) sequence)
//   0x0C      — byte
//   0x10      — V4SF quad
//   0x20,0x24,0x28 — three FPU scalars
struct eSkyGeomHandle { int mIndex; };
#pragma control sched=1
void eSkyGeomTemplate::AssignCopy(const cBase *src) {
    eSkyGeomTemplate *other = dcast<eSkyGeomTemplate>(src);
    *(eSkyGeomHandle *)((char *)this + 0x08) = *(eSkyGeomHandle *)((char *)other + 0x08);
    *(unsigned char *)((char *)this + 0x0C) = *(unsigned char *)((char *)other + 0x0C);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(float *)((char *)this + 0x20) = *(float *)((char *)other + 0x20);
    *(float *)((char *)this + 0x24) = *(float *)((char *)other + 0x24);
    *(float *)((char *)this + 0x28) = *(float *)((char *)other + 0x28);
}
#pragma control sched=2

// ── New ──  @ 0x002042b0, 124B
//
// Pool-driven allocation through allocTable+0x28: size 0x30, alignment 0x10.
// On success, runs the constructor and returns the new object cast to cBase*.
#pragma control sched=1
cBase *eSkyGeomTemplate::New(cMemPool *pool, cBase *parent) {
    eSkyGeomTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocEntry *rec = (AllocEntry *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSkyGeomTemplate *obj = (eSkyGeomTemplate *)rec->fn(base, 0x30, 0x10, 0, 0);
    if (obj != 0) {
        eSkyGeomTemplate_eSkyGeomTemplate(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// ── GetType ──  @ 0x0020432c, 156B
#pragma control sched=1
const cType *eSkyGeomTemplate::GetType(void) const {
    if (!type_eSkyGeomTemplate) {
        if (!type_base) {
            const char *name = eSkyGeomTemplate_base_name;
            const char *desc = eSkyGeomTemplate_base_desc;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            type_base = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *base = type_base;
        __asm__ volatile("" : "+r"(base));
        cBase *(*factory)(cMemPool *, cBase *) = &eSkyGeomTemplate::New;
        __asm__ volatile("" : "+r"(factory));
        type_eSkyGeomTemplate = cType::InitializeType(0, 0, 0x1BB, base,
                                                      factory, 0, 0, 0);
    }
    return type_eSkyGeomTemplate;
}
#pragma control sched=2

// ── Write ──  @ 0x0005ca04, 128B
#pragma control sched=1
void eSkyGeomTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cHandle *)((const char *)this + 0x08))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x0C));
    wb.Write(3, (const float *)((const char *)this + 0x10));
    wb.Write(*(const float *)((const char *)this + 0x20));
    wb.Write(*(const float *)((const char *)this + 0x24));
    wb.Write(*(const float *)((const char *)this + 0x28));
    wb.End();
}
#pragma control sched=2

class eSphereCast {
public:
    eSphereCast(const mVec3 &, const mVec3 &, float, unsigned int, int);
};

// ── eSphereCast ──  @ 0x0024908c, 240B
eSphereCast::eSphereCast(const mVec3 &origin, const mVec3 &end, float radius,
                         unsigned int mask, int flags) {
    *(unsigned int *)((char *)this + 0x00) = mask;
    *(int *)((char *)this + 0x04) = 0;
    *(unsigned char *)((char *)this + 0x18) = 1;
    *(unsigned char *)((char *)this + 0x19) = 0;
    *(float *)((char *)this + 0x20) = 3.4028235e38f;

    float zero = 0.0f;
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
        "sv.q C120, 0x30(%3)\n"
        :
        : "r"(x), "r"(y), "r"(z), "r"(this));

    int one;
    __asm__ volatile("mfc1 %0, %1" : "=r"(x) : "f"(zero));
    __asm__ volatile("mfc1 %0, %1" : "=r"(y) : "f"(zero));
    float onef = 1.0f;
    __asm__ volatile("mfc1 %0, %1" : "=r"(one) : "f"(onef));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x40(%3)\n"
        :
        : "r"(x), "r"(y), "r"(one), "r"(this));

    *(int *)((char *)this + 0x50) = -1;
    *(int *)((char *)this + 0x54) = -1;
    *(int *)((char *)this + 0x58) = 0;
    *(int *)((char *)this + 0x5C) = 0;
    *(int *)((char *)this + 0x60) = flags;

    *(v4sf_t *)((char *)this + 0x70) = *(const v4sf_t *)&origin;
    *(v4sf_t *)((char *)this + 0x80) = *(const v4sf_t *)&end;
    *(float *)((char *)this + 0x90) = radius;
    *(v4sf_t *)((char *)this + 0xA0) = *(const v4sf_t *)&origin;
    *(float *)((char *)this + 0xAC) = radius;
    *(v4sf_t *)((char *)this + 0xB0) = *(const v4sf_t *)&origin;
    *(v4sf_t *)((char *)this + 0xC0) = *(const v4sf_t *)&end;

    int length_bits;
    __asm__ volatile(
        "lv.q C130, 0x0(%1)\n"
        "vsub.t C120, C120, C130\n"
        "sv.q C120, 0xD0(%2)\n"
        "vdot.t S100, C120, C120\n"
        "vcmp.s ez, S100\n"
        "vrsq.s S101, S100\n"
        "vsqrt.s S102, S100\n"
        "vpfxs 1, Y, Z, W\n"
        "vcmovt.s S101, S100, 0\n"
        "vscl.t C120, C120, S101\n"
        "sv.q C120, 0xD0(%2)\n"
        "mfv %0, S102\n"
        : "=r"(length_bits)
        : "r"(&origin), "r"(this));
    *(float *)((char *)this + 0xE0) = *(float *)&length_bits;
}
