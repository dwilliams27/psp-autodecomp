// eFontGroup — modeled after eMaterialGroup.cpp (same cGroup-derived
// engine resource pattern, identical Write/New/dtor codegen shape).

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

class eFontGroup : public cGroup {
public:
    eFontGroup(cBase *);
    ~eFontGroup();
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

extern char eFontGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

// ── eFontGroup::Write(cFile &) const @ 0x00017628 ──
void eFontGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eFontGroup::New(cMemPool *, cBase *) static @ 0x001DD274 ──
cBase *eFontGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eFontGroup *result = 0;
    eFontGroup *obj = (eFontGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eFontGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eFontGroup::~eFontGroup(void) @ 0x001DD428 ──
// Canonical destructor body. SNC's ABI emits the deleting variant (D0)
// that includes the (this != 0) guard, the chain call to ~cGroup, and the
// flag-gated dispatch through operator delete.
eFontGroup::~eFontGroup() {
    ((void **)this)[1] = eFontGroupvirtualtable;
}
