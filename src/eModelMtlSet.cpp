// eModelMtlSet.cpp — decompiled from eAll_psp.obj
//   0x0005eac8  eModelMtlSet::Write(cFile &) const
//   0x00205934  eModelMtlSet::AssignCopy(const cBase *)
//   0x00205980  eModelMtlSet::New(cMemPool *, cBase *) static
//
// eModelMtlSet inherits from eGeomMtlSet which inherits (eventually) from cObject.
// Vtables: 0x37FEA8 (eGeomMtlSet), 0x382468 (eModelMtlSet).

class cBase;
class cFile;
class cMemPool;
class eMaterial;

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

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

class eGeomMtlSet : public cObject {
public:
    void Write(cFile &) const;
};

class eModelMtlSet : public eGeomMtlSet {
public:
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

extern char eGeomMtlSetvirtualtable[];   // 0x37FEA8
extern char eModelMtlSetvirtualtable[];  // 0x382468

extern "C" void cObject_cObject(void *self, cBase *parent);

#pragma control sched=1

// ── eModelMtlSet::Write(cFile &) const @ 0x0005EAC8 ──
void eModelMtlSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomMtlSet *)this)->Write(file);
    wb.End();
}

// ── eModelMtlSet::AssignCopy(const cBase *) @ 0x00205934 ──
void eModelMtlSet::AssignCopy(const cBase *base) {
    eModelMtlSet *other = dcast<eModelMtlSet>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x44))->operator=(
        *(cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x44));
}

// ── eModelMtlSet::New(cMemPool *, cBase *) static @ 0x00205980 ──
cBase *eModelMtlSet::New(cMemPool *pool, cBase *parent) {
    eModelMtlSet *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eModelMtlSet *obj = (eModelMtlSet *)entry->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = eGeomMtlSetvirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        ((void **)obj)[1] = eModelMtlSetvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}
