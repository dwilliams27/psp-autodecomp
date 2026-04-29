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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    int Read(cFile &, cMemPool *);
};

class ePathGroup : public cGroup {
public:
    ePathGroup(cBase *);
    ~ePathGroup();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class eGeomTemplateGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSoundDataGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eAnimationGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eSurfaceSetGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char ePathGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E20;

void ePathGroup::AssignCopy(const cBase *base) {
    ePathGroup *src = dcast<ePathGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eGeomTemplateGroup::AssignCopy(const cBase *base) {
    eGeomTemplateGroup *src = dcast<eGeomTemplateGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSoundDataGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eAnimationGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eSurfaceSetGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── ePathGroup::Write(cFile &) const @ 0x00014294 ──
void ePathGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── ePathGroup::New(cMemPool *, cBase *) static @ 0x001DBCCC ──
cBase *ePathGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    ePathGroup *result = 0;
    ePathGroup *obj = (ePathGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = ePathGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── ePathGroup::GetType(void) const @ 0x001DBD88 ──
const cType *ePathGroup::GetType(void) const {
    if (D_00040E20 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E20 = cType::InitializeType(0, 0, 0x19, D_00040C94,
                                           &ePathGroup::New,
                                           0, 0, 8);
    }
    return D_00040E20;
}

// ── ePathGroup::~ePathGroup(void) @ 0x001DBE80 ──
ePathGroup::~ePathGroup() {
    ((void **)this)[1] = ePathGroupvirtualtable;
}
