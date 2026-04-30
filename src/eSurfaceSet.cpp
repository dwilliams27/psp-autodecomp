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
class cType;
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
    void Write(cFile &) const;
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class eSurfaceSet : public cObject {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

extern char eSurfaceSetvirtualtable[];   // 0x381D20
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A1C;

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

// ── eSurfaceSet::GetType(void) const @ 0x001F66C0 ──
const cType *eSurfaceSet::GetType(void) const {
    if (D_00046A1C == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eSurfaceSet::New;
        const char *kindName = (const char *)0x36CE68;
        const char *kindDesc = (const char *)0x36CE74;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory), "+r"(kindName),
                         "+r"(kindDesc));
        D_00046A1C = cType::InitializeType(0, 0, 0x3B, parentType, factory,
                                           kindName, kindDesc, 5);
    }
    return D_00046A1C;
}

// ── eSurfaceSet::Write(cFile &) const @ 0x00053C3C ──
void eSurfaceSet::Write(cFile &file) const {
    int var_a1;
    int var_s0;
    int var_s1;
    cHandle *temp_a0;
    cHandle *temp_a0_2;
    int var_s2;

    cWriteBlock wb(file, 1);
    cObject::Write(file);

    temp_a0 = *(cHandle **)((char *)this + 0x44);
    var_a1 = 0;
    if (temp_a0 != 0) {
        var_a1 = ((int *)temp_a0)[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_a1);

    var_s1 = 0;
    temp_a0_2 = *(cHandle **)((char *)this + 0x44);
    var_s0 = 0;
    if (temp_a0_2 != 0) {
        var_s0 = ((int *)temp_a0_2)[-1] & 0x3FFFFFFF;
    }
    if (var_s1 < var_s0) {
        var_s2 = 0;
        var_s2 = (int)temp_a0_2 + var_s2;
        do {
            ((cHandle *)var_s2)->Write(wb);
            var_s1 += 1;
            var_s2 += 4;
        } while (var_s1 < var_s0);
    }

    wb.End();
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
