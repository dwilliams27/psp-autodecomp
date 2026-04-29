// gcStateFunction — named state function with embedded parameter sub-object

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *mFile;
    int _pad;
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

template <class T>
T dcast(const cBase *);

struct StateFunctionNamedBlob {
    int w0, w1, w2, w3, w4, w5;
};

static inline void CopyStateFunctionNamedBlob(const StateFunctionNamedBlob *src,
                                              StateFunctionNamedBlob *dst) {
    *dst = *src;
}

class cNamed {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

struct ParamWriteVtableEntry {
    short adj;
    short pad;
    void (*fn)(void *self, cFile *file);
};

class gcStateFunction : public cNamed {
public:
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A3CC;

extern "C" {
    void gcStateFunction__gcStateFunction_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcStateFunction::Write(cFile &) const @ 0x0010973c ──
void gcStateFunction::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cNamed::Write(file);
    wb.Write(*(unsigned int *)((char *)this + 0x3c));
    char *vtable = *(char **)((char *)this + 0x24);
    ParamWriteVtableEntry *entry = (ParamWriteVtableEntry *)(vtable + 0x28);
    char *subThis = (char *)this + 0x20;
    void *adjThis = subThis + entry->adj;
    entry->fn(adjThis, wb.mFile);
    wb.End();
}

// ── gcStateFunction::New(cMemPool *, cBase *) static @ 0x00256830 ──
cBase *gcStateFunction::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStateFunction *result = 0;
    gcStateFunction *obj = (gcStateFunction *)rec->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        gcStateFunction__gcStateFunction_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcStateFunction::AssignCopy(const cBase *) @ 0x002567a8 ──
void gcStateFunction::AssignCopy(const cBase *src) {
    gcStateFunction *other = dcast<gcStateFunction *>(src);
    CopyStateFunctionNamedBlob((const StateFunctionNamedBlob *)((char *)other + 8),
                               (StateFunctionNamedBlob *)((char *)this + 8));
    ((gcEvent *)((char *)this + 0x20))->operator=(
        *(gcEvent *)((char *)other + 0x20));
    int *srcIndex = (int *)((char *)other + 0x3C);
    int *dstIndex = (int *)((char *)this + 0x3C);
    *dstIndex = *srcIndex;
}

// ── gcStateFunction::GetType(void) const @ 0x002568ac ──
const cType *gcStateFunction::GetType(void) const {
    if (D_0009A3CC == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_0009A3CC = cType::InitializeType(0, 0, 0xC8, D_000385E0,
                                           &gcStateFunction::New, 0, 0, 0);
    }
    return D_0009A3CC;
}
