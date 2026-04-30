class cBase;
class cFile;
class cGUID;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    int Load(void);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cMemBlockAllocation {
public:
    cMemBlockAllocation(cMemPool *, unsigned int, bool);
    ~cMemBlockAllocation();
    char _pad[0x18];
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cFactory : public cObject {
public:
    char _pad0[72];
    unsigned int mField48;

    ~cFactory(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Clean(bool flag);
    void LoadLocalized(void);
    void LoadLocalized(const char *);
    int Load(void);
    cObject *CreateObject(const cType *, const cGUID &, bool, unsigned int, bool);
    cObject *FindObject(const cType *, const cGUID &, bool) const;
    void *FindGroup(const cType *);
    void ClearVisitedReferences(unsigned int);
    void MarkForClean(unsigned int);
    void DeleteMarkedForClean(unsigned int, bool);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

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

class cGroup {
public:
    cObject *CreateObject(const cType *, const cGUID &, cMemPool *, bool, bool);
    cObject *FindObject(const cGUID &) const;
};

void cFile_SetCurrentPos(void *, unsigned int);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C90;

// ── cFactory::Write(cFile &) const ──
void cFactory::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cObject *)this)->Write(file);
    wb.End();
}

// ── cFactory::Clean(bool) ──
void cFactory::Clean(bool flag) {
    this->ClearVisitedReferences(0xfe00);
    this->MarkForClean(this->mField48);
    this->DeleteMarkedForClean(this->mField48, flag);
}

// ── cFactory::LoadLocalized(void) ──
class cStr {
public:
    char _data[256];
    cStr() { _data[0] = '\0'; }
    operator const char *() const { return _data; }
};

class cGUID;
class cType;

struct TypeDispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *);
};

namespace cObject_Static {
    void GetLocalizedFilename(const cType *, const cGUID &, cStr *);
}

void cFactory::LoadLocalized(void) {
    char *classdesc = *(char **)((char *)this + 4);
    cStr strBuf;
    TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 8);
    cType *type = entry->fn((char *)this + entry->offset);
    cObject_Static::GetLocalizedFilename(type, *(cGUID *)((char *)this + 32), &strBuf);
    this->LoadLocalized((const char *)strBuf);
}

cObject *cFactory::FindObject(const cType *type, const cGUID &guid,
                              bool searchParents) const {
    cObject *object;
    cFactory *factory;
    cGroup *group = (cGroup *)((cFactory *)this)->FindGroup(type);

    if (group != 0) {
        object = group->FindObject(guid);
        if (object != 0) {
            goto found;
        }
    }
    if (((unsigned char)searchParents) == 0) {
        goto not_found;
    }
    factory = *(cFactory *const *)this;
    if (factory != 0) {
        goto init;
    }
    factory = 0;
    goto recurse_parent;

not_found:
    return 0;

found:
    return object;

init: {
        const char *baseName = (const char *)0x36C830;
        const char *baseDesc = (const char *)0x36C838;
        cBase *(*namedNew)(cMemPool *, cBase *) = &cNamed::New;

loop:
        if (D_00040C90 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(baseName,
                                                           baseDesc,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       namedNew, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_00040C90 = cType::InitializeType(0, 0, 5, D_000385E4,
                                               0, 0, 0, 0);
        }

        const cType *factoryType = D_00040C90;
        char *classdesc = *(char **)((char *)factory + 4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 8);
        cType *objType = entry->fn((char *)factory + entry->offset);
        int isFactory;

        if (factoryType == 0) {
            isFactory = 0;
        } else if (objType != 0) {
            do {
                if (objType == factoryType) {
                    isFactory = 1;
                    goto type_done;
                }
                objType = (cType *)objType->mParent;
            } while (objType != 0);
            isFactory = 0;
        } else {
            isFactory = 0;
        }

type_done:
        if (isFactory == 0) {
            goto advance_factory;
        } else {
            goto recurse_parent;
        }
advance_factory:
        factory = *(cFactory **)factory;
        if (factory != 0) {
            goto loop;
        }
        factory = 0;
    }

recurse_parent:
    if (factory == 0) {
        goto not_found;
    }
    return factory->FindObject(type, guid, true);
}

cObject *cFactory::CreateObject(const cType *type, const cGUID &guid, bool flag,
                                unsigned int size, bool flag2) {
    cGroup *group = (cGroup *)this->FindGroup(type);
    if (group == 0) {
        return 0;
    }

    cMemPool *pool = cMemPool::GetPoolFromPtr(group);
    if (size != 0) {
        cMemBlockAllocation alloc(pool, size, false);
        cObject *object = group->CreateObject(type, guid, pool, flag, flag2);
        return object;
    }

    return group->CreateObject(type, guid, pool, flag, flag2);
}

// ── cFactory::Load(void) ──
struct LoadDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, unsigned int, int, int, int, int);
};

int cFactory::Load(void) {
    this->LoadLocalized();
    if (!((cObject *)this)->Load()) {
        return 0;
    }
    unsigned int f48 = this->mField48;
    if (f48 != 0) {
        char *classdesc = *(char **)((char *)this + 4);
        LoadDispatchEntry *entry = (LoadDispatchEntry *)(classdesc + 96);
        entry->fn((char *)this + entry->offset, f48, 0, 0, 0, 1);
    }
    return 1;
}

// ── cFactory::GetType(void) const ──
const cType *cFactory::GetType(void) const {
    if (D_00040C90 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36C830,
                                                       (const char *)0x36C838,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00040C90 = cType::InitializeType(0, 0, 5, D_000385E4,
                                           0, 0, 0, 0);
    }
    return D_00040C90;
}

// ── cFactory::~cFactory(void) ──
extern char cFactoryvirtualtable[];

cFactory::~cFactory() {
    *(void **)((char *)this + 4) = cFactoryvirtualtable;
}

// ── cFactory::Read(cFile &, cMemPool *) ──
int cFactory::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cObject::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
