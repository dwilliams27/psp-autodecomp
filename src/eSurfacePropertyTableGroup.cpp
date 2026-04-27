class cBase;
class cFile;
class cMemPool;

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

class eSurfacePropertyTableGroup : public cGroup {
public:
    eSurfacePropertyTableGroup(cBase *);
    ~eSurfacePropertyTableGroup();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
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

class eSkyGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eRoomGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class ePortalGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eRoomSetGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char eSurfacePropertyTableGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

void eSurfacePropertyTableGroup::AssignCopy(const cBase *base) {
    eSurfacePropertyTableGroup *src = dcast<eSurfacePropertyTableGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSkyGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eRoomGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool ePortalGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eRoomSetGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── eSurfacePropertyTableGroup::Write(cFile &) const @ 0x0001BB48 ──
void eSurfacePropertyTableGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSurfacePropertyTableGroup::New(cMemPool *, cBase *) static @ 0x001DEF54 ──
cBase *eSurfacePropertyTableGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSurfacePropertyTableGroup *result = 0;
    eSurfacePropertyTableGroup *obj = (eSurfacePropertyTableGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSurfacePropertyTableGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSurfacePropertyTableGroup::~eSurfacePropertyTableGroup(void) @ 0x001DF108 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail dispatch through
// operator delete (which itself does pool/block lookup and calls the slot fn).
eSurfacePropertyTableGroup::~eSurfacePropertyTableGroup() {
    ((void **)this)[1] = eSurfacePropertyTableGroupvirtualtable;
}
