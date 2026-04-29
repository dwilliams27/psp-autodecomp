// gcUIDialogGroup: AssignCopy + Write/Read serialization + destructor.
//
// Same shape as gcConstantGroup / gcFunctionGroup (parent cGroup; cBase
// header 8 bytes; vtable at offset 4). Patterns mirror cFactory::Write /
// eWeatherEffect::Read / eSphereShape::~eSphereShape — SNC's ABI
// auto-generates the dtor's (this!=0) guard, the chain call to ~cGroup,
// and the deleting-tail dispatch through operator delete.

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
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cGroup {
public:
    char _pad[8];
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcUIDialogGroup : public cGroup {
public:
    unsigned char mFlag;
    char _pad1[3];
    int mField;

    gcUIDialogGroup(cBase *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    ~gcUIDialogGroup();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcEntityTemplateGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcEnumerationGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcStateMachineGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcEntityCustomAttackGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char gcUIDialogGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998B4;

// ── gcUIDialogGroup::AssignCopy(const cBase *) ──
void gcUIDialogGroup::AssignCopy(const cBase *base) {
    gcUIDialogGroup *src = dcast<gcUIDialogGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ── gcUIDialogGroup::New(cMemPool *, cBase *) static @ 0x00236924 ──
cBase *gcUIDialogGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcUIDialogGroup *result = 0;
    gcUIDialogGroup *obj =
        (gcUIDialogGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcUIDialogGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcUIDialogGroup::GetType(void) const @ 0x002369E0 ──
const cType *gcUIDialogGroup::GetType() const {
    if (D_000998B4 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998B4 = cType::InitializeType(0, 0, 0x8B, D_00040C94,
                                           &gcUIDialogGroup::New,
                                           0, 0, 8);
    }
    return D_000998B4;
}

// ── gcUIDialogGroup::Write(cFile &) const @ 0x000CECB4 ──
void gcUIDialogGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcUIDialogGroup::Read(cFile &, cMemPool *) @ 0x000CED00 ──
int gcUIDialogGroup::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcUIDialogGroup::~gcUIDialogGroup(void) @ 0x00236AD8 ──
gcUIDialogGroup::~gcUIDialogGroup() {
    *(void **)((char *)this + 4) = gcUIDialogGroupvirtualtable;
}

bool gcEntityTemplateGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcEnumerationGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcStateMachineGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcEntityCustomAttackGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
