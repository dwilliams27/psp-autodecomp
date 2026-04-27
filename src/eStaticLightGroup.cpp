// eStaticLightGroup.cpp — decompiled from eAll_psp.obj
// Vtable hierarchy mirrors eMaterialGroup: cBase=0x37E6A8, cGroup=0x37EA80,
// eStaticLightGroup=0x37F310.

class cBase;
class cFile;
class cMemPool;
class cType;

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

class cFile {
public:
    void SetCurrentPos(unsigned int);
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

class eStaticLightGroup : public cGroup {
public:
    eStaticLightGroup(cBase *);
    ~eStaticLightGroup();
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

extern char eStaticLightGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

// ── eStaticLightGroup::Write(cFile &) const @ 0x00018774 ──
void eStaticLightGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eStaticLightGroup::~eStaticLightGroup() @ 0x001DDB60 ──
// Canonical C++ destructor; SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup() with flags=0, and the deleting-tail dispatch
// through operator delete.
eStaticLightGroup::~eStaticLightGroup() {
    *(void **)((char *)this + 4) = eStaticLightGroupvirtualtable;
}

// ── eStaticLightGroup::New(cMemPool *, cBase *) static @ 0x001DD9AC ──
cBase *eStaticLightGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eStaticLightGroup *result = 0;
    eStaticLightGroup *obj = (eStaticLightGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eStaticLightGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}
