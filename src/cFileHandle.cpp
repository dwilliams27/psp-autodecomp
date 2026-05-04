// cFileHandle::~cFileHandle(void) @ 0x001c8370 (176B, cAll_psp.obj)
//
// cFileHandle embeds a cBufferedFile member at offset 0x00 and adds 4
// trailing fields used to track an in-flight read request.  The destructor
// optionally re-issues a Close() before zeroing the request bookkeeping;
// SNC then auto-emits ~mBuffered(2) and the deleting-tail dispatch.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct DelRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cBufferedFile {
public:
    char _pad[0x148];

    void Close();
    ~cBufferedFile();
};

class cFileHandle {
public:
    cBufferedFile mBuffered;        // 0x000 - 0x147
    int mUnk148;                    // 0x148
    int mUnk14C;                    // 0x14C
    int mUnk150;                    // 0x150
    unsigned char mUnk154;          // 0x154

    ~cFileHandle();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DelRec *rec =
                (DelRec *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

cFileHandle::~cFileHandle() {
    if (mUnk154 == 0) {
        mBuffered.Close();
    }
    mUnk148 = 0;
    mUnk14C = 0;
    mUnk150 = 0;
    mUnk154 = 0;
}
