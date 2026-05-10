// ODR-WARNING: split translation unit for gcValStringCompare destructor only.
// Keep local declarations minimal to avoid register-allocation drift in matched siblings.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern char cBaseclassdesc[];
extern char gcValStringComparevirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValStringCompare {
public:
    ~gcValStringCompare();
    static void operator delete(void *);
};

inline void gcValStringCompare::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DtorDeleteRecord *rec =
        (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcValStringComparedtv, 0x128\n");

gcValStringCompare::~gcValStringCompare() {
    *(char **)((char *)this + 4) = gcValStringComparevirtualtable;
    char *second = (char *)this + 0x08;

    if ((void *)((char *)this + 0x0C) != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0) {
            if (val != 0) {
                char *obj = (char *)val;
                char *type = ((char **)obj)[1];
                DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn(obj + off, (void *)3);
                *(int *)((char *)this + 0x0C) = 0;
            }
        }
    }

    if ((void *)second != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
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
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}
