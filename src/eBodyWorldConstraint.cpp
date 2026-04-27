// eBodyWorldConstraint — joint constraint that pins a rigid body to a fixed
// world transform. Inherits from eSimulatedConstraint (8-byte base: parent +
// classdesc). Body fields begin at offset 8 — three int slots, two scalar
// floats, then two 16-byte (V4SF) blocks at 0x20 and 0x30.
//
// Functions matched here:
//   eBodyWorldConstraint::Write(cFile &) const           @ 0x0006b338  (eAll_psp.obj)
//   eBodyWorldConstraint::AssignCopy(const cBase *)      @ 0x0020992c  (eAll_psp.obj)
//   eBodyWorldConstraint::~eBodyWorldConstraint(void)    @ 0x0006b49c  (eAll_psp.obj)

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;

extern char eBodyWorldConstraintvirtualtable[];

// cWriteBlock — RAII block-tag helper used by Write().
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
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
};

class eBodyWorldConstraint : public eSimulatedConstraint {
public:
    eBodyWorldConstraint(cBase *);
    ~eBodyWorldConstraint();

    void Write(cFile &) const;
    void AssignCopy(const cBase *);

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
