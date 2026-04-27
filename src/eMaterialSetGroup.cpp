// AssignCopy trampolines for engine *Group classes that copy a flag byte
// (offset 8) and an int field (offset 12) from a downcast source object.

class cBase;
class cFile;

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

class eMaterialSetGroup : public cGroup {
public:
    eMaterialSetGroup(cBase *);
    ~eMaterialSetGroup();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
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

class eSurfaceGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSpriteGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

extern char eMaterialSetGroupvirtualtable[];
extern char cBasevirtualtable[];
extern char cGroupvirtualtable[];

void eMaterialSetGroup::AssignCopy(const cBase *base) {
    eMaterialSetGroup *src = dcast<eMaterialSetGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eSurfaceGroup::AssignCopy(const cBase *base) {
    eSurfaceGroup *src = dcast<eSurfaceGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eSpriteGroup::AssignCopy(const cBase *base) {
    eSpriteGroup *src = dcast<eSpriteGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ── eMaterialSetGroup::Write(cFile &) const @ 0x000153C0 ──
void eMaterialSetGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eMaterialSetGroup::~eMaterialSetGroup(void) @ 0x001DC5B8 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail dispatch through
// operator delete (which itself does pool/block lookup and calls the slot
// fn). Same pattern as eSphereShape::~eSphereShape (matched).
eMaterialSetGroup::~eMaterialSetGroup() {
    *(void **)((char *)this + 4) = eMaterialSetGroupvirtualtable;
}

// ── eMaterialSetGroup::New(cMemPool *, cBase *) static @ 0x001DC404 ──
cBase *eMaterialSetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eMaterialSetGroup *result = 0;
    eMaterialSetGroup *obj = (eMaterialSetGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eMaterialSetGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ----------------------------------------------------------------------
// lflush @ 0x001ab114 (28B) — wraps fflush(NULL)
// ----------------------------------------------------------------------
extern "C" {
    int fflush(void);

    void lflush(void) {
        fflush();
    }

    // ------------------------------------------------------------------
    // atof @ 0x001ac584 (28B) — wraps strtod(s, NULL)
    // ------------------------------------------------------------------
    double strtod(const char *, char **);

    double atof(const char *s) {
        return strtod(s, 0);
    }
}
