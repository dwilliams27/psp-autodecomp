// gcValEventVar — event-bound LValue.
//
// Functions matched here:
//   gcValEventVar::AssignCopy(const cBase *)             @ 0x0034282c  (gcAll_psp.obj)
//   gcValEventVar::Write(cFile &) const                  @ 0x00342aac  (gcAll_psp.obj)
//   gcValEventVar::New(cMemPool *, cBase *) static       @ 0x00342864  (gcAll_psp.obj)
//   gcValEventVar::~gcValEventVar(void)                  @ 0x00342a48  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcValEventVarclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cBase {
public:
    cBase *mOwner;          // 0
    void *mClassDesc;       // 4

    cBase(cBase *owner) {
        mClassDesc = cBaseclassdesc;
        mOwner = owner;
    }
};

class gcLValue : public cBase {
public:
    gcLValue(cBase *parent) : cBase(parent) {}
    void Write(cFile &) const;
};

class gcValEventVar : public gcLValue {
public:
    int mField08;       // 0x08
    int mFieldC;        // 0x0C

    gcValEventVar(cBase *parent) : gcLValue(parent) {
        mClassDesc = gcValEventVarclassdesc;
        mField08 = 1;
        mFieldC = 0;
    }
    ~gcValEventVar();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static gcValEventVar *New(cMemPool *, cBase *);

    // Inlined into the deleting-destructor variant.
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

template <class T> T *dcast(const cBase *);

// ── AssignCopy ──  @ 0x0034282c, 56B
void gcValEventVar::AssignCopy(const cBase *base) {
    gcValEventVar *src = dcast<gcValEventVar>(base);
    mField08 = src->mField08;
    mFieldC = src->mFieldC;
}

// ── Write ──  @ 0x00342aac, 100B
void gcValEventVar::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    wb.Write(mField08);
    wb.Write(mFieldC);
    wb.End();
}

// ── ~gcValEventVar ──  @ 0x00342a48, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard and
// the deleting-tail dispatch through operator delete on (flag & 1). Body
// resets the classdesc pointer at offset 4 to the parent (cBase) classdesc.
gcValEventVar::~gcValEventVar() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ── New ──  @ 0x00342864, 144B
gcValEventVar *gcValEventVar::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEventVar *result = 0;
    gcValEventVar *obj = (gcValEventVar *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEventVar(parent);
        result = obj;
    }
    return result;
}
