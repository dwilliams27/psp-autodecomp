// gcValMemCardStatus.cpp — decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00350008 gcValMemCardStatus::New(cMemPool *, cBase *) static    140B
//   0x003501ac gcValMemCardStatus::Write(cFile &) const             100B
//   0x00350490 gcValMemCardStatus::~gcValMemCardStatus(void)         100B  (deleting)

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct gcValMemCardStatus_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct gcValMemCardStatus_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcValMemCardStatus_AllocRecord {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcValMemCardStatus {
public:
    void *_field0;          // 0x0
    void *_field4;          // 0x4 — classdesc
    unsigned int mField8;   // 0x8
    unsigned int mFieldC;   // 0xC

    ~gcValMemCardStatus(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        void *self = p;
        gcValMemCardStatus_DeleteRecord *rec =
            (gcValMemCardStatus_DeleteRecord *)
            (((gcValMemCardStatus_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, self);
    }
};

extern char cBaseclassdesc[];   // @ 0x37E6A8
extern char gcValMemCardStatusvirtualtable[];
void cStrAppend(char *, const char *, ...);

// ── gcValMemCardStatus::New @ 0x00350008 ──
cBase *gcValMemCardStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((gcValMemCardStatus_PoolBlock *)block)->allocTable;
    gcValMemCardStatus_AllocRecord *rec =
        (gcValMemCardStatus_AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValMemCardStatus *result = 0;
    gcValMemCardStatus *obj =
        (gcValMemCardStatus *)rec->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValMemCardStatusvirtualtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValMemCardStatus::Write @ 0x003501ac ──
void gcValMemCardStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mFieldC);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValMemCardStatus;

const cType *gcValMemCardStatus::GetType(void) const {
    if (!type_gcValMemCardStatus) {
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
        type_gcValMemCardStatus = cType::InitializeType(
            0, 0, 0x1C1, type_value, gcValMemCardStatus::New, 0, 0, 0);
    }
    return type_gcValMemCardStatus;
}

void gcValMemCardStatus::GetText(char *buf) const {
    if (((const int *)this)[2] == 0) {
        cStrAppend(buf, (const char *)0x36F588, (const char *)0x36DAF0);
        return;
    }
    cStrAppend(buf, (const char *)0x36DAF0);
}

// ── gcValMemCardStatus::~gcValMemCardStatus @ 0x00350490, 100B ──
// Canonical destructor: SNC auto-emits the (this != 0) guard, the absence of a
// parent destructor chain, and the deleting-tail dispatch through operator
// delete on (flag & 1). Body just resets the classdesc pointer at offset 4.
gcValMemCardStatus::~gcValMemCardStatus(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
