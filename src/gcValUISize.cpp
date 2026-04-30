// gcValUISize — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x00365264  AssignCopy(const cBase *)                    (104B)
//   0x003652cc  New(cMemPool *, cBase *) static              (176B)
//   0x003654d0  Write(cFile &) const                       (124B)
//   0x00366478  VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int) (128B)
//   0x003664f8  ~gcValUISize(void)                         (100B)
//
// Class layout (32 bytes, alloc size 0x20):
//   [0x00] mParent (cBase *)            ─┐ gcLValue base
//   [0x04] mVtable                       ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mField14 (int)
//   [0x18] mField18 (bool)
//   [0x1C] mField1C (float)

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void Write(float);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

void gcDesiredUIWidgetHelper_ctor(void *, int);

extern char gcLValuevirtualtable[];
extern char gcValUISizevirtualtable[];
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
    gcLValue(cBase *parent);
    void Write(cFile &) const;
};

class gcValUISize : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;
    int mField14;
    bool mField18;
    float mField1C;

    ~gcValUISize();
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

inline gcLValue::gcLValue(cBase *parent) {
    mVtable = gcLValuevirtualtable;
    mParent = parent;
}

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_variable asm("D_0009F3EC");
static cType *type_gcValUISize asm("D_0009F908");

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mId;
};

// ── gcValUISize::GetType(void) const @ 0x0036537c ──
const cType *gcValUISize::GetType(void) const {
    if (!type_gcValUISize) {
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
        type_gcValUISize = cType::InitializeType(
            0, 0, 0x92, type_variable, gcValUISize::New, 0, 0, 0);
    }
    return type_gcValUISize;
}

// ── gcValUISize::AssignCopy(const cBase *) @ 0x00365264 ──
void gcValUISize::AssignCopy(const cBase *base) {
    gcValUISize *other = dcast<gcValUISize>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
    mField14 = other->mField14;
    mField18 = other->mField18;
    mField1C = other->mField1C;
}

// ── gcValUISize::New(cMemPool *, cBase *) static @ 0x003652cc ──
cBase *gcValUISize::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValUISize *result = 0;
    gcValUISize *obj = (gcValUISize *)e->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUISizevirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        obj->mField14 = 0;
        obj->mField18 = false;
        obj->mField1C = 0.85f;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUISize::Write(cFile &) const @ 0x003654d0 ──
void gcValUISize::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Write(wb);
    wb.Write(mField14);
    wb.Write(mField18);
    wb.Write(mField1C);
    wb.End();
}

// ── gcValUISize::VisitReferences @ 0x00366478 ──
void gcValUISize::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ── gcValUISize::~gcValUISize(void) @ 0x003664f8 ──
gcValUISize::~gcValUISize() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
