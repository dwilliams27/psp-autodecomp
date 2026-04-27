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
    void End(void);
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

class eSoundDataGroup : public cGroup {
public:
    eSoundDataGroup(cBase *);
    ~eSoundDataGroup();
    void Write(cFile &) const;
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcStringTableGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class gcTriggerGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcCinematicGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcDesiredPoint;
class gcDesiredPointHelper;
class ePoint;

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
    cBase *GetObject(bool) const;
};

template <class T, class H, class O>
cBase *gcDesiredObjectT<T, H, O>::GetObject(bool b) const {
    return (cBase *)Get(b);
}

template class gcDesiredObjectT<gcDesiredPoint, gcDesiredPointHelper, ePoint>;

extern char eSoundDataGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

void eSoundDataGroup::AssignCopy(const cBase *base) {
    eSoundDataGroup *src = dcast<eSoundDataGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void gcStringTableGroup::AssignCopy(const cBase *base) {
    gcStringTableGroup *src = dcast<gcStringTableGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcTriggerGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcCinematicGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ── eSoundDataGroup::Write(cFile &) const @ 0x00015974 ──
void eSoundDataGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSoundDataGroup::~eSoundDataGroup(void) @ 0x001dc820 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the ~cGroup() base-chain call (with flags=0), and the deleting-tail
// `if (flags & 1) operator delete(this)` epilogue.
eSoundDataGroup::~eSoundDataGroup() {
    *(void **)((char *)this + 4) = eSoundDataGroupvirtualtable;
}

// ── eSoundDataGroup::New(cMemPool *, cBase *) static @ 0x001dc66c ──
cBase *eSoundDataGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSoundDataGroup *result = 0;
    eSoundDataGroup *obj = (eSoundDataGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSoundDataGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}
