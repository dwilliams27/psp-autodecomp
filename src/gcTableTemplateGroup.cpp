// gcTableTemplateGroup — inherits cGroup, holds a bool flag and an int.
// Decompiled functions:
//   0x000d31e4 gcTableTemplateGroup::Write(cFile &) const
//   0x000d3230 gcTableTemplateGroup::Read(cFile &, cMemPool *)
//   0x000d32ec gcTableTemplateGroup::GetManagedType(void) const
//   0x000d3414 gcTableTemplateGroup::GetDataDirectory(void) const
//   0x000d3540 gcTableTemplateGroup::GetFileExtension(void) const
//   0x000d366c gcTableTemplateGroup::IsManagedTypeExternalStatic(void) static
//   0x002385cc gcTableTemplateGroup::AssignCopy(const cBase *)
//   0x00238604 gcTableTemplateGroup::New(cMemPool *, cBase *) static
//   0x002386c0 gcTableTemplateGroup::GetType(void) const
//   0x002387b8 gcTableTemplateGroup::~gcTableTemplateGroup(void)

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
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
class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cGroup {
public:
    cBase *m_parent;       // 0x00
    void *m_classdesc;     // 0x04

    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcTableTemplateGroup : public cGroup {
public:
    unsigned char mFlag;   // 0x08
    char _pad1[3];         // 0x09..0x0B
    int mField;            // 0x0C

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    const cType *GetType() const;
    const cType *GetManagedType() const;
    const char *GetDataDirectory() const;
    const char *GetFileExtension() const;
    ~gcTableTemplateGroup();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_000998E4;
extern cType *D_0009F494;

// ============================================================
// 0x000d31e4 — Write(cFile &) const
// ============================================================
void gcTableTemplateGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x000d3230 — Read(cFile &, cMemPool *)
// ============================================================
int gcTableTemplateGroup::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || cGroup::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    return result;
}

// ============================================================
// 0x002385cc — AssignCopy(const cBase *)
// ============================================================
void gcTableTemplateGroup::AssignCopy(const cBase *base) {
    gcTableTemplateGroup *src = dcast<gcTableTemplateGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ============================================================
// 0x00238604 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcTableTemplateGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcTableTemplateGroup *result = 0;
    gcTableTemplateGroup *obj =
        (gcTableTemplateGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = (void *)0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = (void *)0x37EA80;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = (void *)0x386A20;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x002386c0 — GetType(void) const
// ============================================================
const cType *gcTableTemplateGroup::GetType() const {
    if (D_000998E4 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998E4 = cType::InitializeType(0, 0, 0x217, D_00040C94,
                                           &gcTableTemplateGroup::New,
                                           0, 0, 8);
    }
    return D_000998E4;
}

// ============================================================
// 0x002387b8 — ~gcTableTemplateGroup(void)
// ============================================================
gcTableTemplateGroup::~gcTableTemplateGroup() {
    *(void **)((char *)this + 4) = (void *)0x386A20;
}

// ============================================================
// 0x000d32ec — GetManagedType(void) const
// ============================================================
const cType *gcTableTemplateGroup::GetManagedType(void) const {
    if (D_0009F494 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F494 = cType::InitializeType(
            0, 0, 0x216, D_000385E4, &gcTableTemplateGroup::New,
            (const char *)0x36D9CC, (const char *)0x36D9DC, 5);
    }
    return D_0009F494;
}

// ============================================================
// 0x000d3414 — GetDataDirectory(void) const
// ============================================================
const char *gcTableTemplateGroup::GetDataDirectory(void) const {
    if (D_0009F494 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F494 = cType::InitializeType(
            0, 0, 0x216, D_000385E4, &gcTableTemplateGroup::New,
            (const char *)0x36D9CC, (const char *)0x36D9DC, 5);
    }
    return (const char *)((int *)D_0009F494)[5];
}

// ============================================================
// 0x000d3540 — GetFileExtension(void) const
// ============================================================
const char *gcTableTemplateGroup::GetFileExtension(void) const {
    if (D_0009F494 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F494 = cType::InitializeType(
            0, 0, 0x216, D_000385E4, &gcTableTemplateGroup::New,
            (const char *)0x36D9CC, (const char *)0x36D9DC, 5);
    }
    return (const char *)((int *)D_0009F494)[6];
}

// ============================================================
// 0x000d366c — IsManagedTypeExternalStatic(void) static
// ============================================================
bool gcTableTemplateGroup::IsManagedTypeExternalStatic() {
    if (D_0009F494 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F494 = cType::InitializeType(
            0, 0, 0x216, D_000385E4, &gcTableTemplateGroup::New,
            (const char *)0x36D9CC, (const char *)0x36D9DC, 5);
    }
    int flags = *(int *)D_0009F494;
    bool result = false;
    if (flags & 1) result = true;
    return result;
}
