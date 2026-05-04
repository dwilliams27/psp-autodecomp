#include "eFluidVolume.h"
#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class eBspTree {
public:
    eBspTree(cBase *);
    void ReadOld(cReadBlock &);
};

template <class T> T *dcast(const cBase *);

extern char eFluidVolumeclassdesc[];
extern char cBaseclassdesc[];
extern const char eFluidVolume_base_name[];
extern const char eFluidVolume_base_desc[];

static cType *type_base;
static cType *type_eFluidVolume;

struct DtorDispatchEntry {
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

// Constructor (0x0003c5dc)
eFluidVolume::eFluidVolume(cBase *parent) {
    mOwner = parent;
    mClassDesc = eFluidVolumeclassdesc;
    mRef = 0;
    mField_C = 1.0f;
    mField_10 = 0.0f;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ~eFluidVolume (0x0003c60c)
// SNC ABI auto-generates the (this != 0) guard and the deleting-tail through
// operator delete (defined inline in the header). The 8-byte `b .; nop` tail
// at the end of the symbol comes from the two file-scope `.word` directives.
eFluidVolume::~eFluidVolume() {
    mClassDesc = eFluidVolumeclassdesc;
    if (mRef != 0) {
        char *type = ((char **)mRef)[1];
        DtorDispatchEntry *rec = (DtorDispatchEntry *)(type + 0x50);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn((char *)mRef + off, (void *)3);
        mRef = 0;
    }
    mClassDesc = cBaseclassdesc;
}

// AssignCopy (0x001e8d54)
void eFluidVolume::AssignCopy(const cBase *src) {
    eFluidVolume *other = dcast<eFluidVolume>(src);
    mRef = other->mRef;
    mField_C = other->mField_C;
    mField_10 = other->mField_10;
}

// ReadOld (0x0003c6b8)
void eFluidVolume::ReadOld(cReadBlock &arg_rb) {
    cReadBlock rb(*reinterpret_cast<cFile *>(arg_rb._data[0]), 3, true);
    float dummy;
    eBspTree *result = 0;
    if (static_cast<unsigned int>(rb._data[3]) < 3U) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        void *block = ((void **)pool)[9];
        char *allocTable = ((PoolBlock *)block)->allocTable;
        AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
        short off = entry->offset;
        void *base = (char *)block + off;
        eBspTree *obj = (eBspTree *)entry->fn(base, 0x14, 4, 0, 0);
        if (obj != 0) {
            new (obj) eBspTree((cBase *)this);
            result = obj;
        }
        mRef = (cBase *)result;
        result->ReadOld(rb);
    } else {
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        cBase *temp = mRef;
        rb.ReadBase(pool, (cBase *)this, temp);
        mRef = temp;
    }
    cFileSystem::Read(*(void **)rb._data[0], &mField_C, 4);
    cFileSystem::Read(*(void **)rb._data[0], &mField_10, 4);
    if (rb._data[3] == 1) {
        cFileSystem::Read(*(void **)rb._data[0], &dummy, 4);
    }
}

// Write (0x0003c4a0)
void eFluidVolume::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.WriteBase(mRef);
    wb.Write(mField_C);
    wb.Write(mField_10);
    wb.End();
}

// New (0x001e8d94)
cBase *eFluidVolume::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eFluidVolume *result = 0;
    eFluidVolume *obj = (eFluidVolume *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) eFluidVolume(parent);
        result = obj;
    }
    return (cBase *)result;
}

// GetType (0x001e8e10)
const cType *eFluidVolume::GetType(void) const {
    if (!type_eFluidVolume) {
        if (!type_base) {
            type_base = cType::InitializeType(eFluidVolume_base_name, eFluidVolume_base_desc, 1, 0, 0, 0, 0, 0);
        }
        type_eFluidVolume = cType::InitializeType(0, 0, 0x25B, type_base, &eFluidVolume::New, 0, 0, 0);
    }
    return type_eFluidVolume;
}
