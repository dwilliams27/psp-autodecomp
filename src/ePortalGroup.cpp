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

class ePortalGroup : public cGroup {
public:
    ePortalGroup(cBase *);
    ~ePortalGroup();
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

extern char ePortalGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

// ── ePortalGroup::Write(cFile &) const @ 0x0001A9FC ──
void ePortalGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── ePortalGroup::New(cMemPool *, cBase *) static @ 0x001DE81C ──
cBase *ePortalGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    ePortalGroup *result = 0;
    ePortalGroup *obj = (ePortalGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = ePortalGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── ePortalGroup::~ePortalGroup(void) @ 0x001DE9D0 ──
ePortalGroup::~ePortalGroup() {
    ((void **)this)[1] = ePortalGroupvirtualtable;
}
