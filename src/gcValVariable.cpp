// Days of Thunder decompilation: gcValVariable
//   gcValVariable::Write(cFile &) const                  @ 0x0036abbc
//   gcValVariable::New(cMemPool *, cBase *) static       @ 0x0036a9e0
//   gcValVariable::~gcValVariable(void)                  @ 0x0036b1c4
//
// Same shape as gcValHeadset (gcLValue-derived class with one extra word
// at offset 8). gcValVariable holds a cHandle at offset 8 instead of a
// plain int.

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

class gcValVariable : public gcLValue {
public:
    cHandle mHandle;      // 0x8
    gcValVariable(cBase *parent);
    ~gcValVariable();
    const cType *GetType(void) const;
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
extern char gcValVariablevirtualtable[];

inline gcLValue::gcLValue(cBase *parent) {
    mVtable = gcLValuevirtualtable;
    mParent = parent;
}

inline gcValVariable::gcValVariable(cBase *parent) : gcLValue(parent) {
    mVtable = gcValVariablevirtualtable;
    mHandle.mIndex = 0;
}

inline void *operator new(unsigned, void *p) { return p; }

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_variable asm("D_0009F3EC");
static cType *type_gcValVariable asm("D_0009F920");

// ─────────────────────────────────────────────────────────────────────────
// gcValVariable::GetType(void) const  @ 0x0036aa68, 340B
// ─────────────────────────────────────────────────────────────────────────
const cType *gcValVariable::GetType(void) const {
    if (!type_gcValVariable) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValVariable = cType::InitializeType(
            0, 0, 0x9D, type_variable, gcValVariable::New, 0, 0, 0);
    }
    return type_gcValVariable;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValVariable::Write(cFile &) const  @ 0x0036abbc, 88B
// ─────────────────────────────────────────────────────────────────────────
void gcValVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    mHandle.Write(wb);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcValVariable::New(cMemPool *, cBase *) static  @ 0x0036a9e0, 136B
// ─────────────────────────────────────────────────────────────────────────
cBase *gcValVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValVariable *result = 0;
    gcValVariable *obj = (gcValVariable *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValVariable(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValVariable::~gcValVariable(void)  @ 0x0036b1c4, 100B
// ─────────────────────────────────────────────────────────────────────────
gcValVariable::~gcValVariable() {
    *(void **)((char *)this + 4) = gcLValuevirtualtable;
}
