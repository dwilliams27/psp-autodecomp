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

template <class T> T *dcast(const cBase *);

extern char eFluidVolumeclassdesc[];
extern const char eFluidVolume_base_name[];
extern const char eFluidVolume_base_desc[];

static cType *type_base;
static cType *type_eFluidVolume;

// Constructor (0x0003c5dc)
eFluidVolume::eFluidVolume(cBase *parent) {
    mOwner = parent;
    mClassDesc = eFluidVolumeclassdesc;
    mRef = 0;
    mField_C = 1.0f;
    mField_10 = 0.0f;
}

// AssignCopy (0x001e8d54)
void eFluidVolume::AssignCopy(const cBase *src) {
    eFluidVolume *other = dcast<eFluidVolume>(src);
    mRef = other->mRef;
    mField_C = other->mField_C;
    mField_10 = other->mField_10;
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
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

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
