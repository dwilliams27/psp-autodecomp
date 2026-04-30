// gcNetworkConfigStrings — gcAll_psp.obj
//   0x00285de8 New(cMemPool *, cBase *) static            (144B)
//   0x00285f54 Write(cFile &) const                       (100B)
//
// Layout: derives from gcStringValue, embeds gcDesiredValue at offset 0x8,
// has int member at 0xC. Total size 0x10. Type ID for Write block = 10.

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

extern char gcNetworkConfigStringsvirtualtable[];
extern char cBaseclassdesc[];

class gcStringValue {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcNetworkConfigStrings : public gcStringValue {
public:
    ~gcNetworkConfigStrings();
    void Write(cFile &) const;
    static gcNetworkConfigStrings *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// 0x00285f54 — Write(cFile &) const
void gcNetworkConfigStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 10);
    gcStringValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 0xC));
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// 0x00285de8 — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcNetworkConfigStrings *gcNetworkConfigStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcNetworkConfigStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = (int)gcNetworkConfigStringsvirtualtable;
        *(int *)((char *)p + 8)  = (int)p | 1;
        *(int *)((char *)p + 0xC) = 0;
        result = (gcNetworkConfigStrings *)p;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oWgcNetworkConfigStringsdtv, 0xd4\n");

// 0x0028652c - gcNetworkConfigStrings::~gcNetworkConfigStrings(void)
gcNetworkConfigStrings::~gcNetworkConfigStrings() {
    *(char **)((char *)this + 4) = gcNetworkConfigStringsvirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
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
