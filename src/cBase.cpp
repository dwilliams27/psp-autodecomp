class cMemPool;
class cFile;
class cBase;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct cBaseDispatchRecord {
    short offset;
    short pad;
    void *fn;
};

typedef void *(*cBaseGetTypeFn)(void *);
typedef cBase *(*cBaseCreateFn)(cMemPool *, cBase *);
typedef void (*cBaseCopyFn)(void *, const cBase *);

class cBase {
public:
    ~cBase(void);
    const cType *GetType(void) const;
    cBase *Copy(cMemPool *, cBase *) const;
    void SetDirty(void);
    int IsEditable(void) const;
    void Reset(cMemPool *, bool);
    void GetName(char *) const;
    int GetProperties(void) const;
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            cBaseDispatchRecord *rec =
                (cBaseDispatchRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            ((void (*)(void *, void *))rec->fn)(block + off, p);
        } else {
            free(p);
        }
    }
};

class gcExpression {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    void SetChild(int, gcExpression *);
    gcExpression *GetChild(int) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    unsigned int GetTextColor(void) const;
    gcExpression *GetNext(void) const;
    void SetNext(gcExpression *);
    int GetExprFlags(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    const char *GetTextSeparator(int) const;
    void GetTextRecursive(char *) const;
};

extern cType *D_000385DC;

cBase::~cBase(void) {
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

const cType *cBase::GetType(void) const {
    if (D_000385DC == 0) {
        D_000385DC = cType::InitializeType((const char *)0x36C830,
                                           (const char *)0x36C838,
                                           1, 0, 0, 0, 0, 0);
    }
    return D_000385DC;
}

int cBase::GetProperties(void) const {
    return 0;
}

void cBase::OnMemPoolReset(const cMemPool *, unsigned int) {
}

cBase *cBase::Copy(cMemPool *pool, cBase *parent) const {
    cBaseDispatchRecord *typeRec =
        (cBaseDispatchRecord *)(*(char **)((char *)this + 4) + 8);
    cType *type = (cType *)((cBaseGetTypeFn)typeRec->fn)(
        (char *)this + typeRec->offset);
    cBase *copy = ((cBaseCreateFn)((void **)type)[4])(pool, parent);
    if (copy != 0) {
        cBaseDispatchRecord *copyRec =
            (cBaseDispatchRecord *)(*(char **)((char *)copy + 4) + 0x68);
        ((cBaseCopyFn)copyRec->fn)((char *)copy + copyRec->offset, this);
    }
    return copy;
}

void cBase::Write(cFile &) const {
}

int cBase::Read(cFile &, cMemPool *) {
    return 1;
}

int gcExpression::GetMaxChildren(void) const {
    return 0;
}
