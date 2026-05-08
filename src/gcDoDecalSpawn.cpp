#include "cBase.h"

class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class cStr {
public:
    char _data[256];
    cStr(const char *, ...);
};

class gcDoDecalSpawn {
public:
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
    ~gcDoDecalSpawn(void);
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    void Write(cFile &) const;
};

class gcDoSaveGameOp {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoEntityApplyRigidBodyImpulse {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

void gcAction_gcAction(gcDoDecalSpawn *, cBase *);
void gcAction_Write(const gcDoDecalSpawn *, cFile &);
void *cMemPool_GetPoolFromPtr(const void *);
void cStrCat(char *, const char *);
extern "C" int gcAction_Read(void *, cFile &, cMemPool *)
    asm("__0fIgcActionEReadR6FcFileP6IcMemPool");
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoDecalSpawnvirtualtable[];
extern cType *D_000385D4;
extern cType *D_000385D8;
extern cType *D_000385DC;
extern cType *D_0009F5D4;
extern cType *D_0009F5E8;

inline void gcDoDecalSpawn::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

cBase *gcDoDecalSpawn::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoDecalSpawn *result = 0;
    gcDoDecalSpawn *obj = (gcDoDecalSpawn *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoDecalSpawnvirtualtable;
        *(float *)((char *)obj + 0x0C) = 0.1f;
        *(bool *)((char *)obj + 0x10) = true;
        ((int *)obj)[5] = 0;
        ((int *)obj)[6] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoDecalSpawn::GetType(void) const {
    if (D_0009F5D4 == 0) {
        if (D_000385D4 == 0) {
            if (D_000385D8 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385D8 = cType::InitializeType(0, 0, 0x6A, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000385D4 = cType::InitializeType(0, 0, 0x6B, D_000385D8,
                                               0, 0, 0, 0);
        }
        D_0009F5D4 = cType::InitializeType(0, 0, 0xA5, D_000385D4,
                                           gcDoDecalSpawn::New, 0, 0, 0);
    }
    return D_0009F5D4;
}

void gcDoDecalSpawn::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    wb.Write(*(bool *)((const char *)this + 0x10));
    wb.Write(*(float *)((const char *)this + 0x0C));
    ((const cHandle *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    wb.End();
}

int gcDoDecalSpawn::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char active;
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || gcAction_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], &active, 1);
    cFile *f = *(cFile **)&rb[0];
    *(char *)((char *)this + 0x10) = active != 0;
    cFileSystem::Read(*(cFileHandle **)f, (char *)this + 0x0C, 4);
    {
        cHandle *handle = (cHandle *)((char *)this + 0x14);
        *(int *)((char *)this + 0x14) = 0;
        handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));
    }
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

int gcDoSaveGameOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 6, true);
    if ((unsigned int)rb[3] >= 7U ||
        (unsigned int)rb[3] < 5U ||
        gcAction_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    if ((unsigned int)rb[3] >= 6U) {
        ((gcDesiredValue *)((char *)this + 0x1C))->Read(*(cReadBlock *)rb);
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoDecalSpawn::GetText(char *buf) const {
    int val = *(int *)((const char *)this + 0x14);
    const char *fmt = (const char *)0x36E454;
    int flag;
    if (val == 0) {
        flag = 1;
    } else {
        int table = 0x38890;
        int offset = (val & 0xFFFF) << 2;
        int entry = *(int *)(offset + table);
        int check = 0;
        if (entry != 0) {
            if (*(int *)(entry + 0x30) == val) {
                check = entry;
            }
        }
        flag = check == 0;
        flag = flag & 0xFF;
    }

    const char *arg;
    if (flag != 0) {
        arg = (const char *)0x36DACC;
    } else {
        int entry = 0;
        if (val != 0) {
            int table = 0x38890;
            int offset = (val & 0xFFFF) << 2;
            entry = *(int *)(offset + table);
        }
        arg = (const char *)(entry + 8);
    }
    const cStr &text = cStr(fmt, arg);
    cStrCat(buf, (const char *)&text);
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoDecalSpawn::~gcDoDecalSpawn(void) {
    *(void **)((char *)this + 4) = gcDoDecalSpawnvirtualtable;

    if ((void *)((char *)this + 0x18) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x18) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}

const cType *gcDoEntityApplyRigidBodyImpulse::GetType(void) const {
    if (D_0009F5E8 == 0) {
        if (D_000385D4 == 0) {
            if (D_000385D8 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385D8 = cType::InitializeType(0, 0, 0x6A, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000385D4 = cType::InitializeType(0, 0, 0x6B, D_000385D8,
                                               0, 0, 0, 0);
        }
        D_0009F5E8 = cType::InitializeType(
            0, 0, 0x1C2, D_000385D4, gcDoEntityApplyRigidBodyImpulse::New,
            0, 0, 0);
    }
    return D_0009F5E8;
}
