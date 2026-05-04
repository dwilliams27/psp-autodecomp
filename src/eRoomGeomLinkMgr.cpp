class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" {
    void free(void *);
}

struct DeleteRecord {
    short offset;
    short _pad;
    void *fn;
};

class eRoomGeomLinkMgr {
public:
    void *mField0;

    ~eRoomGeomLinkMgr();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            void *self = p;
            DeleteRecord *rec =
                (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            void *fn = rec->fn;
            ((void (*)(void *, void *, void *, short))fn)((char *)block + off, self, fn, off);
        } else {
            free(p);
        }
    }
};

eRoomGeomLinkMgr::~eRoomGeomLinkMgr() {
    if (mField0 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(mField0);
        void *block = *(void **)((char *)pool + 0x24);
        void *res = mField0;
        DeleteRecord *rec =
            (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        void *fn = rec->fn;
        ((void (*)(void *, void *, void *, short))fn)((char *)block + off, res, fn, off);
        mField0 = 0;
    }
}
