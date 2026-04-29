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

class ePointGroup : public cGroup {
public:
    ePointGroup(cBase *);
    ~ePointGroup();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
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

class eSkyGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class ePortalGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class eRoomEnvironmentGroup {
public:
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char ePointGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E48;
extern cType *D_00040E68;
extern cType *D_00040E70;

// ── ePointGroup::Write(cFile &) const @ 0x00017BEC ──
void ePointGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── ePointGroup::New(cMemPool *, cBase *) static @ 0x001DD4DC ──
cBase *ePointGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    ePointGroup *result = 0;
    ePointGroup *obj = (ePointGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = ePointGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── ePointGroup::GetType(void) const @ 0x001DD598 ──
const cType *ePointGroup::GetType(void) const {
    if (D_00040E48 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E48 = cType::InitializeType(0, 0, 0x44, D_00040C94,
                                           &ePointGroup::New,
                                           0, 0, 8);
    }
    return D_00040E48;
}

// ── ePointGroup::~ePointGroup(void) @ 0x001DD690 ──
ePointGroup::~ePointGroup() {
    ((void **)this)[1] = ePointGroupvirtualtable;
}

void ePointGroup::AssignCopy(const cBase *base) {
    ePointGroup *src = dcast<ePointGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eSkyGroup::AssignCopy(const cBase *base) {
    eSkyGroup *src = dcast<eSkyGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void ePortalGroup::AssignCopy(const cBase *base) {
    ePortalGroup *src = dcast<ePortalGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ── ePortalGroup::GetType(void) const @ 0x001DE8D8 ──
const cType *ePortalGroup::GetType(void) const {
    if (D_00040E68 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E68 = cType::InitializeType(0, 0, 0x21F, D_00040C94,
                                           &ePortalGroup::New,
                                           0, 0, 8);
    }
    return D_00040E68;
}

// ── eRoomEnvironmentGroup::GetType(void) const @ 0x001DEDA8 ──
const cType *eRoomEnvironmentGroup::GetType(void) const {
    if (D_00040E70 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E70 = cType::InitializeType(0, 0, 0x236, D_00040C94,
                                           &eRoomEnvironmentGroup::New,
                                           0, 0, 8);
    }
    return D_00040E70;
}

class gcDoConsoleOp {
public:
    void EvaluateCommon() const;
    float Evaluate() const;
};

float gcDoConsoleOp::Evaluate() const {
    EvaluateCommon();
    return 0.0f;
}

struct _reent;
extern "C" int _close_r(struct _reent *, int);

struct __sFILE_lite {
    char _pad0[14];
    short _file;
    char _pad1[68];
    struct _reent *_data;
};

extern "C" int __sclose(struct __sFILE_lite *fp) {
    return _close_r(fp->_data, fp->_file);
}
