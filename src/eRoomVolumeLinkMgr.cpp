#pragma control sched=1

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct eRoomVolumeLinkMgr_DelRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eRoomVolumeLinkMgr {
public:
    void *mField0;

    ~eRoomVolumeLinkMgr();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            __asm__ volatile("" ::: "memory");
            eRoomVolumeLinkMgr_DelRec *rec =
                (eRoomVolumeLinkMgr_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

eRoomVolumeLinkMgr::~eRoomVolumeLinkMgr() {
    if (mField0 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(mField0);
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        eRoomVolumeLinkMgr_DelRec *rec =
            (eRoomVolumeLinkMgr_DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, mField0);
        mField0 = 0;
    }
}
