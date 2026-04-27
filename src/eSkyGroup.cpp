// eSkyGroup.cpp — decompiled from eAll_psp.obj
// Parent: cGroup. Vtable at 0x37F550 (eSkyGroupvirtualtable).
// Same shape as eMaterialGroup (cGroup-derived, not-yet-overridden Write).

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End();
};

class cGroup {
public:
    cBase *m_parent;        // 0x00
    void *m_vtbl;           // 0x04
    unsigned char mFlag;    // 0x08
    char _pad[3];
    int mField;             // 0x0C
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
};

class eSkyGroup : public cGroup {
public:
    eSkyGroup(cBase *);
    ~eSkyGroup();
    void Write(cFile &) const;
    static bool IsManagedTypeExternalStatic();
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

extern char eSkyGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

// ── eSkyGroup::Write(cFile &) const @ 0x00019e74 ──
void eSkyGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSkyGroup::New(cMemPool *, cBase *) static @ 0x001de34c ──
cBase *eSkyGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSkyGroup *result = 0;
    eSkyGroup *obj = (eSkyGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSkyGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSkyGroup::~eSkyGroup(void) @ 0x001de500 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the chain-call to ~cGroup() with flags=0, and the deleting-tail dispatch
// `if (flags & 1) operator delete(this)` from the inline body above.
eSkyGroup::~eSkyGroup() {
    *(void **)((char *)this + 4) = eSkyGroupvirtualtable;
}
