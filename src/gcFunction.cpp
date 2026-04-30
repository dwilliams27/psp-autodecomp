// gcFunction.cpp — decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _id;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class gcEvent {
public:
    char _pad[0x1C];
    gcEvent &operator=(const gcEvent &);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcFunction : public cObject {
public:
    gcEvent mEvent;

    gcFunction(cBase *);
    ~gcFunction();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern "C" void gcFunction__gcFunction_cBaseptr(void *self, cBase *parent);
extern "C" void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("__0oHgcEventctP6FcBasePCc");
extern "C" void gcEvent___dtor_gcEvent_void(void *, int);
extern char gcFunctionvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F4C8;

// ── gcFunction::gcFunction(cBase *) @ 0x0012F900 ──
gcFunction::gcFunction(cBase *parent)
    : cObject(parent) {
    *(void **)((char *)this + 4) = gcFunctionvirtualtable;
    gcEvent_ctor((char *)this + 0x44, (cBase *)this, 0);
}

// ── gcFunction::AssignCopy(const cBase *) @ 0x0027D108 ──
void gcFunction::AssignCopy(const cBase *base) {
    gcFunction *other = dcast<gcFunction>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    mEvent = other->mEvent;
}

// ── gcFunction::Write(cFile &) const @ 0x0012F7A8 ──
struct gcEventVEntry {
    short adj;
    short pad;
    void (*fn)(const void *, cFile *);
};

void gcFunction::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cObject *)this)->Write(file);
    const gcEventVEntry *vt = *(const gcEventVEntry **)((const char *)this + 0x48);
    const gcEventVEntry *e = vt + 5;
    short adj = e->adj;
    const char *base44 = (const char *)this + 0x44;
    void (*fn)(const void *, cFile *) = e->fn;
    fn(base44 + adj, wb.file);
    wb.End();
}

// ── gcFunction::New(cMemPool *, cBase *) static @ 0x0027D154 ──
cBase *gcFunction::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcFunction *result = 0;
    gcFunction *obj = (gcFunction *)entry->fn(base, 0x60, 4, 0, 0);
    if (obj != 0) {
        gcFunction__gcFunction_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcFunction::GetType(void) const @ 0x0027D1D0 ──
const cType *gcFunction::GetType(void) const {
    if (D_0009F4C8 == 0) {
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
        D_0009F4C8 = cType::InitializeType(0, 0, 0x74, D_000385E4,
                                           &gcFunction::New,
                                           (const char *)0x36DA14,
                                           (const char *)0x36DA20, 5);
    }
    return D_0009F4C8;
}

// ── gcFunction::~gcFunction(void) @ 0x0012F944 ──
gcFunction::~gcFunction() {
    *(void **)((char *)this + 4) = gcFunctionvirtualtable;
    gcEvent___dtor_gcEvent_void((char *)this + 0x44, 2);
}
