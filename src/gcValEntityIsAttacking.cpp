#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];

    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _pad0[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

class gcValEntityIsAttacking : public gcValue {
public:
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

void cStrCat(char *, const char *);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

void gcValEntityIsAttacking::GetText(char *buf) const {
    const cTypeMethod *entityText =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x78);
    const char *entityBase = (const char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)entityText->fn)((void *)(entityBase + entityText->offset), buf);

    if (*(const int *)((const char *)this + 0x4C) & 2) {
        cStrCat(buf, (const char *)0x36F2EC);
    } else {
        cStrCat(buf, (const char *)0x36F30C);
    }
}

void gcValEntityIsAttacking::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);

    const cTypeMethod *entityWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    const char *entityBase = (const char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entityWrite->fn)((void *)(entityBase + entityWrite->offset), wb.file);

    const cTypeMethod *attackingWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 0x28);
    const char *attackingBase = (const char *)this + 0x34;
    ((WriteFn)attackingWrite->fn)((void *)(attackingBase + attackingWrite->offset), wb.file);

    wb.Write(*(const int *)((const char *)this + 0x4C));
    wb.End();
}

int gcValEntityIsAttacking::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int tag = rb._pad[1];
    int version;
    __asm__ volatile("ori %0, $0, 1" : "=r"(version));
    if (tag != version || gcValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(rb.file, rb._pos);
        return 0;
    }

    char *entity = (char *)this + 8;
    char *entityType = *(char **)((char *)this + 12);
    const cTypeMethod *entityRead = (const cTypeMethod *)(entityType + 0x30);
    cFile *f = rb.file;
    typedef void (*ReadFn)(void *, cFile *, void *);
    ((ReadFn)entityRead->fn)(entity + entityRead->offset, f,
                             cMemPool_GetPoolFromPtr(entity));

    char *attacking = (char *)this + 0x34;
    char *attackingType = *(char **)((char *)this + 0x38);
    const cTypeMethod *attackingRead =
        (const cTypeMethod *)(attackingType + 0x30);
    f = rb.file;
    ((ReadFn)attackingRead->fn)(attacking + attackingRead->offset, f,
                                cMemPool_GetPoolFromPtr(attacking));

    cFileSystem_Read(*(void **)rb.file, (char *)this + 0x4C, 4);
    return 1;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityIsAttacking;

const cType *gcValEntityIsAttacking::GetType(void) const {
    if (!type_gcValEntityIsAttacking) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityIsAttacking = cType::InitializeType(
            0, 0, 0x1B7, type_value, gcValEntityIsAttacking::New, 0, 0, 0);
    }
    return type_gcValEntityIsAttacking;
}

cBase *gcValEntityIsAttacking::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityIsAttacking *result = 0;
    gcValEntityIsAttacking *obj =
        (gcValEntityIsAttacking *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x0" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,0x7568" : "+r"(type0));
            ((void **)obj)[1] = type0;
        }
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        {
            register void *type1 asm("a0");
            __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                             : "=r"(type1));
            ((void **)obj)[3] = type1;
        }
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        {
            register void *helper_desc asm("a0");
            __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                             : "=r"(helper_desc));
            ((void **)obj)[8] = (void *)0x37E6A8;
            ((void **)obj)[3] = helper_desc;
        }
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        int sub_or_1 = (int)sub | 1;
        ((void **)obj)[14] = (void *)0x37E6A8;
        ((int *)obj)[12] = sub_or_1;
        ((gcValEntityIsAttacking **)obj)[13] = obj;
        ((void **)obj)[14] = (void *)0x388568;
        ((char *)obj)[0x3C] = 1;
        ((char *)obj)[0x3D] = 0;
        ((int *)obj)[16] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[17] = 0;
        ((int *)obj)[18] = obj_or_1;
        ((int *)obj)[19] = 1;
        result = obj;
    }
    return (cBase *)result;
}
