// eSurfacePropertyTable - eAll_psp.obj
//   0x001f6f28 AssignCopy(const cBase *)

#include "cBase.h"
#include "cObject.h"

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

class cFile;
class cFileHandle;
class cMemPool;

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

template <class T> T *dcast(const cBase *);

class cBaseArray {
public:
    void *mData;
    void *mOwner;

    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

template <class T>
class cArrayBase {
public:
    void *mData;

    cArrayBase &operator=(const cArrayBase &);
    bool SetSize(int, const cArrayBase &);
};

template <class T>
class cArray : public cArrayBase<T> {
public:
    void Read(cReadBlock &);
};

class eSurfacePropertyPair;

class eSurfacePropertyTable : public cObject {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class cName {
public:
    void Read(cReadBlock &);
};

class gcValue {
public:
    int Read(cFile &, cMemPool *);
};

class gcValNavMeshDirection : public gcValue {
public:
    int Read(cFile &, cMemPool *);
};

struct ReadEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A20;

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" int __0fHcObjectEReadR6FcFileP6IcMemPool(cObject *, cFile &,
                                                    cMemPool *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                             bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

#pragma control sched=1
void eSurfacePropertyTable::AssignCopy(const cBase *base) {
    eSurfacePropertyTable *other = dcast<eSurfacePropertyTable>(base);
    cObject::operator=(*other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));
    ((cArrayBase<eSurfacePropertyPair> *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<eSurfacePropertyPair> *)((char *)other + 0x4C));
}

const cType *eSurfacePropertyTable::GetType(void) const {
    if (D_00046A20 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0,
                                                       0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0, 0, 0, 0,
                                               0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) = &eSurfacePropertyTable::New;
        const char *kindName = (const char *)0x36CE78;
        const char *kindDesc = (const char *)0x36CE90;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory), "+r"(kindName),
                         "+r"(kindDesc));
        D_00046A20 = cType::InitializeType(0, 0, 0x2B8, parentType, factory,
                                           kindName, kindDesc, 5);
    }
    return D_00046A20;
}

#pragma control sched=1

void eSurfacePropertyTable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cObject *)this)->Write(file);
    ((const cBaseArray *)((const char *)this + 0x44))->Write(wb);

    int *temp_a0 = (int *)((cArrayBase<int *> *)((char *)this + 0x4C))->mData;
    int var_s1 = 0;
    if (temp_a0 != 0) {
        var_s1 = temp_a0[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_s1);

    int *temp_s0 = (int *)((cArrayBase<int *> *)((char *)this + 0x4C))->mData;
    int var_s1_2 = 0;
    if (temp_s0 != 0) {
        var_s1_2 = temp_s0[-1] & 0x3FFFFFFF;
    }

    int var_s2 = 0;
    if (var_s2 < var_s1_2) {
        int *var_s3_base = temp_s0;
        int var_s3_offset = 0;
        __asm__ volatile("" : "+r"(var_s3_base), "+r"(var_s3_offset));
        int *var_s3 = var_s3_base + var_s3_offset;
        do {
            int *table0 = var_s3 + 0;
            __asm__ volatile("" : "+r"(table0));
            int *temp_a1 = *(int **)table0;
            int var_a0 = 0;
            if (temp_a1 != 0) {
                var_a0 = temp_a1[-1] & 0x3FFFFFFF;
            }
            int *table1 = table0 + 0;
            __asm__ volatile("" : "+r"(table1));
            wb.Write(var_a0);

            int *temp_a0_2 = *(int **)table1;
            int var_s4 = 0;
            if (temp_a0_2 != 0) {
                var_s4 = temp_a0_2[-1] & 0x3FFFFFFF;
            }

            int var_s5 = 0;
            if (var_s5 < var_s4) {
                float *var_s6_base = (float *)temp_a0_2;
                int var_s6_offset = 0;
                __asm__ volatile("" : "+r"(var_s6_base), "+r"(var_s6_offset));
                float *var_s6 =
                    (float *)((char *)var_s6_base + var_s6_offset);
                do {
                    wb.Write(var_s6[0]);
                    wb.Write(var_s6[1]);
                    wb.Write(var_s6[2]);
                    wb.Write(var_s6[3]);
                    var_s5 += 1;
                    var_s6 = (float *)((char *)var_s6 + 0x10);
                } while (var_s5 < var_s4);
            }

            var_s2 += 1;
            var_s3 += 1;
        } while (var_s2 < var_s1_2);
    }

    wb.End();
}

int eSurfacePropertyTable::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$18");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);

    unsigned int version = rb._data[3];
    if (version >= 3) goto fail;
    if (version < 1) goto fail;
    if (__0fHcObjectEReadR6FcFileP6IcMemPool((cObject *)this, file, pool))
        goto success;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    ((cBaseArray *)((char *)this + 0x44))->Read(rb);

    cArrayBase<cArray<eSurfacePropertyPair> > old;
    int count;
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &count, 4);
    }

    register cArrayBase<cArray<eSurfacePropertyPair> > *tables __asm__("$4") =
        (cArrayBase<cArray<eSurfacePropertyPair> > *)((char *)this + 0x4C);
    register int size __asm__("$5") = count;
    __asm__ volatile("" : "+r"(tables), "+r"(size));
    ((volatile cArrayBase<cArray<eSurfacePropertyPair> > *)&old)->mData = 0;
    ((volatile cArrayBase<cArray<eSurfacePropertyPair> > *)&old)->mData = 0;
    tables->SetSize(size, old);

    void *data = old.mData;
    int oldCount = 0;
    if (data != 0) {
        oldCount = ((int *)data)[-1] & 0x3FFFFFFF;
    }

    int index = 0;
    int inRange = index < oldCount;
    if (inRange != 0) {
        do {
            index += 1;
            inRange = index < oldCount;
        } while (inRange != 0);
    }

    if (data != 0) {
        data = (char *)data - 4;
        if (data != 0) {
            cMemPool *oldPool = cMemPool::GetPoolFromPtr(data);
            char *block = ((char **)oldPool)[9];
            char *allocTable = ((PoolBlock *)block)->allocTable;
            register DeleteEntry *rec __asm__("$5") =
                (DeleteEntry *)(allocTable + 0x30);
            __asm__ volatile("" : "+r"(rec));
            short off = rec->offset;
            void *target = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(target, data);
        }
        old.mData = 0;
    }

    int total = count;
    int row = 0;
    cArray<eSurfacePropertyPair> *pairs =
        *(cArray<eSurfacePropertyPair> **)((char *)this + 0x4C);
    if (row < total) {
        register int offset __asm__("$4");
        __asm__ volatile("ori %0, $0, 0" : "=r"(offset));
        pairs = (cArray<eSurfacePropertyPair> *)((char *)pairs + offset);
        do {
            pairs->Read(rb);
            row += 1;
            pairs = (cArray<eSurfacePropertyPair> *)((char *)pairs + 4);
        } while (row < total);
    }

    {
        char *typeInfo = *(char **)((char *)this + 4);
        register ReadEntry *rec __asm__("$5") = (ReadEntry *)(typeInfo + 0x38);
        __asm__ volatile("" : "+r"(rec));
        short off = rec->offset;
        void *target = (char *)this + off;
        void (*fn)(void *, cFile *, cMemPool *) = rec->fn;
        fn(target, (cFile *)pool, 0);
    }

    return result;
}

#pragma control sched=2
int gcValNavMeshDirection::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 0xC, true);
    if ((unsigned int)rb[3] >= 0xD || (unsigned int)rb[3] < 0xA ||
        gcValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x0C);
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        char *desired = (char *)this + 0x08;
        short off = rec->offset;
        char *target = desired + off;
        register int f __asm__("$18") = rb[0];
        cMemPool *desiredPool = cMemPool::GetPoolFromPtr(desired);
        rec->fn(target, (cFile *)f, desiredPool);
    }

    int sp14;
    int value = *(int *)((char *)this + 0x34);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x34);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    int readFile = rb[0];
    *(int *)((char *)this + 0x34) = newValue;

    cFileSystem::Read(*(void **)readFile, (char *)this + 0x38, 4);
    cFileSystem::Read(*(void **)rb[0], (char *)this + 0x3C, 4);
    ((cName *)((char *)this + 0x40))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(void **)rb[0], (char *)this + 0x58, 4);

    int version = rb[3];
    int versionUnder11 = (unsigned int)version < 0xB;
    int versionUnder12 = (unsigned int)version < 0xC;
    if (versionUnder11 == 0) {
        cFileSystem::Read(*(void **)rb[0], (char *)this + 0x5C, 4);
        versionUnder12 = (unsigned int)rb[3] < 0xC;
    }
    if (versionUnder12 == 0) {
        cFileSystem::Read(*(void **)rb[0], (char *)this + 0x60, 4);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
