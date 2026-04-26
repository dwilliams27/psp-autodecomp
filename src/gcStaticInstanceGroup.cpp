class cBase;
class cFile;
class cReadBlock;
class cMemPool;
class eGeom;

template <class T> T *dcast(const cBase *);

class gcStaticInstanceGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
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
    void Write(int, const unsigned short *);
};

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
