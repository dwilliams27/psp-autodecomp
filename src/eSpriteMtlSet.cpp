// eSpriteMtlSet.cpp — decompiled from eAll_psp.obj
// eSpriteMtlSet inherits from eMaterialSet.

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

class eMaterialSet : public cObject {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eSpriteMtlSet : public eMaterialSet {
public:
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

extern char eMaterialSetvirtualtable[];   // 0x37FEA8
extern char eSpriteMtlSetvirtualtable[];  // 0x380410
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF0;
extern cType *D_0004103C;

extern "C" void cObject_cObject(void *self, cBase *parent);

// ── eSpriteMtlSet::Write(cFile &) const @ 0x00031AD0 ──
void eSpriteMtlSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eMaterialSet *)this)->Write(file);
    wb.End();
}

// ── eSpriteMtlSet::Read(cFile &, cMemPool *) @ 0x00031B1C ──
int eSpriteMtlSet::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((eMaterialSet *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eSpriteMtlSet::AssignCopy(const cBase *) @ 0x001E3EE8 ──
void eSpriteMtlSet::AssignCopy(const cBase *base) {
    eSpriteMtlSet *other = dcast<eSpriteMtlSet>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x44))->operator=(
        *(cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x44));
}

// ── eSpriteMtlSet::New(cMemPool *, cBase *) static @ 0x001E3F34 ──
cBase *eSpriteMtlSet::New(cMemPool *pool, cBase *parent) {
    eSpriteMtlSet *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eSpriteMtlSet *obj = (eSpriteMtlSet *)entry->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = eMaterialSetvirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        ((void **)obj)[1] = eSpriteMtlSetvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSpriteMtlSet::GetType(void) const @ 0x001E3FCC ──
const cType *eSpriteMtlSet::GetType(void) const {
    if (D_0004103C == 0) {
        if (D_00040FF0 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36CD74, (const char *)0x36CD7C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                                                       0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_00040FF0 = cType::InitializeType(
                0, 0, 0x23, D_000385E4, 0,
                (const char *)0x36CDB8, (const char *)0x36CDC8, 5);
        }
        D_0004103C = cType::InitializeType(
            0, 0, 0x29, D_00040FF0,
            (cBase *(*)(cMemPool *, cBase *))&eSpriteMtlSet::New, 0, 0, 0);
    }
    return D_0004103C;
}
