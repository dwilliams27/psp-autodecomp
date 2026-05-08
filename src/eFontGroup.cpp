// eFontGroup — modeled after eMaterialGroup.cpp (same cGroup-derived
// engine resource pattern, identical Write/New/dtor codegen shape).

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock();
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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eFont {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eFontGroup : public cGroup {
public:
    eFontGroup(cBase *);
    ~eFontGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
    const char *GetFileExtension(void) const;
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_00040E44;
extern cType *D_00041114;

// ── eFontGroup::Write(cFile &) const @ 0x00017628 ──
void eFontGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eFontGroup::Read(cFile &, cMemPool *) @ 0x00017674 ──
int eFontGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1) goto fail;
    if (cGroup::Read(file, pool)) goto succ;
fail:
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;
succ:
    return result;
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

// ── eFontGroup::GetType(void) const @ 0x001DD330 ──
const cType *eFontGroup::GetType(void) const {
    if (D_00040E44 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E44 = cType::InitializeType(0, 0, 0x41, D_00040C94,
                                           &eFontGroup::New,
                                           0, 0, 8);
    }
    return D_00040E44;
}

// ── eFontGroup::GetManagedType(void) const @ 0x00017730 ──
const cType *eFontGroup::GetManagedType(void) const {
    if (D_00041114 == 0) {
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
        D_00041114 = cType::InitializeType(
            0, 0, 0x40, D_000385E4, &eFont::New,
            (const char *)0x36CDE8, (const char *)0x36CDF0, 5);
    }
    return D_00041114;
}

// ── eFontGroup::GetDataDirectory(void) const @ 0x00017858 ──
const char *eFontGroup::GetDataDirectory(void) const {
    if (D_00041114 == 0) {
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
        D_00041114 = cType::InitializeType(
            0, 0, 0x40, D_000385E4, &eFont::New,
            (const char *)0x36CDE8, (const char *)0x36CDF0, 5);
    }
    return (const char *)((int *)D_00041114)[5];
}

// ── eFontGroup::GetFileExtension(void) const @ 0x00017984 ──
const char *eFontGroup::GetFileExtension(void) const {
    if (D_00041114 == 0) {
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
        D_00041114 = cType::InitializeType(
            0, 0, 0x40, D_000385E4, &eFont::New,
            (const char *)0x36CDE8, (const char *)0x36CDF0, 5);
    }
    return (const char *)((int *)D_00041114)[6];
}

// ── eFontGroup::IsManagedTypeExternalStatic(void) static @ 0x00017ab0 ──
bool eFontGroup::IsManagedTypeExternalStatic() {
    if (D_00041114 == 0) {
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
        D_00041114 = cType::InitializeType(
            0, 0, 0x40, D_000385E4, &eFont::New,
            (const char *)0x36CDE8, (const char *)0x36CDF0, 5);
    }
    int flags = *(int *)D_00041114;
    bool result = false;
    if (flags & 1) result = true;
    return result;
}

// ── eFontGroup::~eFontGroup(void) @ 0x001DD428 ──
// Canonical destructor body. SNC's ABI emits the deleting variant (D0)
// that includes the (this != 0) guard, the chain call to ~cGroup, and the
// flag-gated dispatch through operator delete.
eFontGroup::~eFontGroup() {
    ((void **)this)[1] = eFontGroupvirtualtable;
}
