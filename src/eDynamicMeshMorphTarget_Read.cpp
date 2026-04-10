class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cReadBlock {
public:
    int _data[5];
};

struct eDynamicMeshMorphVert {
    int _data[2];
    void Read(cReadBlock &);
};

class eDynamicMeshMorphTarget {
public:
    int mCount;
    float mWeight;
    eDynamicMeshMorphVert *mVerts;

    void Read(cReadBlock &);
};

void eDynamicMeshMorphTarget::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], this, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 4, 4);

    void *pool = cMemPool::GetPoolFromPtr(this);
    void *block = *(void **)((char *)pool + 0x24);
    __asm__ volatile("" ::: "memory");
    PoolBlock *pb = (PoolBlock *)block;
    AllocEntry *entry = (AllocEntry *)(pb->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    mVerts = (eDynamicMeshMorphVert *)entry->fn(base, mCount * 8, 0, 0, 0);

    int count = mCount;
    eDynamicMeshMorphVert *verts = mVerts;
    for (int i = 0; i < count; i++) {
        verts[i].Read(rb);
    }
}
