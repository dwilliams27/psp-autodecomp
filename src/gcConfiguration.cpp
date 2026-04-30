// Decompiled functions:
//   0x000f04d8 gcConfiguration::gcConfiguration(cBase *)
//   0x00246650 gcConfiguration::AssignCopy(const cBase *)
//   0x000f02f0 gcConfiguration::Write(cFile &) const
//   0x002466b0 gcConfiguration::New(cMemPool *, cBase *) static
//   0x0024672c gcConfiguration::GetType(void) const

#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;

class cName {
public:
    void Set(const char *, ...);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject : public cBase {
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

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *);
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

class cType {
public:
    char _pad[0x10];
    cBase *(*mCreate)(cMemPool *, cBase *);

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    char _pad[0x24];
    void *mBlock;

    static cMemPool *GetPoolFromPtr(const void *);
};

class gcConfig : public cBase {
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00099AF8;

extern "C" int cGetCurrentPlatform(void);
extern "C" void gcConfig__gcConfig_cBaseptr(void *self, cBase *parent);

class gcConfiguration : public cObject {
public:
    char _pad[0x44];
    cBase *mField44;
    cBase *mField48;
    cBase *mField4C;
    gcConfig *mField50;

    gcConfiguration(cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    cType *GetCoreConfigType(void);
    cType *GetEngineConfigType(void);
    cType *GetNetworkConfigType(void);
    static cBase *New(cMemPool *, cBase *);
};

gcConfiguration::gcConfiguration(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x387B50;
    *(gcConfiguration **)0x37D7E4 = this;

    int platform = cGetCurrentPlatform();
    const char *name = 0;
    switch (platform) {
    case 0:
        name = (const char *)0x36D868;
        break;
    case 1:
        name = (const char *)0x36D86C;
        break;
    case 2:
        name = (const char *)0x36D870;
        break;
    case 3:
        name = (const char *)0x36D878;
        break;
    case 4:
        name = (const char *)0x36D87C;
        break;
    case 5:
        name = (const char *)0x36D880;
        break;
    case 6:
        name = (const char *)0x36D884;
        break;
    case 7:
        name = (const char *)0x36D888;
        break;
    case 8:
        name = (const char *)0x36D890;
        goto set_name;
    default:
        name = 0;
        break;
    }

set_name:
    ((cName *)((char *)this + 8))->Set(name);
    DispatchEntry *dispatch =
        (DispatchEntry *)(*(char **)((char *)this + 4) + 0x70);
    dispatch->fn((char *)this + dispatch->offset);

    int *state = (int *)((char *)this + 0x20);
    int one = 1;
    state[0] = one;
    state[1] = one;

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);

    cType *coreType = GetCoreConfigType();
    mField44 = coreType->mCreate(pool, this);

    cType *engineType = GetEngineConfigType();
    mField48 = engineType->mCreate(pool, this);

    cType *networkType = GetNetworkConfigType();
    mField4C = networkType->mCreate(pool, this);

    void *block = pool->mBlock;
    PoolBlock *poolBlock = (PoolBlock *)block;
    gcConfig *result = 0;
    gcConfig *obj =
        (gcConfig *)((AllocEntry *)(poolBlock->allocTable + 0x28))->fn(
            (char *)block + ((AllocEntry *)(poolBlock->allocTable + 0x28))->offset,
            0x78, 4, 0, 0);
    if (obj != 0) {
        gcConfig__gcConfig_cBaseptr(obj, this);
        result = obj;
    }
    mField50 = result;
}

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
        new (obj) gcConfiguration(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcConfiguration::GetType(void) const @ 0x0024672c ──
const cType *gcConfiguration::GetType(void) const {
    if (D_00099AF8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00099AF8 = cType::InitializeType(0, 0, 0x1B8, D_000385E4,
                                           &gcConfiguration::New,
                                           (const char *)0x36D910,
                                           (const char *)0x36D920, 1);
    }
    return D_00099AF8;
}
