// gcRegionSet — small data class. Direct subclass of cBase.
// Layout: parent(0), classdesc(4), 2x cGUID at 0x08 / 0x10, uint at 0x18.
//
// Functions matched here:
//   gcRegionSet::Write(cFile &) const     @ 0x000ef7e0 (128B)
//   gcRegionSet::AssignCopy(const cBase *) @ 0x00245bf8 (100B)
//   gcRegionSet::New(cMemPool *, cBase *) static @ 0x00245c5c (180B)
//   gcRegionSet::GetType(void) const      @ 0x00245d10 (160B)
//   gcRegionSet::~gcRegionSet(void)       @ 0x0024602c (100B)

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class cGUID {
public:
    int m0;
    int m4;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

extern char cBaseclassdesc[];                  // @ 0x37E6A8
extern const char gcRegionSet_base_name[];     // @ 0x36D894
extern const char gcRegionSet_base_desc[];     // @ 0x36D89C

unsigned int cIRand(void);
extern "C" void *__vec_new(void *, int, int, void (*)(void *));

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcRegionSetPoolNS {
public:
    static gcRegionSetPoolNS *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcRegionSet {
public:
    cBase *m_parent;
    void *m_classdesc;
    cGUID mRegions[2];
    unsigned int mId;

    ~gcRegionSet();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcRegionSetPoolNS *pool = gcRegionSetPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

gcRegionSet *dcast(const cBase *);

// ── Write ── @ 0x000ef7e0, 128B
void gcRegionSet::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    wb.Write(*(const unsigned int *)((const char *)this + 0x18));
    wb.Write((int)2);
    const cGUID *base = (const cGUID *)((const char *)this + 8);
    int off;
    __asm__ volatile("ori %0,$0,0" : "=r"(off));
    int i = 0;
    const cGUID *guid = (const cGUID *)((const char *)base + off);
    do {
        guid->Write(wb);
        i++;
        guid++;
    } while (i < 2);
    wb.End();
}

// ── Read ── @ 0x000ef860, 268B
int gcRegionSet::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0,$0,1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    cFile *rbFile = (cFile *)rb._data[0];
    register int version asm("a0") = 2;
    if ((unsigned int)rb._data[3] != (unsigned int)version)
        goto fail;
    int countTemp;
    countTemp = version;
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x18, 4);
    cFileSystem::Read(*(void **)rb._data[0], &countTemp, 4);
    int count = countTemp;
    int i = 0;
    if (i < count)
        goto read_guid;
    goto done;
fail:
    rbFile->SetCurrentPos(rb._data[1]);
    return 0;
read_guid:
    {
        cGUID *base = (cGUID *)((char *)this + 8);
        int off;
        __asm__ volatile("ori %0,$0,0" : "=r"(off));
        cGUID *guid = (cGUID *)((char *)base + off);
        do {
            guid->Read(rb);
            i++;
            guid++;
        } while (i < count);
    }
done:
    return result;
}

// ── AssignCopy ── @ 0x00245bf8, 100B
void gcRegionSet::AssignCopy(const cBase *base) {
    gcRegionSet *other = dcast(base);
    int i = 0;
    cGUID *dst = mRegions;
    cGUID *src = other->mRegions;
    do {
        *dst = *src;
        i++;
        dst++;
        src++;
    } while (i < 2);
    mId = other->mId;
}

// ── New ── @ 0x00245c5c, 180B
cBase *gcRegionSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcRegionSet *result = 0;
    gcRegionSet *obj = (gcRegionSet *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        obj->m_parent = parent;
        *(int *)((char *)obj + 4) = 0x3879F8;
        __vec_new((char *)obj + 8, 2, 8, (void (*)(void *))0x245578);
        obj->mId = cIRand() | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ── @ 0x00245d10, 160B
static cType *type_base;
static cType *type_gcRegionSet;

const cType *gcRegionSet::GetType(void) const {
    if (!type_gcRegionSet) {
        if (!type_base) {
            type_base = cType::InitializeType(gcRegionSet_base_name,
                                              gcRegionSet_base_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_gcRegionSet = cType::InitializeType(0, 0, 0xFE, type_base,
                                                 &gcRegionSet::New,
                                                 0, 0, 0);
    }
    return type_gcRegionSet;
}

// ── ~gcRegionSet ── @ 0x0024602c, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard,
// the absence of a parent-destructor chain (cBase has none), and the
// deleting-tail dispatch through operator delete on (flag & 1).
gcRegionSet::~gcRegionSet() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
