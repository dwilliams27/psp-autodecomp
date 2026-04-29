// Days of Thunder decompilation: gcEventString methods

class cBase {
public:
    int _b0;
    int _b4;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class cFile;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _d1;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

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

class gcStringValue : public cBase {
public:
    gcStringValue(cBase *parent);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcEventString : public gcStringValue {
public:
    gcEventString(cBase *parent);
    ~gcEventString();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Get(wchar_t *, int) const;
    void GetName(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    const cType *GetType() const;
    int Read(cFile &, cMemPool *);
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

extern char cBaseclassdesc[];
extern char gcEventStringvirtualtable[];

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4BC;

inline gcStringValue::gcStringValue(cBase *parent) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
    *(cBase **)((char *)this + 0) = parent;
}

inline gcEventString::gcEventString(cBase *parent) : gcStringValue(parent) {
    *(void **)((char *)this + 4) = gcEventStringvirtualtable;
}

inline void *operator new(unsigned, void *p) { return p; }

gcEventString *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void cStrCopy(wchar_t *, const wchar_t *, int);

extern const char gcEventString_Name[];

struct gcLangCtxEntry {
    char _pad[20];
    const wchar_t *mText;  // 0x14
};

struct gcLangCtx {
    char _pad[12];
    gcLangCtxEntry *mEntry;  // 0xC
};

extern gcLangCtx *gLangCtx;

void gcEventString::AssignCopy(const cBase *other) {
    dcast(other);
}

void gcEventString::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcStringValue::Write(file);
    wb.End();
}

void gcEventString::Get(wchar_t *wbuf, int size) const {
    gcLangCtxEntry *q = 0;
    if (gLangCtx != 0) {
        q = gLangCtx->mEntry;
    }
    cStrCopy(wbuf, q->mText, size);
}

void gcEventString::GetName(char *buf) const {
    cStrAppend(buf, gcEventString_Name);
}

void gcEventString::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)(void *)this, arg3);
    }
}

// ─────────────────────────────────────────────────────────────────────────
// gcEventString::~gcEventString(void)  @ 0x0027bf28, 100B
// ─────────────────────────────────────────────────────────────────────────
gcEventString::~gcEventString() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ─────────────────────────────────────────────────────────────────────────
// gcEventString::New(cMemPool *, cBase *) static  @ 0x0027bc20, 132B
// ─────────────────────────────────────────────────────────────────────────
cBase *gcEventString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcEventString *result = 0;
    gcEventString *obj = (gcEventString *)e->fn(base, 8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEventString(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcEventString::GetType(void) const  @ 0x0027bca4, 220B
// ─────────────────────────────────────────────────────────────────────────
const cType *gcEventString::GetType() const {
    if (D_0009F4BC == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4BC = cType::InitializeType(0, 0, 0x24B, D_0009F454,
                                           &gcEventString::New, 0, 0, 0);
    }
    return D_0009F4BC;
}

// ─────────────────────────────────────────────────────────────────────────
// gcEventString::Read(cFile &, cMemPool *)  @ 0x0027bdcc, 188B
// ─────────────────────────────────────────────────────────────────────────
int gcEventString::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcStringValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
