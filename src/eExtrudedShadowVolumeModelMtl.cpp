#include "eTextureMap.h"

#pragma control sched=1

// ── Forward class declarations ──

class eDrawInfo;

class eModelMtl {
public:
    void Write(cFile &) const;
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

// ── Write ──

void eExtrudedShadowVolumeModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eModelMtl *)this)->Write(file);
    wb.End();
}

// ── Constructor ──

extern "C" void eShadowVolumeModelMtl_eShadowVolumeModelMtl(void *, cBase *);

extern char eExtrudedShadowVolumeModelMtlclassdesc[];

eExtrudedShadowVolumeModelMtl::eExtrudedShadowVolumeModelMtl(cBase *parent) {
    eShadowVolumeModelMtl_eShadowVolumeModelMtl(this, parent);
    ((void **)this)[0x4 / 4] = eExtrudedShadowVolumeModelMtlclassdesc;
    CreateData();
}

// ── Destructor ──

eExtrudedShadowVolumeModelMtl::~eExtrudedShadowVolumeModelMtl() {
    *(void **)((char *)this + 4) = eExtrudedShadowVolumeModelMtlclassdesc;
}

// ── Read ──

int eExtrudedShadowVolumeModelMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && eShadowVolumeModelMtl::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    CreateData();
    return result;
}

// ── New ──

extern "C" void eExtrudedShadowVolumeModelMtl_eExtrudedShadowVolumeModelMtl(void *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eExtrudedShadowVolumeModelMtl *eExtrudedShadowVolumeModelMtl::New(cMemPool *pool, cBase *parent) {
    eExtrudedShadowVolumeModelMtl *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eExtrudedShadowVolumeModelMtl *obj = (eExtrudedShadowVolumeModelMtl *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        eExtrudedShadowVolumeModelMtl_eExtrudedShadowVolumeModelMtl(obj, parent);
        result = obj;
    }
    return result;
}

// ── AssignCopy ──

class eMaterial {
public:
    eMaterial &operator=(const eMaterial &);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT &operator=(const cHandleT &rhs) { mIndex = rhs.mIndex; return *this; }
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

struct cHandle {
    int mIndex;
};

void eExtrudedShadowVolumeModelMtl::AssignCopy(const cBase *src) {
    eExtrudedShadowVolumeModelMtl &other = *dcast<eExtrudedShadowVolumeModelMtl>(src);
    ((eMaterial *)this)->operator=(*(const eMaterial *)&other);
    ((unsigned char *)this)[0x5C] = ((unsigned char *)&other)[0x5C];
    ((unsigned char *)this)[0x5D] = ((unsigned char *)&other)[0x5D];
    ((unsigned char *)this)[0x5E] = ((unsigned char *)&other)[0x5E];
    ((signed char *)this)[0x5F] = ((signed char *)&other)[0x5F];
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x60));
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x64));
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x68) = *(cHandle *)((char *)&other + 0x68);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x6C) = *(cHandle *)((char *)&other + 0x6C);
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x70 / 4] = ((int *)&other)[0x70 / 4];
    ((unsigned char *)this)[0x74] = ((unsigned char *)&other)[0x74];
    ((unsigned char *)this)[0x75] = ((unsigned char *)&other)[0x75];
}
