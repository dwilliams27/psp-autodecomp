class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;

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

class gcStaticInstanceGroup : public cGroup {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    ~gcStaticInstanceGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcFunctionGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const unsigned short *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos_Group(void *, unsigned int);

extern char gcStaticInstanceGroupvirtualtable[];  // 0x00386090
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998A0;

class eDynamicMeshExtrudedShadowFace {
public:
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mIndex;
    static void SkipRead(cReadBlock &, cMemPool *);
    void Read(cReadBlock &, cMemPool *);
};

class eRoom {
public:
    void RemoveGeom(eGeom *);
    void ClearRoomGeomList(eGeom *);
};

class eGeomBody {
public:
    char _pad[0x68];
    int mField68;
};

void gcStaticInstanceGroup::AssignCopy(const cBase *base) {
    gcStaticInstanceGroup *src = dcast<gcStaticInstanceGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

const cType *gcStaticInstanceGroup::GetType(void) const {
    if (D_000998A0 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998A0 = cType::InitializeType(0, 0, 0x68, D_00040C94,
                                           &gcStaticInstanceGroup::New, 0, 0, 8);
    }
    return D_000998A0;
}

// ── gcStaticInstanceGroup::New(cMemPool *, cBase *) static @ 0x00235d1c ──
cBase *gcStaticInstanceGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcStaticInstanceGroup *result = 0;
    gcStaticInstanceGroup *obj =
        (gcStaticInstanceGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcStaticInstanceGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

void gcFunctionGroup::AssignCopy(const cBase *base) {
    gcFunctionGroup *src = dcast<gcFunctionGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

#pragma control sched=1
void eDynamicMeshExtrudedShadowFace::Write(cWriteBlock &wb) const {
    wb.Write(3, (const unsigned short *)this);
}
#pragma control sched=2

void cHandle::SkipRead(cReadBlock &rb, cMemPool *pool) {
    cHandle tmp;
    tmp.mIndex = 0;
    tmp.Read(rb, pool);
}

void eRoom::RemoveGeom(eGeom *geom) {
    ClearRoomGeomList(geom);
    ((eGeomBody *)geom)->mField68 = 0;
}

// ── gcStaticInstanceGroup::Write(cFile &) const @ 0x000ccfe0 ──
// Same shape as cFactory::Write — chain to cGroup::Write under a cWriteBlock.
void gcStaticInstanceGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->cGroup::Write(file);
    wb.End();
}

// ── gcStaticInstanceGroup::Read(cFile &, cMemPool *) @ 0x000cd02c ──
// Same shape as eWeatherEffect::Read.
int gcStaticInstanceGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos_Group(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcStaticInstanceGroup::~gcStaticInstanceGroup(void) @ 0x00235ed0 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail `if (flags & 1)`
// dispatch via operator delete.
gcStaticInstanceGroup::~gcStaticInstanceGroup() {
    *(void **)((char *)this + 4) = gcStaticInstanceGroupvirtualtable;
}
