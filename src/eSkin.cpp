// eSkin.cpp - decompiled from eAll_psp.obj.
//
// Functions:
//   0x001f1724 eSkin::New(cMemPool *, cBase *) static  144B

class cBase;
class cMemPool;
class cType;
class eMaterialSet;
class eSurfaceSet;

class cObject {
public:
    cObject &operator=(const cObject &);
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

template <class T>
T dcast(const cBase *);

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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eSkin {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void cObject_cObject(void *self, cBase *parent);

extern char eSkinclassdesc[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469F8;

#pragma control sched=1
// -- eSkin::AssignCopy @ 0x001f16cc --
void eSkin::AssignCopy(const cBase *base) {
    eSkin *src = dcast<eSkin *>(base);
    ((cObject *)this)->operator=(*(cObject *)src);
    ((cArrayBase<cHandleT<eMaterialSet> > *)((char *)this + 0x44))->operator=(
        *(cArrayBase<cHandleT<eMaterialSet> > *)((char *)src + 0x44));
    ((cArrayBase<cHandleT<eSurfaceSet> > *)((char *)this + 0x48))->operator=(
        *(cArrayBase<cHandleT<eSurfaceSet> > *)((char *)src + 0x48));
}

// -- eSkin::New @ 0x001f1724 --
cBase *eSkin::New(cMemPool *pool, cBase *parent) {
    eSkin *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSkin *obj = (eSkin *)entry->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = eSkinclassdesc;
        *(int *)((char *)obj + 0x44) = 0;
        *(int *)((char *)obj + 0x48) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// -- eSkin::GetType(void) const @ 0x001f17b4 --
const cType *eSkin::GetType(void) const {
    if (D_000469F8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) = &eSkin::New;
        const char *kindName = (const char *)0x36CE5C;
        const char *kindDesc = (const char *)0x36CE64;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory), "+r"(kindName),
                         "+r"(kindDesc));
        D_000469F8 = cType::InitializeType(0, 0, 0x45, parentType, factory,
                                           kindName, kindDesc, 5);
    }
    return D_000469F8;
}
#pragma control sched=2
