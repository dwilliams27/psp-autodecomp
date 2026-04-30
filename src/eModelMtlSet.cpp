// eModelMtlSet.cpp — decompiled from eAll_psp.obj
//   0x0005eac8  eModelMtlSet::Write(cFile &) const
//   0x0005eb14  eModelMtlSet::Read(cFile &, cMemPool *)
//   0x00205934  eModelMtlSet::AssignCopy(const cBase *)
//   0x00205980  eModelMtlSet::New(cMemPool *, cBase *) static
//
// eModelMtlSet inherits from eGeomMtlSet which inherits (eventually) from cObject.
// Vtables: 0x37FEA8 (eGeomMtlSet), 0x382468 (eModelMtlSet).

class cBase;
class cFile;
class cMemPool;
class cType;
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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    int Read(cFile &, cMemPool *);
};

class eModelMtlSet : public eGeomMtlSet {
public:
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

extern char eGeomMtlSetvirtualtable[];   // 0x37FEA8
extern char eModelMtlSetvirtualtable[];  // 0x382468
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF0;
extern cType *D_00040FFC;
extern cType *D_00046B2C;

extern "C" void cObject_cObject(void *self, cBase *parent);

#pragma control sched=1

// ── eModelMtlSet::Write(cFile &) const @ 0x0005EAC8 ──
void eModelMtlSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomMtlSet *)this)->Write(file);
    wb.End();
}

// ── eModelMtlSet::Read(cFile &, cMemPool *) @ 0x0005EB14 ──
int eModelMtlSet::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eGeomMtlSet *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
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

// ── eModelMtlSet::GetType(void) const @ 0x00205A18 ──
const cType *eModelMtlSet::GetType(void) const {
    if (D_00046B2C == 0) {
        if (D_00040FFC == 0) {
            if (D_00040FF0 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CDB8;
                const char *kindDesc = (const char *)0x36CDC8;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_00040FF0 = cType::InitializeType(0, 0, 0x23, parentType,
                                                   0, kindName, kindDesc, 5);
            }
            D_00040FFC = cType::InitializeType(0, 0, 0x24, D_00040FF0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00040FFC;
        cBase *(*factory)(cMemPool *, cBase *) = &eModelMtlSet::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B2C = cType::InitializeType(0, 0, 0x25, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B2C;
}
