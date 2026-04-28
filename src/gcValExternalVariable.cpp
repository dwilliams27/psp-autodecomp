// gcValExternalVariable — gcLValue subclass wrapping a cHandle reference
// to an external (designer-named) variable.
//
// Functions matched here:
//   gcValExternalVariable::Write(cFile &) const               @ 0x003430ac  (gcAll_psp.obj)
//   gcValExternalVariable::New(cMemPool *, cBase *) static    @ 0x00342ed0
//   gcValExternalVariable::~gcValExternalVariable(void)       @ 0x00343698

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct PoolBlock {
    char pad[0x1C];
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

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcLValue {
public:
    cBase *mParent;       // 0x0
    void *mVtable;        // 0x4
    gcLValue(cBase *parent);
    void Write(cFile &) const;
};

class gcValExternalVariable : public gcLValue {
public:
    cHandle mHandle;      // 0x8
    gcValExternalVariable(cBase *parent);
    ~gcValExternalVariable();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
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

extern char gcLValuevirtualtable[];
extern char gcValExternalVariablevirtualtable[];

inline gcLValue::gcLValue(cBase *parent) {
    mVtable = gcLValuevirtualtable;
    mParent = parent;
}

inline gcValExternalVariable::gcValExternalVariable(cBase *parent) : gcLValue(parent) {
    mVtable = gcValExternalVariablevirtualtable;
    mHandle.mIndex = 0;
}

inline void *operator new(unsigned, void *p) { return p; }

// ─────────────────────────────────────────────────────────────────────────
// gcValExternalVariable::Write(cFile &) const  @ 0x003430ac, 88B
// ─────────────────────────────────────────────────────────────────────────
void gcValExternalVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    mHandle.Write(wb);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcValExternalVariable::New(cMemPool *, cBase *) static  @ 0x00342ed0, 136B
// ─────────────────────────────────────────────────────────────────────────
cBase *gcValExternalVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValExternalVariable *result = 0;
    gcValExternalVariable *obj = (gcValExternalVariable *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValExternalVariable(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValExternalVariable::~gcValExternalVariable(void)  @ 0x00343698, 100B
// ─────────────────────────────────────────────────────────────────────────
gcValExternalVariable::~gcValExternalVariable() {
    *(void **)((char *)this + 4) = gcLValuevirtualtable;
}
