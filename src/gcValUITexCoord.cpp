// gcValUITexCoord — gcAll_psp.obj
// Decompiled functions:
//   0x0036655c  gcValUITexCoord::AssignCopy(const cBase *)            (112B)
//   0x003665cc  gcValUITexCoord::New(cMemPool *, cBase *) static      (180B)
//   0x00367070  gcValUITexCoord::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int) (128B)
//   0x003670f0  gcValUITexCoord::~gcValUITexCoord(void)               (100B)
//
// Class layout (36 bytes, alloc size 0x24):
//   [0x00] mParent (cBase *)             ─┐ gcLValue base
//   [0x04] mVtable                        ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mField14 (int)
//   [0x18] mField18 (int)
//   [0x1C] mField1C (bool)
//   [0x20] mField20 (float)

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];   // @ 0x37E6A8
extern char gcLValuevirtualtable[];
extern char gcValUITexCoordvirtualtable[];

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

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

void gcDesiredUIWidgetHelper_ctor(void *, int);

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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

class gcValUITexCoord {
public:
    cBase *m_parent;     // 0x00
    void  *m_vtable;     // 0x04
    int    m_helper[3];  // 0x08, 0x0C, 0x10
    int    mField14;     // 0x14
    int    mField18;     // 0x18
    bool   mField1C;     // 0x1C
    float  mField20;     // 0x20

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    ~gcValUITexCoord();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_variable asm("D_0009F3EC");
static cType *type_gcValUITexCoord asm("D_0009F90C");

// ============================================================
// 0x0036655c — AssignCopy(const cBase *)
// ============================================================
struct cHandle {
    int mId;
};

struct gcDesiredUIWidgetHelperData {
    int mField0;
    cHandle mField4;
    unsigned int mField8;
};

void gcValUITexCoord::AssignCopy(const cBase *base) {
    gcValUITexCoord *other = dcast<gcValUITexCoord>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
    mField14 = other->mField14;
    mField18 = other->mField18;
    mField1C = other->mField1C;
    mField20 = other->mField20;
}

// ============================================================
// 0x003665cc — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcValUITexCoord::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValUITexCoord *result = 0;
    gcValUITexCoord *obj = (gcValUITexCoord *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUITexCoordvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        obj->mField14 = 0;
        obj->mField18 = 0;
        obj->mField1C = false;
        obj->mField20 = 0.85f;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00366680 — GetType(void) const
// ============================================================
const cType *gcValUITexCoord::GetType(void) const {
    if (!type_gcValUITexCoord) {
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
        type_gcValUITexCoord = cType::InitializeType(
            0, 0, 0x96, type_variable, gcValUITexCoord::New, 0, 0, 0);
    }
    return type_gcValUITexCoord;
}

// ============================================================
// 0x003667d4 — Write(cFile &) const
// ============================================================
void gcValUITexCoord::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcLValue *)this)->Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 8))->Write(wb);
    wb.Write(mField14);
    wb.Write(mField18);
    wb.Write(mField1C);
    wb.Write(mField20);
    wb.End();
}

// ============================================================
// 0x00367070 — VisitReferences
// ============================================================
void gcValUITexCoord::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ============================================================
// 0x003670f0 — ~gcValUITexCoord(void)  (deleting destructor)
// ============================================================
gcValUITexCoord::~gcValUITexCoord() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
