// gcEvent::operator=(const gcEvent &) — 0x000d61d0

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

struct cGUID {
    int a;
    int b;
    static cGUID Generate(void);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(const cGUID &);
    void End(void);
};

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

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &other);
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcEvent {
public:
    char _pad0[8];
    gcExpressionList mExprList;     // +0x08
    char _pad1[4];                  // pad so mField10 lands at +0x10
    int mField10;                   // +0x10
    mutable cGUID mGUID;            // +0x14

    gcEvent(cBase *, const char *);
    ~gcEvent();
    gcEvent &operator=(const gcEvent &other);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_000998FC;
extern char gcEventvirtualtable[];
extern "C" void gcExpressionList_dtor(void *, int) __asm__("__0oQgcExpressionListdtv");

gcEvent &gcEvent::operator=(const gcEvent &other) {
    mExprList = other.mExprList;
    mField10 = other.mField10;
    return *this;
}

gcEvent::~gcEvent() {
    *(void **)((char *)this + 4) = gcEventvirtualtable;
    gcExpressionList_dtor((char *)this + 8, 2);
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

void gcEvent::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    bool valid = (mGUID.a != 0) || (mGUID.b != 0);
    cGUID *guid = &mGUID;

    if (!valid) {
        mGUID = cGUID::Generate();
    }
    mExprList.Write(wb);
    wb.Write(*guid);
    wb.End();
}

cBase *gcEvent::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEvent *result = 0;
    gcEvent *obj = (gcEvent *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEvent(parent, 0);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcEvent::GetType(void) const {
    if (D_000998FC == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_000998FC = cType::InitializeType(0, 0, 0x69, D_000385DC,
                                           &gcEvent::New, 0, 0, 0);
    }
    return D_000998FC;
}

void gcEvent::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    this->mExprList.VisitReferences(flags, (cBase *)this, cb, user, mask);
}
