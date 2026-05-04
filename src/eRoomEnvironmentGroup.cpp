class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

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

class eRoomEnvironment {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B10;

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

class eRoomEnvironmentGroup : public cGroup {
public:
    eRoomEnvironmentGroup(cBase *);
    ~eRoomEnvironmentGroup();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char eRoomEnvironmentGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

void eRoomEnvironmentGroup::AssignCopy(const cBase *base) {
    eRoomEnvironmentGroup *src = dcast<eRoomEnvironmentGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eRoomEnvironmentGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void eRoomEnvironmentGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

int eRoomEnvironmentGroup::Read(cFile &file, cMemPool *pool) {
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

cBase *eRoomEnvironmentGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eRoomEnvironmentGroup *result = 0;
    eRoomEnvironmentGroup *obj = (eRoomEnvironmentGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eRoomEnvironmentGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

const cType *eRoomEnvironmentGroup::GetManagedType(void) const {
    if (D_00046B10 == 0) {
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
        D_00046B10 = cType::InitializeType(
            0, 0, 0x235, D_000385E4, &eRoomEnvironment::New,
            (const char *)0x36CEBC, (const char *)0x36CED0, 0);
    }
    return D_00046B10;
}

const char *eRoomEnvironmentGroup::GetDataDirectory(void) const {
    if (D_00046B10 == 0) {
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
        D_00046B10 = cType::InitializeType(
            0, 0, 0x235, D_000385E4, &eRoomEnvironment::New,
            (const char *)0x36CEBC, (const char *)0x36CED0, 0);
    }
    return (const char *)((int *)D_00046B10)[5];
}

eRoomEnvironmentGroup::~eRoomEnvironmentGroup() {
    ((void **)this)[1] = eRoomEnvironmentGroupvirtualtable;
}
