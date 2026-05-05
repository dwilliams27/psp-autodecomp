// eStaticLightGroup.cpp — decompiled from eAll_psp.obj
// Vtable hierarchy mirrors eMaterialGroup: cBase=0x37E6A8, cGroup=0x37EA80,
// eStaticLightGroup=0x37F310.

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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eStaticLightGroup : public cGroup {
public:
    eStaticLightGroup(cBase *);
    ~eStaticLightGroup();
    const cType *GetType(void) const;
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
    const char *GetFileExtension(void) const;
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_00040E50;
extern cType *D_00046B30;

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

// ── eStaticLightGroup::GetManagedType(void) const @ 0x0001887c ──
const cType *eStaticLightGroup::GetManagedType(void) const {
    if (D_00046B30 == 0) {
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
        D_00046B30 = cType::InitializeType(
            0, 0, 0x4A, D_000385E4, 0,
            (const char *)0x36CEE0, (const char *)0x36CEEC, 0);
    }
    return D_00046B30;
}

// ── eStaticLightGroup::GetDataDirectory(void) const @ 0x000189a0 ──
const char *eStaticLightGroup::GetDataDirectory(void) const {
    if (D_00046B30 == 0) {
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
        D_00046B30 = cType::InitializeType(
            0, 0, 0x4A, D_000385E4, 0,
            (const char *)0x36CEE0, (const char *)0x36CEEC, 0);
    }
    return (const char *)((int *)D_00046B30)[5];
}

// ── eStaticLightGroup::GetFileExtension(void) const @ 0x00018ac8 ──
const char *eStaticLightGroup::GetFileExtension(void) const {
    if (D_00046B30 == 0) {
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
        D_00046B30 = cType::InitializeType(
            0, 0, 0x4A, D_000385E4, 0,
            (const char *)0x36CEE0, (const char *)0x36CEEC, 0);
    }
    return (const char *)((int *)D_00046B30)[6];
}

// ── eStaticLightGroup::IsManagedTypeExternalStatic(void) static @ 0x00018bf0 ──
bool eStaticLightGroup::IsManagedTypeExternalStatic() {
    if (D_00046B30 == 0) {
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
        D_00046B30 = cType::InitializeType(
            0, 0, 0x4A, D_000385E4, 0,
            (const char *)0x36CEE0, (const char *)0x36CEEC, 0);
    }
    int flags = *(int *)D_00046B30;
    bool result = false;
    if (flags & 1) result = true;
    return result;
}

// ── eStaticLightGroup::GetType(void) const @ 0x001DDA68 ──
const cType *eStaticLightGroup::GetType(void) const {
    if (D_00040E50 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E50 = cType::InitializeType(0, 0, 0x50, D_00040C94,
                                           &eStaticLightGroup::New,
                                           0, 0, 8);
    }
    return D_00040E50;
}
