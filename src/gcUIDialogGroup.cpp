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

template <class T> T *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

// ── gcUIDialogGroup::AssignCopy(const cBase *) ──
void gcUIDialogGroup::AssignCopy(const cBase *base) {
    gcUIDialogGroup *src = dcast<gcUIDialogGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
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
