class cBase;
class cFile;
class cMemPool;
class eGeom;

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
