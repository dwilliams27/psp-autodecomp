// eSurfaceSet.cpp — decompiled from eAll_psp.obj
//   0x00053d04  eSurfaceSet::Read(cFile &, cMemPool *)
//   0x001f65e8  eSurfaceSet::AssignCopy(const cBase *)
//   0x001f6634  eSurfaceSet::New(cMemPool *, cBase *) static
//
// eSurfaceSet inherits directly from cObject.
// Layout: [0..0x44) = cObject base, [0x44..0x48) = cArrayBase<cHandleT<eSurface>>.
// Vtable address: 0x381D20.

#include "cBase.h"

class cFile;
class cMemPool;
class eSurface;

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T>
class cArray {
public:
    void Read(class cReadBlock &);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    int Read(cFile &, cMemPool *);
};

class eSurfaceSet : public cObject {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
};

extern char eSurfaceSetvirtualtable[];   // 0x381D20

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" void cObject_cObject(void *self, cBase *parent);

#pragma control sched=1

// ── eSurfaceSet::AssignCopy(const cBase *) @ 0x001F65E8 ──
void eSurfaceSet::AssignCopy(const cBase *base) {
    eSurfaceSet *other = dcast<eSurfaceSet>(base);
    cObject::operator=(*other);
    ((cArrayBase<cHandleT<eSurface> > *)((char *)this + 0x44))->operator=(
        *(cArrayBase<cHandleT<eSurface> > *)((char *)other + 0x44));
}

// ── eSurfaceSet::New(cMemPool *, cBase *) static @ 0x001F6634 ──
cBase *eSurfaceSet::New(cMemPool *pool, cBase *parent) {
    eSurfaceSet *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSurfaceSet *obj = (eSurfaceSet *)entry->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = eSurfaceSetvirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSurfaceSet::Read(cFile &, cMemPool *) @ 0x00053D04 ──
int eSurfaceSet::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && cObject::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((cArray<cHandleT<eSurface> > *)((char *)this + 0x44))->Read(rb);
    return result;
}
