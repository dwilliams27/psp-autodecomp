class cBase;
class cFile;
class cMemPool;

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

class eSurfaceSetGroup : public cGroup {
public:
    eSurfaceSetGroup(cBase *);
    ~eSurfaceSetGroup();
    void AssignCopy(const cBase *);
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

class eStaticLightGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eLensFlareGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eCameraEffectGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class ePointGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

extern char eSurfaceSetGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

void eSurfaceSetGroup::AssignCopy(const cBase *base) {
    eSurfaceSetGroup *src = dcast<eSurfaceSetGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eStaticLightGroup::AssignCopy(const cBase *base) {
    eStaticLightGroup *src = dcast<eStaticLightGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eLensFlareGroup::AssignCopy(const cBase *base) {
    eLensFlareGroup *src = dcast<eLensFlareGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eCameraEffectGroup::AssignCopy(const cBase *base) {
    eCameraEffectGroup *src = dcast<eCameraEffectGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool ePointGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── eSurfaceSetGroup::Write(cFile &) const @ 0x00016AA0 ──
void eSurfaceSetGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSurfaceSetGroup::New(cMemPool *, cBase *) static @ 0x001DCDA4 ──
cBase *eSurfaceSetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSurfaceSetGroup *result = 0;
    eSurfaceSetGroup *obj = (eSurfaceSetGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSurfaceSetGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSurfaceSetGroup::~eSurfaceSetGroup(void) @ 0x001DCF58 ──
eSurfaceSetGroup::~eSurfaceSetGroup() {
    ((void **)this)[1] = eSurfaceSetGroupvirtualtable;
}
