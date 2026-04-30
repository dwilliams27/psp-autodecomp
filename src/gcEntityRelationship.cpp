// gcEntityRelationship — gcAll_psp.obj
//   0x00152fe4 Write(cFile &) const                       (88B)
//   0x0015303c Read(cFile &, cMemPool *)                  (200B)
//   0x0031aa00 New(cMemPool *, cBase *) static            (220B)
//
// Layout: derives from gcObjectRelationship, embeds gcDesiredEntityHelper
// at offset 0x20.  Total size 0x28.

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
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcObjectRelationship {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredEntityHelper {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

void gcDesiredEntityHelper_gcDesiredEntityHelper(void *, int, int, int);

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcEntityRelationship : public gcObjectRelationship {
public:
    ~gcEntityRelationship();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static gcEntityRelationship *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oUgcEntityRelationshipdtv, 0xf0\n");

// 0x0031add4 — ~gcEntityRelationship(void)
gcEntityRelationship::~gcEntityRelationship() {
    *(char **)((char *)this + 4) = (char *)0x388928;
    char *baseVtable = (char *)0x37E6A8;
    char *sub = (char *)this + 8;
    if (sub != 0) {
        *(char **)((char *)this + 0xC) = (char *)0x388568;
        char *slot = (char *)this + 0x1C;
        if (slot != 0) {
            int keep = 1;
            int val = *(int *)((char *)this + 0x1C);
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
                *(int *)((char *)this + 0x1C) = 0;
            }
        }
        *(char **)((char *)this + 0xC) = baseVtable;
    }
    *(char **)((char *)this + 4) = baseVtable;
}

// 0x00152fe4 — Write(cFile &) const
void gcEntityRelationship::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcObjectRelationship::Write(file);
    ((const gcDesiredEntityHelper *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}

// 0x0015303c — Read(cFile &, cMemPool *)
int gcEntityRelationship::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcObjectRelationship::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredEntityHelper *)((char *)this + 0x20))->Read(rb);
    return result;
}

// 0x0031aa00 — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcEntityRelationship *gcEntityRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcEntityRelationship *result = 0;
    int *p = (int *)entry->fn(base, 0x28, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = 0x388928;
        *(int *)((char *)p + 12) = 0x37E6A8;
        *(int **)((char *)p + 8) = p;
        *(int *)((char *)p + 12) = 0x388568;
        *((unsigned char *)p + 16) = 1;
        *((unsigned char *)p + 17) = 0;
        *(int *)((char *)p + 20) = 0;
        *(int *)((char *)p + 24) = 0;
        *(int *)((char *)p + 28) = (int)p | 1;
        *(int *)((char *)p + 4)  = 0x38C7A0;
        gcDesiredEntityHelper_gcDesiredEntityHelper((char *)p + 0x20, 1, 0, 0);
        result = (gcEntityRelationship *)p;
    }
    return result;
}
