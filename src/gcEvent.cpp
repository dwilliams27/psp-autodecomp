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

struct VTableSlot {
    short offset;
    short pad;
    void *(*fn)(void *);
};

class gcExpressionList {
public:
    gcExpressionList(cBase *);
    gcExpressionList &operator=(const gcExpressionList &other);
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcEvent {
public:
    char _pad0[0x10];
    int mField10;                   // +0x10
    mutable cGUID mGUID;            // +0x14

    gcEvent(cBase *, const char *);
    ~gcEvent();
    gcEvent &operator=(const gcEvent &other);
    void AssignCopy(const cBase *);
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
extern "C" void gcExpressionList_ctor(void *, cBase *) __asm__("__0oQgcExpressionListctP6FcBase");
extern "C" void gcExpressionList_dtor(void *, int) __asm__("__0oQgcExpressionListdtv");

gcEvent::gcEvent(cBase *parent, const char *name) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = gcEventvirtualtable;
    gcExpressionList_ctor((char *)this + 8, (cBase *)this);
    *(const char **)((char *)this + 0x10) = name;
    *(int *)((char *)this + 0x14) = 0;
    *(int *)((char *)this + 0x18) = 0;
}

gcEvent &gcEvent::operator=(const gcEvent &other) {
    *(gcExpressionList *)((char *)this + 8) =
        *(const gcExpressionList *)((const char *)&other + 8);
    *(int *)((char *)this + 0x10) =
        *(const int *)((const char *)&other + 0x10);
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
    ((gcExpressionList *)((char *)this + 8))->Write(wb);
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

void gcEvent::AssignCopy(const cBase *base) {
    const cBase *copy = 0;

    if (base != 0) {
        if (D_000998FC == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000998FC = cType::InitializeType(0, 0, 0x69, D_000385DC,
                                               &gcEvent::New, 0, 0, 0);
        }

        void *vt = ((void **)base)[1];
        void *myType = D_000998FC;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short off = slot->offset;
        void *(*getType)(void *) = slot->fn;
        void *type = getType((char *)base + off);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done_type;
        }
        if (type != 0) {
        loop_type:
            if (type == myType) {
                ok = 1;
                goto done_type;
            }
            type = *(void **)((char *)type + 0x1C);
            if (type != 0) {
                goto loop_type;
            }
        }
        ok = 0;
    done_type:
        if (ok != 0) {
            copy = base;
        }
    }
    *this = *(const gcEvent *)copy;
}

void gcEvent::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcExpressionList *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}
