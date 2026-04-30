class cBase;
class cFile;
class cMemPool;

extern "C" void free(void *);
extern "C" void *__vec_new(void *, int, int, void (*)(void *));

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;

    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcSubGeomController {
public:
    cBase *mParent;
    void *mVtable;
    int mField8;

    gcSubGeomController(cBase *);
    ~gcSubGeomController(void);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

class gcValEntityTimeSince {
public:
    void Write(cFile &) const;
};

extern char gcSubGeomControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F64C;

void gcLValue_Write(const gcValEntityTimeSince *, cFile &);

gcSubGeomController::gcSubGeomController(cBase *parent) {
    mParent = parent;
    mVtable = gcSubGeomControllervirtualtable;
    mField8 = 0;
    float zero = 0.0f;
    *(int *)((char *)this + 0x0C) = 0;
    float one = 1.0f;
    *(float *)((char *)this + 0x18) = zero;
    *(float *)((char *)this + 0x10) = one;
    *(float *)((char *)this + 0x14) = one;
    __vec_new((char *)this + 0x1C, 1, 6, (void (*)(void *))0x24400C);
}

gcSubGeomController::~gcSubGeomController(void) {
    mField8 = 0;
    mVtable = gcSubGeomControllervirtualtable;
}

const cType *gcSubGeomController::GetType(void) const {
    if (D_0009F64C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC, 0, 0, 0, 0);
    }
    return D_0009F64C;
}

void gcValEntityTimeSince::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue_Write(this, file);

    const cTypeMethod *lhs =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *lhsBase = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)lhs->fn)(lhsBase + lhs->offset, wb.file);

    const cTypeMethod *rhs =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 40);
    char *rhsBase = (char *)this + 0x34;
    ((WriteFn)rhs->fn)(rhsBase + rhs->offset, wb.file);

    wb.End();
}
