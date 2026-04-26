// eSound.cpp — decompiled from eAll_psp.obj
// Parent: cObject. Vtable at 0x37FB80 (eSoundvirtualtable).
//
// Functions:
//   eSound::Write(cFile &) const                @ 0x000211b0  (40B)
//   eSound::Read(cFile &, cMemPool *)           @ 0x000211d8  (120B)
//   eSound::eSound(cBase *)                     @ 0x00021250  (120B)
//   eSound::~eSound(void)                       @ 0x000212c8  (140B)
//   eSound::New(cMemPool *, cBase *) static     @ 0x001e0250  (124B)
//
// cObject is declared with INLINE ctor/dtor bodies so SNC inlines them into
// eSound's ctor (leaf, no jal) and dtor (cObject vtable written inline at
// the chain point, no jal to ~cObject). Compare with eSoundData.cpp where
// cObject is non-inline and the ctor/dtor each emit a jal to cObject's
// implementation in cAll_psp.obj.

class cBase;
class cFile;
class cMemPool;
class eSound;
class eWorld;

extern char eSoundvirtualtable[];      // 0x37FB80
extern char cObjectvirtualtable[];     // 0x37E990

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

// Inline cObject — bodies visible so SNC inlines its trivial codegen.
// Explicit data members give cObject 8 bytes (no EBCO) so derived fields
// land at the correct offsets.
class cObject {
public:
    cBase *mParent;     // 0x00
    void *mVtbl;        // 0x04
    // cObject ctor only stores parent — derived class writes vtable in body.
    // (The "real" cObject::cObject in cAll_psp.obj also writes cObjectvtable,
    // but SNC reliably dead-stores it across an external jal in eSoundData;
    // when inlined here, the dead-store is not eliminated. Match the actual
    // emitted instruction stream by simply not writing it.)
    cObject(cBase *parent) {
        mParent = parent;
    }
    ~cObject() {
        mVtbl = cObjectvirtualtable;
    }
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
    ~cReadBlock();
};

class eSound : public cObject {
public:
    eWorld *mWorld;        // 0x08
    short mPad12;          // 0x0C
    short mPad14;          // 0x0E
    float mF10;            // 0x10  (sv.q quad init; offset 0x1C left unset)
    float mF14;            // 0x14
    float mF18;            // 0x18
    int mF1C;              // 0x1C  (uninitialized in ctor — sv.q's 4th lane)
    int mField20;          // 0x20
    int mField24;          // 0x24
    int mField28;          // 0x28
    int mField2C;          // 0x2C
    short mChannelIdx;     // 0x30
    short mField32;        // 0x32
    float mField34;        // 0x34
    float mField38;        // 0x38
    float mField3C;        // 0x3C
    float mField40;        // 0x40

    eSound(cBase *);
    ~eSound();
    static eSound *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

class eWorld {
public:
    void RemoveSound(eSound *);
};

// Placement-new stub for eSound::New ctor invocation.
inline void *operator new(unsigned int, void *p) { return p; }

// ── eSound::Write(cFile &) const @ 0x000211b0 ──
void eSound::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

// ── eSound::Read(cFile &, cMemPool *) @ 0x000211d8 ──
// Versioned read. Only version 1 is supported. On mismatch, seeks back via
// cFile::SetCurrentPos and returns 0. The `result = 1` constant is forced
// into a callee-saved register (s0) via inline asm so it survives the
// ~cReadBlock dtor call on the success path — same trick used in
// eSoundData::Read.
int eSound::Read(cFile &file, cMemPool *) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// ── eSound::eSound(cBase *) @ 0x00021250 ──
// Leaf ctor. cObject() is inlined (parent at 0, cObjectvtable at 4 — the
// latter dead-stored when eSoundvtable is written below). The 16-byte block
// at offset 0x10 is initialized via the SNC mtc1/mfc1×3 / mtv×3 / sv.q
// VFPU quad-store pattern; only S120/S121/S122 are explicitly zeroed
// (S123 is left at whatever the VFPU register held — offset 0x1C is
// uninitialized in this constructor).
eSound::eSound(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eSoundvirtualtable;
    mWorld = 0;
    mPad12 = 0;
    mPad14 = -1;
    {
        float zf = 0.0f;
        int z1, z2, z3;
        __asm__ volatile("mfc1 %0, %1" : "=r"(z1) : "f"(zf));
        __asm__ volatile("mfc1 %0, %1" : "=r"(z2) : "f"(zf));
        __asm__ volatile("mfc1 %0, %1" : "=r"(z3) : "f"(zf));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x10(%3)\n"
            :: "r"(z1), "r"(z2), "r"(z3), "r"(this)
            : "memory"
        );
    }
    mField20 = 0;
    mField24 = 0;
    mField28 = 0;
    mField2C = 0;
    mChannelIdx = -1;
    mField32 = -1;
    mField34 = 0.0f;
    mField38 = 1.0f;
    mField3C = 1.0f;
    mField40 = 0.0f;
}

// ── eSound::~eSound() @ 0x000212c8 ──
// Sets eSound vtable, then unregisters from world if owned. SNC ABI auto-
// generates the `(this != 0)` guard, the chain to ~cObject (inlined since
// cObject's dtor is inline-defined above — emits the cObjectvtable write),
// and the deleting-tail `if (flag & 1) operator delete(this)` epilogue.
eSound::~eSound() {
    *(void **)((char *)this + 4) = eSoundvirtualtable;
    if (mWorld != 0) {
        mWorld->RemoveSound(this);
    }
}

// ── eSound::New(cMemPool *, cBase *) static @ 0x001e0250 ──
// Standard SNC factory. Pool->[9] is the factory-block; block->[7] is the
// per-class allocator/deleter table; +0x28 is the alloc record (vs +0x30
// for delete in operator delete). Allocator takes 5 args (size, align,
// flag1, flag2 — flag2 lands in $t0 via SNC's calling convention).
eSound *eSound::New(cMemPool *pool, cBase *parent) {
    eSound *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eSound *obj = (eSound *)entry->fn(base, 0x50, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eSound(parent);
        result = obj;
    }
    return result;
}
