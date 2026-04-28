// eSky — eAll_psp.obj
// Decompiled functions:
//   0x002048d8  eSky::AssignCopy(const cBase *)             (112B)
//   0x00204948  eSky::New(cMemPool *, cBase *) static       (124B)
//
// eSky inherits from cObject. Layout:
//   [0x00..0x44) cObject base
//   [0x44..0x48) cArrayBase<cHandleT<eGeomTemplate> >
//   [0x48..0x50) cBaseArray
//   [0x50..0x58) cBaseArray
//   [0x58..0x60) cBaseArray
// sizeof(eSky) = 0x60

class cBase;
class cMemPool;
class eGeomTemplate;

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

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    cBaseArray &operator=(const cBaseArray &);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class eSky : public cObject {
public:
    eSky(cBase *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void eSky__eSky_cBaseptr(void *self, cBase *parent);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── eSky::AssignCopy(const cBase *) @ 0x002048d8 ──
#pragma control sched=1
void eSky::AssignCopy(const cBase *base) {
    eSky *other = dcast<eSky>(base);
    cObject::operator=(*other);
    ((cArrayBase<cHandleT<eGeomTemplate> > *)((char *)this + 0x44))->operator=(
        *(cArrayBase<cHandleT<eGeomTemplate> > *)((char *)other + 0x44));
    ((cBaseArray *)((char *)this + 0x48))->operator=(
        *(cBaseArray *)((char *)other + 0x48));
    ((cBaseArray *)((char *)this + 0x50))->operator=(
        *(cBaseArray *)((char *)other + 0x50));
    ((cBaseArray *)((char *)this + 0x58))->operator=(
        *(cBaseArray *)((char *)other + 0x58));
}
#pragma control sched=2

// ── eSky::New(cMemPool *, cBase *) static @ 0x00204948 ──
#pragma control sched=1
cBase *eSky::New(cMemPool *pool, cBase *parent) {
    eSky *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSky *obj = (eSky *)entry->fn(base, 0x60, 4, 0, 0);
    if (obj != 0) {
        eSky__eSky_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
