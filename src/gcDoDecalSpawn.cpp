#include "cBase.h"

class cFile;
class cMemPool;
class cType;

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
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoDecalSpawn {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
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

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoDecalSpawn *, cBase *);
void gcAction_Write(const gcDoDecalSpawn *, cFile &);
extern char gcDoDecalSpawnvirtualtable[];
extern cType *D_000385D4;
extern cType *D_000385D8;
extern cType *D_000385DC;
extern cType *D_0009F5D4;
extern cType *D_0009F5E8;

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
