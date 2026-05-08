// gcValExternalVariable — gcLValue subclass wrapping a cHandle reference
// to an external (designer-named) variable.
//
// Functions matched here:
//   gcValExternalVariable::Write(cFile &) const               @ 0x003430ac  (gcAll_psp.obj)
//   gcValExternalVariable::New(cMemPool *, cBase *) static    @ 0x00342ed0
//   gcValExternalVariable::~gcValExternalVariable(void)       @ 0x00343698
//   gcValExternalVariable::GetText(char *) const              @ 0x003432d4

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cType;

class gcValPointValue {
public:
    void GetText(char *) const;
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
    ~cReadBlock(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

class gcLValue {
public:
    cBase *mParent;       // 0x0
    void *mVtable;        // 0x4
    gcLValue(cBase *parent);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcValExternalVariable : public gcLValue {
public:
    cHandle mHandle;      // 0x8
    gcValExternalVariable(cBase *parent);
    ~gcValExternalVariable();
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
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

class gcValue {
public:
    int Read(cFile &, cMemPool *);
};

// ODR-WARNING: split-TU method addition for gcValEnumerationEntry. Keep this
// local redeclaration minimal so existing matched siblings do not drift.
class gcValEnumerationEntry : public gcValue {
public:
    int Read(cFile &, cMemPool *);
};

extern char gcLValuevirtualtable[];
extern char gcValExternalVariablevirtualtable[];
extern void *D_00038890[];

void cStrCat(char *, const char *);

struct HandleObject {
    char pad0[8];
    char name[20];
    short nameLen;
    char pad1[18];
    int handle;
};

struct cTypeTextMethod {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

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
// gcValExternalVariable::Read(cFile &, cMemPool *)  @ 0x00343104, 220B
// ─────────────────────────────────────────────────────────────────────────
int gcValExternalVariable::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcLValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    mHandle.mIndex = 0;
    cHandle *handle = &mHandle;
    handle->Read(rb, cMemPool::GetPoolFromPtr(handle));
    return result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValEnumerationEntry::Read(cFile &, cMemPool *)  @ 0x00342184, 236B
// ─────────────────────────────────────────────────────────────────────────
int gcValEnumerationEntry::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        char *mType = *(char **)((char *)this + 12);
        char *base = (char *)this + 8;
        cTypeMethod *e = (cTypeMethod *)(mType + 48);
        typedef void (*ReadFn)(void *, cFile *, cMemPool *);
        ((ReadFn)e->fn)(base + e->offset, *(cFile **)&rb._data[0],
                        cMemPool::GetPoolFromPtr(base));
    }
    return result;
}

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

// ─────────────────────────────────────────────────────────────────────────
// gcValExternalVariable::GetText(char *) const  @ 0x003432d4, 172B
// ─────────────────────────────────────────────────────────────────────────
void gcValExternalVariable::GetText(char *buf) const {
    int h = mHandle.mIndex;
    HandleObject *obj;
    if (h == 0) {
        obj = 0;
    } else {
        HandleObject *cand = (HandleObject *)D_00038890[h & 0xFFFF];
        obj = 0;
        if (cand != 0) {
            if (cand->handle == h) {
                obj = cand;
            }
        }
    }

    const char *text;
    if (obj != 0) {
        unsigned int empty = (obj->nameLen == 0);
        empty &= 0xFF;
        if (empty) {
            text = (const char *)0x36DAB8;
            goto object_done;
        }
        text = (const char *)obj + 8;
object_done:
        __asm__ volatile("" : : "r"(text));
        goto append;
    } else if (h != 0) {
        text = (const char *)0x36DAC4;
    } else {
        text = (const char *)0x36DACC;
    }
append:
    cStrCat(buf, text);
}

// ODR-WARNING: split-TU method addition for gcValPointValue. Keep this local
// redeclaration minimal so existing matched gcValPointValue siblings do not drift.
// ─────────────────────────────────────────────────────────────────────────
// gcValPointValue::GetText(char *) const  @ 0x003577e8, 176B
// ─────────────────────────────────────────────────────────────────────────
void gcValPointValue::GetText(char *buf) const {
    const char *space = (const char *)0x36DAF0;
    cStrCat(buf, space);

    if (*(const int *)((const char *)this + 8) == 0) {
        cStrCat(buf, (const char *)0x36E6DC);
        char *base = (char *)this + 0x10;
        const cTypeTextMethod *method =
            (const cTypeTextMethod *)(*(const char **)((const char *)this + 0x14) + 0x78);
        method->fn(base + method->offset, buf);
        cStrCat(buf, (const char *)0x36DCEC);
    }

    cStrCat(buf, (const char *)0x36DADC);
    cStrCat(buf, space);
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValExternalVariable;

const cType *gcValExternalVariable::GetType(void) const {
    if (!type_gcValExternalVariable) {
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
        type_gcValExternalVariable = cType::InitializeType(
            0, 0, 0x166, type_variable, gcValExternalVariable::New, 0, 0, 0);
    }
    return type_gcValExternalVariable;
}
