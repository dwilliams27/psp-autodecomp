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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class ePortal {
public:
    static cBase *New(cMemPool *, cBase *);
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
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A0;

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

// ── ePortalGroup::GetManagedType(void) const @ 0x0001AB04 ──
const cType *ePortalGroup::GetManagedType(void) const {
    if (D_000469A0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_000469A0 = cType::InitializeType(
            0, 0, 0x21E, D_000385E4, &ePortal::New,
            (const char *)0x36CE20, (const char *)0x36CE28, 3);
    }
    return D_000469A0;
}

// ── ePortalGroup::GetDataDirectory(void) const @ 0x0001AC2C ──
const char *ePortalGroup::GetDataDirectory(void) const {
    if (D_000469A0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_000469A0 = cType::InitializeType(
            0, 0, 0x21E, D_000385E4, &ePortal::New,
            (const char *)0x36CE20, (const char *)0x36CE28, 3);
    }
    return (const char *)((int *)D_000469A0)[5];
}
