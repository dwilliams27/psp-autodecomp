#include "cBase.h"
#include "cFile.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class eLightGridNode;
class eLightGridSample;

template <class T>
class cArrayBase {
public:
    void *_data;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T>
class cArray : public cArrayBase<T> {
public:
    void Read(cReadBlock &);
};

class eLightGrid {
public:
    cBase *mOwner;
    void *mClassDesc;
    cArray<eLightGridNode> mNodes;
    cArray<eLightGridSample> mSamples;

    eLightGrid(cBase *);
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

template <class T> T *dcast(const cBase *);

extern char eLightGridclassdesc[];
extern const char eLightGrid_base_name[];
extern const char eLightGrid_base_desc[];

static cType *type_base;
static cType *type_eLightGrid;

// --- ctor ---
eLightGrid::eLightGrid(cBase *parent) {
    mOwner = parent;
    mClassDesc = eLightGridclassdesc;
    mNodes._data = 0;
    mSamples._data = 0;
}

void cFile_SetCurrentPos(void *, unsigned int);

// --- Read ---
int eLightGrid::Read(cFile &file, cMemPool *) {
    int result = 1;
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] != 2) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    mNodes.Read(rb);
    mSamples.Read(rb);
    return result;
}

// --- AssignCopy ---
#pragma control sched=1
void eLightGrid::AssignCopy(const cBase *src) {
    eLightGrid *rhs = dcast<eLightGrid>(src);
    cArrayBase<eLightGridNode> *srcNodes =
        (cArrayBase<eLightGridNode> *)((char *)rhs + 8);
    __asm__ volatile("" : "+r"(srcNodes));
    ((cArrayBase<eLightGridNode> *)((char *)this + 8))->operator=(*srcNodes);
    ((cArrayBase<eLightGridSample> *)((char *)this + 0xC))->operator=(
        *(cArrayBase<eLightGridSample> *)((char *)rhs + 0xC));
}
#pragma control sched=2

// --- New ---
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *eLightGrid::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eLightGrid *result = 0;
    eLightGrid *obj = (eLightGrid *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) eLightGrid(parent);
        result = obj;
    }
    return (cBase *)result;
}

// --- GetType ---
#pragma control sched=1
const cType *eLightGrid::GetType(void) const {
    if (!type_eLightGrid) {
        if (!type_base) {
            const char *name = eLightGrid_base_name;
            const char *desc = eLightGrid_base_desc;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            type_base = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *base = type_base;
        __asm__ volatile("" : "+r"(base));
        cBase *(*factory)(cMemPool *, cBase *) = &eLightGrid::New;
        __asm__ volatile("" : "+r"(factory));
        type_eLightGrid = cType::InitializeType(0, 0, 0x1BA, base, factory, 0, 0, 0);
    }
    return type_eLightGrid;
}
#pragma control sched=2
