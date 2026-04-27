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

// dcast<T>(const cBase *) — linker-resolved per-type template instantiation
// that downcasts via classdesc lookup.
template <class T> T *dcast(const cBase *);

// External constructor (defined elsewhere; eSkyGeomTemplate::eSkyGeomTemplate
// @ 0x0005cba8, not yet matched). Invoked from ::New via safe-name wrapper.
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

class eSkyGeomTemplate {
public:
    char _pad[0x30];

    ~eSkyGeomTemplate();
    void AssignCopy(const cBase *);
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
