class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

class eRoomGroup : public cGroup {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    ~eRoomGroup();
    static bool IsManagedTypeExternalStatic();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class eRoomSetGroup : public cGroup {
public:
    void AssignCopy(const cBase *);
};

class gcStaticInstanceGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcFunctionGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcUIDialogGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char eRoomGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E64;

void eRoomGroup::AssignCopy(const cBase *base) {
    eRoomGroup *src = dcast<eRoomGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eRoomSetGroup::AssignCopy(const cBase *base) {
    eRoomSetGroup *src = dcast<eRoomSetGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcStaticInstanceGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcFunctionGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcUIDialogGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── eRoomGroup::Write(cFile &) const @ 0x0001A438 ──
void eRoomGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eRoomGroup::New(cMemPool *, cBase *) static @ 0x001DE5B4 ──
cBase *eRoomGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eRoomGroup *result = 0;
    eRoomGroup *obj = (eRoomGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eRoomGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eRoomGroup::GetType(void) const @ 0x001DE670 ──
const cType *eRoomGroup::GetType(void) const {
    if (D_00040E64 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E64 = cType::InitializeType(0, 0, 0x21D, D_00040C94,
                                           &eRoomGroup::New,
                                           0, 0, 8);
    }
    return D_00040E64;
}

// ── eRoomGroup::~eRoomGroup(void) @ 0x001DE768 ──
// SNC auto-generates the deleting destructor (D0) from this canonical body
// plus inheritance from cGroup and the inline `operator delete` declared
// above: vtable write, chain to ~cGroup(), delete-flag check, pool dispatch.
eRoomGroup::~eRoomGroup() {
    ((void **)this)[1] = eRoomGroupvirtualtable;
}
