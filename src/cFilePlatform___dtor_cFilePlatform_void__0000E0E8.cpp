// cFilePlatform::~cFilePlatform(void) — 0x0000E0E8 (deleting destructor variant)
// Emits __0oNcFilePlatformdtv.
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard
// and the deleting-tail dispatch through operator delete. The real dtor body
// is just this->Close(). operator delete does pool lookup; if no pool, free().
//
// Split TU per SPLIT-TU rule: cFilePlatform is locally redeclared so this dtor
// can declare its own operator delete without clashing with the flat layout
// used elsewhere.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

class cFilePlatform {
public:
    char _pad[0x104];
    int mSize;
    int mHandle;
    char _pad2[0x14];

    void Close(void);
    ~cFilePlatform();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
};

cFilePlatform::~cFilePlatform() {
    Close();
}
