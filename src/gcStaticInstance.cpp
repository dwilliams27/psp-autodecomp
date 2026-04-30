class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

class gcStaticInstance : public cObject {
public:
    char _pad8[0x44];
    cHandle mField44;
    cBase *mField48;
    float mField4C;
    bool mField50;

    gcStaticInstance(cBase *);
    ~gcStaticInstance();
    gcStaticInstance &operator=(const gcStaticInstance &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcStaticInstanceclassdesc[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F560;

gcStaticInstance::gcStaticInstance(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = gcStaticInstanceclassdesc;
    mField44.mId = 0;
    mField48 = 0;
    mField4C = 1.0f;
    mField50 = false;
}

gcStaticInstance::~gcStaticInstance() {
    *(void **)((char *)this + 4) = gcStaticInstanceclassdesc;
    if (mField48 != 0) {
        char *classdesc = *(char **)((char *)mField48 + 4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 0x50);
        entry->fn((char *)mField48 + entry->offset, 3);
        mField48 = 0;
    }
    return;
}

void gcStaticInstance::Write(cFile &file) const {
    cWriteBlock wb(file, 0xA);
    cObject::Write(file);
    mField44.Write(wb);
    wb.WriteBase(mField48);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.End();
}

cBase *gcStaticInstance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStaticInstance *result = 0;
    gcStaticInstance *obj = (gcStaticInstance *)rec->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcStaticInstance(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcStaticInstance::AssignCopy(const cBase *base) {
    const gcStaticInstance *other = 0;

    if (base != 0) {
        if (D_0009F560 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_0009F560 = cType::InitializeType(0, 0, 0x67, D_000385E4,
                                               &gcStaticInstance::New,
                                               (const char *)0x36DA34,
                                               (const char *)0x36DA48, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F560;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcStaticInstance *)base;
        }
    }
    operator=(*other);
}

const cType *gcStaticInstance::GetType(void) const {
    if (D_0009F560 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F560 = cType::InitializeType(0, 0, 0x67, D_000385E4,
                                           &gcStaticInstance::New,
                                           (const char *)0x36DA34,
                                           (const char *)0x36DA48, 0);
    }
    return D_0009F560;
}
