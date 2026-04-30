class cBase;
class cFile;
class cMemPool;
class cType;
class eShape;
class mOCS;
class mPlane;
class eBspNode;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eBspTree {
public:
    eBspTree(cBase *);
    void AssignCopy(const cBase *);
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    int Contains(const eShape *, const mOCS &) const;
    void Contains(int, const eShape *, const mOCS &, bool *, bool *) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
    void Write(int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_00040F78;

void cFile_SetCurrentPos(void *, unsigned int);
void eBspTree::AssignCopy(const cBase *base) {
    eBspTree *other = dcast<eBspTree>(base);
    const cArrayBase<eBspNode> *otherNodes =
        (const cArrayBase<eBspNode> *)((char *)other + 8);
    __asm__ volatile("" :: "r"(otherNodes) : "memory");
    ((cArrayBase<eBspNode> *)((char *)this + 8))->operator=(*otherNodes);
    ((cArrayBase<mPlane> *)((char *)this + 0xC))->operator=(
        *(const cArrayBase<mPlane> *)((char *)other + 0xC));
    *(int *)((char *)this + 0x10) = *(int *)((char *)other + 0x10);
}

void eBspTree::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

#pragma control sched=1
void eBspTree::Write(cFile &file) const {
    cWriteBlock wb(file, 1);

    const unsigned char *nodes =
        *(const unsigned char *const *)((const char *)this + 8);
    int nodeCount0 = 0;
    if (nodes != 0) {
        nodeCount0 = ((const int *)nodes)[-1] & 0x3FFFFFFF;
    }
    wb.Write(nodeCount0);

    int nodeCount = 0;
    nodes = *(const unsigned char *const *)((const char *)this + 8);
    if (nodes != 0) {
        nodeCount = ((const int *)nodes)[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < nodeCount) {
        int offset = 0;
        const unsigned char *node = nodes + offset;
        do {
            wb.Write(node[0]);
            wb.Write(node[1]);
            wb.Write(node[2]);
            i++;
            node += 3;
        } while (i < nodeCount);
    }

    const float *planes = *(const float *const *)((const char *)this + 0xC);
    int planeCount0 = 0;
    if (planes != 0) {
        planeCount0 = ((const int *)planes)[-1] & 0x3FFFFFFF;
    }
    wb.Write(planeCount0);

    int planeCount = 0;
    planes = *(const float *const *)((const char *)this + 0xC);
    if (planes != 0) {
        planeCount = ((const int *)planes)[-1] & 0x3FFFFFFF;
    }

    int j = 0;
    if (j < planeCount) {
        int offset = 0;
        const float *plane = (const float *)((const char *)planes + offset);
        do {
            wb.Write(3, plane);
            wb.Write(*(const float *)((const char *)plane + 0xC));
            j++;
            plane = (const float *)((const char *)plane + 0x10);
        } while (j < planeCount);
    }

    wb.End();
}
#pragma control sched=2

int eBspTree::Contains(const eShape *shape, const mOCS &ocs) const {
    bool flags[2];
    flags[0] = false;
    flags[1] = false;
    Contains(0, shape, ocs, &flags[1], &flags[0]);
    if (flags[0]) {
        if (flags[1]) {
            return 1;
        }
        goto outside;
    }
    return 2;
outside:
    return 0;
}

cBase *eBspTree::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eBspTree *result = 0;
    eBspTree *obj = (eBspTree *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) eBspTree(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eBspTree::GetType(void) const {
    if (D_00040F78 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                               (const char *)0x36CD7C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00040F78 = cType::InitializeType(0, 0, 0x25A, D_000385DC,
                                           &eBspTree::New, 0, 0, 0);
    }
    return D_00040F78;
}
