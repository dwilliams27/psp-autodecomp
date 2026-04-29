// gcEnumerationEntry.cpp — decompiled from gcAll_psp.obj.
// Parent: cNamed. cNamed parent's classdesc lives at 0x37E6A8 (cBaseclassdesc).
//
// Functions:
//   0x000d3d6c gcEnumerationEntry::Write(cFile &) const                100B
//   0x00238b14 gcEnumerationEntry::New(cMemPool *, cBase *) static     168B
//   0x00238bbc gcEnumerationEntry::GetType(void) const                 228B
//   0x00238a9c gcEnumerationEntry::AssignCopy(const cBase *)           120B
//   0x00238ca0 gcEnumerationEntry::~gcEnumerationEntry(void)           100B

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(unsigned char);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct cNameData { int _w[6]; };  // 24-byte name buffer at cNamed+8

class cNamed {
public:
    char _pad[8];          // cBase header: parent (0), classdesc (4)
    cNameData mName;       // 8..0x20
    cNamed(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

struct cHandle {
    int mIndex;
};

extern char cBaseclassdesc[];   // @ 0x37E6A8
extern char cNamedclassdesc[];

template <class T> T *dcast(const cBase *);

class gcEnumerationEntry : public cNamed {
public:
    unsigned char mField20;  // 0x20
    unsigned int  mField24;  // 0x24

    gcEnumerationEntry(cBase *);
    ~gcEnumerationEntry(void);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

static cType *type_cBase;
static cType *type_cNamed;
static cType *type_gcEnumerationEntry;

// ── gcEnumerationEntry::New @ 0x00238b14 ──
cBase *gcEnumerationEntry::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEnumerationEntry *result = 0;
    gcEnumerationEntry *obj = (gcEnumerationEntry *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = cNamedclassdesc;
        *(short *)((char *)obj + 0x1C) = 0;
        *(short *)((char *)obj + 0x1E) = 0;
        *(char *)((char *)obj + 8) = 0;
        *(void **)((char *)obj + 4) = (void *)0x386B40;
        *(unsigned char *)((char *)obj + 0x20) = 1;
        *(int *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcEnumerationEntry::GetType @ 0x00238bbc ──
const cType *gcEnumerationEntry::GetType(void) const {
    if (!type_gcEnumerationEntry) {
        if (!type_cNamed) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            type_cNamed = cType::InitializeType(0, 0, 2,
                                                type_cBase,
                                                cNamed::New,
                                                0, 0, 0);
        }
        type_gcEnumerationEntry = cType::InitializeType(0, 0, 0xAA,
                                                        type_cNamed,
                                                        gcEnumerationEntry::New,
                                                        0, 0, 0);
    }
    return type_gcEnumerationEntry;
}

// ── gcEnumerationEntry::Write @ 0x000d3d6c ──
void gcEnumerationEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cNamed::Write(file);
    wb.Write(mField24);
    wb.Write(mField20);
    wb.End();
}

// ── gcEnumerationEntry::AssignCopy @ 0x00238a9c ──
void gcEnumerationEntry::AssignCopy(const cBase *base) {
    gcEnumerationEntry *other = dcast<gcEnumerationEntry>(base);
    cNameData *src = (cNameData *)((char *)other + 8);
    cNameData *dst = (cNameData *)((char *)this + 8);
    *dst = *src;
    *(unsigned char *)((char *)this + 0x20) = *(const unsigned char *)((char *)other + 0x20);
    *(cHandle *)((char *)this + 0x24) = *(const cHandle *)((char *)other + 0x24);
}

// ── gcEnumerationEntry::~gcEnumerationEntry @ 0x00238ca0 ──
// SNC auto-emits the (this!=0) guard and the (flag & 1) deleting tail
// dispatch through operator delete. The body just restores the parent
// (cNamed→cBase) classdesc at offset 4.
gcEnumerationEntry::~gcEnumerationEntry(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

extern char gcValLobbyScoreboardInfovirtualtable[];

class gcValLobbyScoreboardInfo {
public:
    cBase *mParent;
    void *mVtable;
    int mField8;
    int mFieldC;
    int mDesired10;
    int mDesired14;
    int mDesired18;
    int mDesired1C;
    int mDesired20;
    int mDesired24;

    static cBase *New(cMemPool *, cBase *);
};

// ── gcValLobbyScoreboardInfo::New @ 0x0034b0d0 ──
cBase *gcValLobbyScoreboardInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyScoreboardInfo *result = 0;
    gcValLobbyScoreboardInfo *obj =
        (gcValLobbyScoreboardInfo *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyScoreboardInfovirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = 0;
        *(int *)((char *)obj + 0x10) = tagged;
        *(int *)((char *)obj + 0x14) = tagged;
        *(int *)((char *)obj + 0x18) = tagged;
        *(int *)((char *)obj + 0x1C) = tagged;
        *(int *)((char *)obj + 0x20) = tagged;
        *(int *)((char *)obj + 0x24) = tagged;
        result = obj;
    }
    return (cBase *)result;
}
