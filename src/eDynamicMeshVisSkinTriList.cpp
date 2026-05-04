extern "C" void *memset(void *, int, unsigned int);
extern "C" void free(void *);

#pragma control sched=1

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eDynamicMeshVisSkinTriList_DelRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eDynamicMeshVisSkinTriList {
public:
    int mField0;
    int mField4;
    int mField8;
    int mFieldC;
    short mField10;
    short mField12;
    short mField14;
    short mPad16;
    int mField18;
    void *mField1C;
    float mField20;

    eDynamicMeshVisSkinTriList(void);
    ~eDynamicMeshVisSkinTriList();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            __asm__ volatile("" ::: "memory");
            eDynamicMeshVisSkinTriList_DelRec *rec =
                (eDynamicMeshVisSkinTriList_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

eDynamicMeshVisSkinTriList::eDynamicMeshVisSkinTriList(void) {
    mField8 = 0;
    mFieldC = 0;
    mField10 = 0;
    mField12 = 0;
    mField14 = 0;
    mField18 = 0;
    mField1C = 0;
    __asm__ volatile("" ::: "memory");
    mField20 = 0.0f;
    memset(this, 0, 8);
}

eDynamicMeshVisSkinTriList::~eDynamicMeshVisSkinTriList() {
    if (mField1C != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(mField1C);
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        eDynamicMeshVisSkinTriList_DelRec *rec =
            (eDynamicMeshVisSkinTriList_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, mField1C);
        mField1C = 0;
    }
}
