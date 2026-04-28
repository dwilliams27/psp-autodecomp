// eTextureGroup and friends.
//
// Same shape as gcCinematicGroup / gcEnumerationGroup (parent cGroup; cBase
// header 8 bytes; vtable at offset 4). Patterns mirror cFactory::Write /
// gcCinematicGroup::~gcCinematicGroup — SNC's ABI auto-generates the dtor's
// (this!=0) guard, the chain call to ~cGroup, and the deleting-tail dispatch
// through operator delete.

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

class cGroup {
public:
    char _pad[8];
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
};

class eTextureGroup : public cGroup {
public:
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    eTextureGroup(cBase *);
    ~eTextureGroup();
    void Write(cFile &) const;
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const class cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class eMaterialGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const class cBase *);
};

class eMaterialSetGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eSurfaceGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eSpriteGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class ePathGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eMeshGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eGeomTemplateGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char eTextureGroupvirtualtable[];

void eTextureGroup::AssignCopy(const cBase *base) {
    eTextureGroup *src = dcast<eTextureGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eTextureGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── eTextureGroup::Write(cFile &) const @ 0x0001372c ──
void eTextureGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eTextureGroup::~eTextureGroup(void) @ 0x001db9b0 ──
eTextureGroup::~eTextureGroup() {
    *(void **)((char *)this + 4) = eTextureGroupvirtualtable;
}

void eMaterialGroup::AssignCopy(const cBase *base) {
    eMaterialGroup *src = dcast<eMaterialGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eMaterialGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eMaterialSetGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eSurfaceGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eSpriteGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool ePathGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eMeshGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eGeomTemplateGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
