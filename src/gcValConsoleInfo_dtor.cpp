// ODR-WARNING: split-TU method addition for gcValConsoleInfo. The canonical
// header does not declare this destructor/operator delete pair; keep this local
// redeclaration isolated so matched siblings in src/gcValConsoleInfo.cpp do not
// see new class members.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValConsoleInfo {
public:
    ~gcValConsoleInfo();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char cBaseclassdesc[];
extern char gcValConsoleInfovirtualtable[];

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oQgcValConsoleInfodtv, 0x128\n");

// -----------------------------------------------------------------------------
// gcValConsoleInfo::~gcValConsoleInfo(void) @ 0x00325234, 296B
// -----------------------------------------------------------------------------
gcValConsoleInfo::~gcValConsoleInfo() {
    *(char **)((char *)this + 4) = gcValConsoleInfovirtualtable;
    char *slot10 = (char *)this + 0x10;
    char *slot14 = (char *)this + 0x14;
    if (slot14 != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }
    if (slot10 != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x10) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}
