class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eDynamicMeshMorphTarget_DelRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

struct eDynamicMeshMorphVert;

class eDynamicMeshMorphTarget {
public:
    int mCount;
    float mWeight;
    eDynamicMeshMorphVert *mVerts;

    eDynamicMeshMorphTarget();
    ~eDynamicMeshMorphTarget();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            __asm__ volatile("" ::: "memory");
            eDynamicMeshMorphTarget_DelRec *rec =
                (eDynamicMeshMorphTarget_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

eDynamicMeshMorphTarget::eDynamicMeshMorphTarget() {
    mCount = 0;
    __asm__ volatile("" ::: "memory");
    mWeight = 0.0f;
    mVerts = 0;
}

eDynamicMeshMorphTarget::~eDynamicMeshMorphTarget() {
    if (mVerts != 0) {
        void *pool = cMemPool::GetPoolFromPtr(mVerts);
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        eDynamicMeshMorphTarget_DelRec *rec =
            (eDynamicMeshMorphTarget_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, mVerts);
        mVerts = 0;
    }
}
