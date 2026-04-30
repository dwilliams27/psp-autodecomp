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
class cFile;
class cMemPool;
class eGeomTemplate;
class cType;

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
    void Write(class cWriteBlock &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class eSky : public cObject {
public:
    eSky(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void eSky__eSky_cBaseptr(void *self, cBase *parent);
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B18;

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

// ── eSky::GetType(void) const @ 0x002049c4 ──
#pragma control sched=1
const cType *eSky::GetType(void) const {
    if (D_00046B18 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) =
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                   factory, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x204948;
        const char *kindName = (const char *)0x36CED4;
        const char *kindDesc = (const char *)0x36CEDC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory), "+r"(kindName),
                         "+r"(kindDesc));
        D_00046B18 = cType::InitializeType(0, 0, 0x1D9, parentType, factory,
                                           kindName, kindDesc, 0);
    }
    return D_00046B18;
}

// ── eSky::Write(cFile &) const @ 0x0005cc60 ──
#pragma control sched=1
void eSky::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    cObject::Write(file);

    int count0 = 0;
    int *handles = *(int **)((char *)this + 0x44);
    if (handles != 0) {
        count0 = handles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    handles = *(int **)((char *)this + 0x44);
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }
    int i = 0;
    int offset = 0;
    if (i < count) {
        int *handle = handles + offset;
        do {
            ((cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    ((const cBaseArray *)((const char *)this + 0x48))->Write(wb);
    wb.End();
}
#pragma control sched=2
