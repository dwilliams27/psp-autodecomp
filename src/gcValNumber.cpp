// Days of Thunder decompilation: gcValNumber
//   gcValNumber::gcValNumber(cBase *, float)            @ 0x0012f0e4 (28B)
//   gcValNumber::Read(cFile &, cMemPool *)              @ 0x0012f014 (208B)
//   gcValNumber::~gcValNumber(void)                     @ 0x0012f100 (100B)
//   gcValNumber::Write(cFile &) const                   @ ~ (88B)
//   gcValNumber::AssignCopy(const cBase *)              @ ~ (48B)
//   gcValNumber::GetText(char *) const                  @ ~ (88B)
//   gcValNumber::New(cMemPool *, cBase *) static        @ 0x0027a924 (132B)
//
// Same shape as gcValVariable: a gcLValue-derived leaf class with one
// extra word at offset 8 holding the carried value. Self-contained TU
// (no header include) following the project's established pattern for
// classes whose deleting-destructor needs an inline operator delete.

class cBase;
class cFile;
class cMemPool;
class cType;

// ──────────────────────────────────────────────────────────────────────────
// Engine-side helper types used by Write/Read/New/dtor below.
// ──────────────────────────────────────────────────────────────────────────

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cStr {
public:
    char _data[256];
    cStr(const char *fmt, ...);
    operator const char *() const { return _data; }
};

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

// gcValue is the abstract parent of gcValNumber. We only declare its
// methods here for the chained-call sites in gcValNumber::Read; the
// parent/vtable storage at offsets 0/4 is described directly on
// gcValNumber so that EBO keeps everything at the right offsets.
class gcValue {
public:
    void Write(cFile &) const;
    int  Read(cFile &, cMemPool *);
};

class gcValNumber : public gcValue {
public:
    int   pad0;           // 0x0  cBase *mParent
    int   pad4;           // 0x4  vtable slot
    float mValue;         // 0x8

    gcValNumber(cBase *, float);
    ~gcValNumber();

    void  Write(cFile &) const;
    void  GetText(char *) const;
    void  AssignCopy(const cBase *);
    float Evaluate(void) const;
    const cType *GetType(void) const;
    int   Read(cFile &, cMemPool *);

    static cBase *New(cMemPool *, cBase *);

    // Inlined into the deleting-destructor variant of ~gcValNumber.
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

inline void *operator new(unsigned, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

// External symbols / helpers
void  cStrCat(char *, const char *);
gcValNumber *dcast(const cBase *);
void  gcValue_Write(const gcValNumber *, cFile &);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);

extern const char gcValNumber_fmt[];
extern char gcValNumbervirtualtable[];
extern char cBaseclassdesc[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValNumber asm("D_0009F4B4");

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::gcValNumber(cBase *, float)  @ 0x0012f0e4, 28B
// ──────────────────────────────────────────────────────────────────────────
gcValNumber::gcValNumber(cBase *base, float val) {
    this->pad0 = (int)base;
    this->pad4 = 0x38A468;   // gcValNumbervirtualtable (literal in unity build)
    this->mValue = val;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::AssignCopy(const cBase *)  @ 48B
// ──────────────────────────────────────────────────────────────────────────
void gcValNumber::AssignCopy(const cBase *base) {
    gcValNumber *other = dcast(base);
    this->mValue = other->mValue;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::Write(cFile &) const  @ 88B
// ──────────────────────────────────────────────────────────────────────────
void gcValNumber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    wb.Write(this->mValue);
    wb.End();
}

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::GetText(char *) const  @ 88B
// ──────────────────────────────────────────────────────────────────────────
void gcValNumber::GetText(char *buf) const {
    cStrCat(buf, cStr(gcValNumber_fmt, (double)this->mValue));
}

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::~gcValNumber(void)  @ 0x0012f100, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard,
// the absence of a parent-destructor chain (gcValue's dtor is empty), and
// the deleting-tail dispatch through operator delete on (flag & 1). Body
// just resets the classdesc pointer at offset 4 to the parent (cBase)
// classdesc.
// ──────────────────────────────────────────────────────────────────────────
gcValNumber::~gcValNumber() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::New(cMemPool *, cBase *) static  @ 0x0027a924, 132B
//
// Allocate a 12-byte block from pool slot 0x28 and placement-new a
// gcValNumber(parent, 0.0f) into it.
// ──────────────────────────────────────────────────────────────────────────
cBase *gcValNumber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValNumber *result = 0;
    gcValNumber *obj = (gcValNumber *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValNumber(parent, 0.0f);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValNumber::GetType(void) const {
    if (!type_gcValNumber) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValNumber = cType::InitializeType(0, 0, 0x72, type_value,
                                                 gcValNumber::New,
                                                 0, 0, 0);
    }
    return type_gcValNumber;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValNumber::Read(cFile &, cMemPool *)  @ 0x0012f014, 208B
//
// Open a tag-1 cReadBlock, dispatch to the parent gcValue::Read, then
// raw-read 4 bytes (the float) from the current file handle into mValue.
// On any failure, rewind via cFile_SetCurrentPos and return 0.
// ──────────────────────────────────────────────────────────────────────────
int gcValNumber::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 8, 4);
    }
    return result;
}
