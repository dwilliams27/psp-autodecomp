// ePoint.cpp — decompiled from eAll_psp.obj
// Parent: cObject. Inherits cObject deleting-destructor chain via SNC ABI.
//
// Functions:
//   0x0005c13c  ePoint::Write(cFile &) const       (152B)
//   0x0005c424  ePoint::~ePoint(void)               (124B)
//   0x00203b18  ePoint::AssignCopy(const cBase *)   (112B)
//   0x00203b88  ePoint::New(cMemPool *, cBase *)    (124B)

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cFilename;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(int, const float *);
    void End(void);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

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

typedef int v4sf_t __attribute__((mode(V4SF)));

class ePoint : public cObject {
public:
    ePoint(cBase *);
    ~ePoint(void);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char ePoint_cBase_vtable[];   // 0x382010

// ── ePoint::Write(cFile &) const @ 0x0005c13c ──
#pragma control sched=1
void ePoint::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cObject::Write(file);
    wb.Write(3, (const float *)((const char *)this + 0x80));
    wb.Write(3, (const float *)((const char *)this + 0x50));
    wb.Write(3, (const float *)((const char *)this + 0x60));
    wb.Write(3, (const float *)((const char *)this + 0x70));
    wb.Write(*(const unsigned int *)((const char *)this + 0x90));
    wb.End();
}
#pragma control sched=2

// ── ePoint::~ePoint(void) @ 0x0005c424 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the ~cObject() base-chain call (with flags=0), and the deleting-tail
// `if (flags & 1) cMemPool_dispatch(this)` epilogue. Transition-zone sched=1.
//
// NOTE: 17/124-byte residual diff in the inlined operator-delete tail —
// our SNC's deleting-destructor synthesis allocates regs a2/a3 with
// `move a1,s1` scheduled early, while the original used a1/a2 with
// `addu a0,a0,a2` before the fn-load and `move a1,s1` in the delay slot.
// Multiple operator delete formulations and sched-pragma combinations
// produce identical canonical (a2/a3) codegen — likely a compiler-version
// divergence in the synthesized D0 thunk, analogous to bnel divergence.
#pragma control sched=1
ePoint::~ePoint(void) {
    *(void **)((char *)this + 4) = ePoint_cBase_vtable;
}
#pragma control sched=2

struct ePointTail { int mField; };

// ── ePoint::AssignCopy(const cBase *) @ 0x00203b18 ──
#pragma control sched=1
void ePoint::AssignCopy(const cBase *base) {
    ePoint *src = dcast<ePoint>(base);
    cObject::operator=(*src);
    *(v4sf_t *)((char *)this + 0x80) = *(v4sf_t *)((char *)src + 0x80);
    *(v4sf_t *)((char *)this + 0x50) = *(v4sf_t *)((char *)src + 0x50);
    *(v4sf_t *)((char *)this + 0x60) = *(v4sf_t *)((char *)src + 0x60);
    *(v4sf_t *)((char *)this + 0x70) = *(v4sf_t *)((char *)src + 0x70);
    __asm__ volatile("" ::: "memory");
    *(ePointTail *)((char *)this + 0x90) = *(const ePointTail *)((char *)src + 0x90);
}
#pragma control sched=2

// ── ePoint::New(cMemPool *, cBase *) static @ 0x00203b88 ──
// Uses sched=1 with the surrounding ePoint class cluster in eAll_psp.
#pragma control sched=1
cBase *ePoint::New(cMemPool *pool, cBase *parent) {
    ePoint *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    ePoint *obj = (ePoint *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) ePoint(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
