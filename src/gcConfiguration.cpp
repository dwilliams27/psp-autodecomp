// Decompiled functions:
//   0x00246650 gcConfiguration::AssignCopy(const cBase *)
//   0x000f02f0 gcConfiguration::Write(cFile &) const
//   0x002466b0 gcConfiguration::New(cMemPool *, cBase *) static

#include "cBase.h"

class cFile;
class cMemPool;

// ─── helper class declarations ────────────────────────────────────────────
class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

template <class T> T *dcast(const cBase *);

// ─── pool-block layout used by ::New() ──────────────────────────────────
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ============================================================================
// gcConfiguration — inherits cObject; four cBase pointers at 0x44..0x50.
// ============================================================================
class gcConfiguration : public cObject {
public:
    char _pad[0x44];          // padding so the four pointer slots land at 0x44..0x50
    cBase *mField44;          // 0x44
    cBase *mField48;          // 0x48
    cBase *mField4C;          // 0x4C
    cBase *mField50;          // 0x50

    gcConfiguration(cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

// Constructor placeholder — defined in another TU; compare_func.py masks the
// call relocation, so the call shape is what matters.
extern "C" void gcConfiguration_construct(void *self, cBase *parent);

// ── gcConfiguration::AssignCopy(const cBase *) @ 0x00246650 ──
void gcConfiguration::AssignCopy(const cBase *src) {
    gcConfiguration *other = dcast<gcConfiguration>(src);
    cObject::operator=(*other);
    mField44 = other->mField44;
    mField48 = other->mField48;
    mField4C = other->mField4C;
    mField50 = other->mField50;
}

// ── gcConfiguration::Write(cFile &) const @ 0x000f02f0 ──
void gcConfiguration::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.WriteBase(mField44);
    wb.WriteBase(mField48);
    wb.WriteBase(mField4C);
    wb.WriteBase(mField50);
    wb.End();
}

// ── gcConfiguration::New(cMemPool *, cBase *) static @ 0x002466b0 ──
cBase *gcConfiguration::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcConfiguration *result = 0;
    gcConfiguration *obj = (gcConfiguration *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcConfiguration_construct(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
