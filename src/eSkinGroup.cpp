// IsManagedTypeExternal / VisitReferences / AssignCopy trampolines + Write/New/dtor.

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

class eSkinGroup : public cGroup {
public:
    eSkinGroup(cBase *);
    ~eSkinGroup();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
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

class eSurfacePropertyTableGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class cObject {
public:
    void VisitReferences(unsigned int, cBase *,
        void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class eDynamicFluidTemplate : public cObject {
public:
    void VisitReferences(unsigned int, cBase *,
        void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcVariableGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcStringTableGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char eSkinGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E4C;

void eSkinGroup::AssignCopy(const cBase *base) {
    eSkinGroup *src = dcast<eSkinGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSurfacePropertyTableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void eDynamicFluidTemplate::VisitReferences(unsigned int a, cBase *b,
    void (*c)(cBase *, unsigned int, void *), void *d, unsigned int e) {
    cObject::VisitReferences(a, b, c, d, e);
}

bool gcVariableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcStringTableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── eSkinGroup::Write(cFile &) const @ 0x000181B0 ──
void eSkinGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSkinGroup::New(cMemPool *, cBase *) static @ 0x001DD744 ──
cBase *eSkinGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSkinGroup *result = 0;
    eSkinGroup *obj = (eSkinGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSkinGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSkinGroup::GetType(void) const @ 0x001DD800 ──
const cType *eSkinGroup::GetType(void) const {
    if (D_00040E4C == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E4C = cType::InitializeType(0, 0, 0x46, D_00040C94,
                                           &eSkinGroup::New,
                                           0, 0, 8);
    }
    return D_00040E4C;
}

// ── eSkinGroup::~eSkinGroup(void) @ 0x001DD8F8 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail dispatch through
// operator delete (which itself does pool/block lookup and calls the slot fn).
eSkinGroup::~eSkinGroup() {
    ((void **)this)[1] = eSkinGroupvirtualtable;
}
