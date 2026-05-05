// gcEntityGroup — inherits cGroup, holds a bool flag and an int.
// Decompiled functions:
//   0x000cf278 gcEntityGroup::Write(cFile &) const
//   0x000cf2c4 gcEntityGroup::Read(cFile &, cMemPool *)
//   0x00236b54 gcEntityGroup::AssignCopy(const cBase *)
//   0x00236b8c gcEntityGroup::New(cMemPool *, cBase *) static
//   0x00236d24 gcEntityGroup::IsManagedTypeExternal(void) const
//   0x00236d40 gcEntityGroup::~gcEntityGroup(void)

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

class gcEntity {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009A408;

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

class gcEntityGroup : public cGroup {
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
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
    const char *GetFileExtension(void) const;
    ~gcEntityGroup();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

// ============================================================
// 0x000cf278 — Write(cFile &) const
// ============================================================
void gcEntityGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x000cf2c4 — Read(cFile &, cMemPool *)
// ============================================================
int gcEntityGroup::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || cGroup::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    return result;
}

// ============================================================
// 0x00236b54 — AssignCopy(const cBase *)
// ============================================================
void gcEntityGroup::AssignCopy(const cBase *base) {
    gcEntityGroup *src = dcast<gcEntityGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ============================================================
// 0x00236b8c — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcEntityGroup *result = 0;
    gcEntityGroup *obj = (gcEntityGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = (void *)0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = (void *)0x37EA80;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = (void *)0x3863F0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x000cf380 — GetManagedType(void) const
// ============================================================
const cType *gcEntityGroup::GetManagedType(void) const {
    if (D_0009A408 == 0) {
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
        D_0009A408 = cType::InitializeType(
            0, 0, 0x8C, D_000385E4, &gcEntity::New,
            (const char *)0x36D968, (const char *)0x36D974, 0);
    }
    return D_0009A408;
}

// ============================================================
// 0x000cf4a8 — GetDataDirectory(void) const
// ============================================================
const char *gcEntityGroup::GetDataDirectory(void) const {
    if (D_0009A408 == 0) {
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
        D_0009A408 = cType::InitializeType(
            0, 0, 0x8C, D_000385E4, &gcEntity::New,
            (const char *)0x36D968, (const char *)0x36D974, 0);
    }
    return (const char *)((int *)D_0009A408)[5];
}

// ============================================================
// 0x000cf5d4 — GetFileExtension(void) const
// ============================================================
const char *gcEntityGroup::GetFileExtension(void) const {
    if (D_0009A408 == 0) {
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
        D_0009A408 = cType::InitializeType(
            0, 0, 0x8C, D_000385E4, &gcEntity::New,
            (const char *)0x36D968, (const char *)0x36D974, 0);
    }
    return (const char *)((int *)D_0009A408)[6];
}

// ============================================================
// 0x00236d24 — IsManagedTypeExternal(void) const
// ============================================================
bool gcEntityGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ============================================================
// 0x00236d40 — ~gcEntityGroup(void)
// ============================================================
gcEntityGroup::~gcEntityGroup() {
    *(void **)((char *)this + 4) = (void *)0x3863F0;
}
