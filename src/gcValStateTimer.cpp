// Days of Thunder decompilation: gcValStateTimer
//   gcValStateTimer::AssignCopy(const cBase *)           @ 0x0035b78c (28B)
//   gcValStateTimer::New(cMemPool *, cBase *) static     @ 0x0035b7a8 (132B)
//   gcValStateTimer::~gcValStateTimer(void)              @ 0x0035b980 (100B)
//   gcValStateTimer::Write(cFile &) const                @ 0x0035b9e4 (76B)
//   gcValStateTimer::Evaluate(void) const                @ 0x0035baec (52B)
//   gcValStateTimer::GetText(char *) const               @ 0x0035bb84 (40B)
//
// gcValStateTimer is a leaf cBase-derived class with no extra payload —
// just the inherited (mParent, mClassdesc) pair at offsets 0/4. New
// allocates 8 bytes from the mempool and placement-news a gcValStateTimer
// (parent ctor stores the cBase classdesc into mClassdesc; the derived
// ctor immediately overwrites it with gcValStateTimer's own vtable).
// The destructor restores the cBase classdesc and dispatches operator
// delete on the deleting-tail flag.

class cBase;
class cFile;
class cMemPool;

extern char cBaseclassdesc[];                   // @ 0x37E6A8
extern char gcValStateTimervirtualtable[];      // @ 0x003978E0

// ──────────────────────────────────────────────────────────────────────────
// Pool-allocator helper layout (matches gcValVariable / gcValNumber).
// ──────────────────────────────────────────────────────────────────────────
struct PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

// ──────────────────────────────────────────────────────────────────────────
// cBase header subset. We model it as an explicit base so SNC emits the
// parent ctor's mClassdesc store before the derived ctor overwrites it.
// ──────────────────────────────────────────────────────────────────────────
class cBaseLayout {
public:
    cBase *mParent;        // 0x0
    void  *mClassdesc;     // 0x4
    cBaseLayout(cBase *parent) {
        mClassdesc = cBaseclassdesc;
        mParent = parent;
    }
};

class gcValStateTimer : public cBaseLayout {
public:
    gcValStateTimer(cBase *parent);
    ~gcValStateTimer();
    void  AssignCopy(const cBase *);
    void  GetText(char *) const;
    float Evaluate(void) const;
    void  Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    // Inlined into the deleting-destructor variant of ~gcValStateTimer.
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

inline gcValStateTimer::gcValStateTimer(cBase *parent) : cBaseLayout(parent) {
    mClassdesc = gcValStateTimervirtualtable;
}

inline void *operator new(unsigned, void *p) { return p; }

// External free-helpers.
extern gcValStateTimer *dcast(const cBase *);
extern void cStrCat(char *, const char *);
extern void gcLValue_Write(const gcValStateTimer *, cFile &);

// ──────────────────────────────────────────────────────────────────────────
// gcValStateTimer::AssignCopy(const cBase *)  @ 0x0035b78c, 28B
// ──────────────────────────────────────────────────────────────────────────
void gcValStateTimer::AssignCopy(const cBase *base) {
    dcast(base);
}

// ──────────────────────────────────────────────────────────────────────────
// gcValStateTimer::Evaluate(void) const  @ 0x0035baec, 52B
// ──────────────────────────────────────────────────────────────────────────
float gcValStateTimer::Evaluate(void) const {
    char *p = *(char **)0x37D864;
    if (p == 0) {
        return 0.0f;
    }
    p += 24;
    return *(float *)0x36C800 * (float)*(int *)p;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValStateTimer::GetText(char *) const  @ 0x0035bb84, 40B
// ──────────────────────────────────────────────────────────────────────────
void gcValStateTimer::GetText(char *buf) const {
    cStrCat(buf, (const char *)0x36F760);
}

// ──────────────────────────────────────────────────────────────────────────
// gcValStateTimer::Write(cFile &) const  @ 0x0035b9e4, 76B
// ──────────────────────────────────────────────────────────────────────────
void gcValStateTimer::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    wb.End();
}

// ──────────────────────────────────────────────────────────────────────────
// gcValStateTimer::~gcValStateTimer(void)  @ 0x0035b980, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard,
// the absence of a parent-destructor chain (cBase has none), and the
// deleting-tail dispatch through operator delete on (flag & 1). Body
// just resets the classdesc pointer at offset 4 to the parent (cBase)
// classdesc.
// ──────────────────────────────────────────────────────────────────────────
gcValStateTimer::~gcValStateTimer() {
    mClassdesc = cBaseclassdesc;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValStateTimer::New(cMemPool *, cBase *) static  @ 0x0035b7a8, 132B
//
// Allocate an 8-byte block from pool slot 0x28 and placement-new a
// gcValStateTimer(parent) into it.
// ──────────────────────────────────────────────────────────────────────────
cBase *gcValStateTimer::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValStateTimer *result = 0;
    gcValStateTimer *obj = (gcValStateTimer *)e->fn(base, 0x8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValStateTimer(parent);
        result = obj;
    }
    return (cBase *)result;
}
