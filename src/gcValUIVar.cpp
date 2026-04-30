// gcValUIVar — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0036823c  AssignCopy(const cBase *)
//   0x00368484  Write(cFile &) const
//   0x003699c0  GetText(char *) const
//   0x00369a10  VisitReferences(...)
//   0x00369a90  ~gcValUIVar(void)
//
// Class layout:
//   [0x00] mParent (cBase *)              ─┐ gcLValue base
//   [0x04] mVtable                         ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mField14 (int)

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
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

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void cStrAppend(char *, const char *, ...);
void gcDesiredUIWidgetHelper_ctor(void *, int);
extern const char gcValUIVar_fmt[];
extern const char gcValUIVar_str2[];

extern char gcLValuevirtualtable[];
extern char gcValUIVarvirtualtable[];
extern char cBaseclassdesc[];

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValUIVar : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;
    int mField14;

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    ~gcValUIVar();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mId;
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValUIVar;

// ── gcValUIVar::AssignCopy @ 0x0036823c ──
void gcValUIVar::AssignCopy(const cBase *base) {
    gcValUIVar *other = dcast<gcValUIVar>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
    *(int *)((char *)this + 20) = *(const int *)((char *)other + 20);
}

// ── gcValUIVar::New @ 0x00368294 ──
cBase *gcValUIVar::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValUIVar *result = 0;
    gcValUIVar *obj = (gcValUIVar *)e->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUIVarvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        obj->mField14 = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUIVar::Write @ 0x00368484 ──
void gcValUIVar::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Write(wb);
    wb.Write(mField14);
    wb.End();
}

// ── gcValUIVar::GetText @ 0x003699c0 ──
void gcValUIVar::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->GetText(local);
    cStrAppend(buf, gcValUIVar_fmt, local, gcValUIVar_str2);
}

// ── gcValUIVar::VisitReferences @ 0x00369a10 ──
void gcValUIVar::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    this->mHelper.VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ── gcValUIVar::~gcValUIVar @ 0x00369a90 ──
gcValUIVar::~gcValUIVar() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

const cType *gcValUIVar::GetType(void) const {
    if (!type_gcValUIVar) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(0, 0, 0x6A,
                                                            type_base, 0, 0,
                                                            0, 0);
                }
                type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                                   0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(0, 0, 0x6D, type_value,
                                                  0, 0, 0, 0);
        }
        type_gcValUIVar = cType::InitializeType(0, 0, 0x98, type_variable,
                                                gcValUIVar::New, 0, 0, 0);
    }
    return type_gcValUIVar;
}
