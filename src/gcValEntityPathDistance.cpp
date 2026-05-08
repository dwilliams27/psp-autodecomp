// gcValEntityPathDistance - gcAll_psp.obj
//   0x0033A058 GetType(void) const
//   0x0033A170 Write(cFile &) const

#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
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
    char _pad[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int parent;
    cType *mType;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValEntityPathDistance : public gcValue {
public:
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

class gcValEntityScreenValue {
public:
    static cBase *New(cMemPool *, cBase *);
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

extern const char gcValEntityPathDistance_base_name[] asm("D_0036D894");
extern const char gcValEntityPathDistance_base_desc[] asm("D_0036D89C");

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
void cStrAppend(char *, const char *, ...);

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

cBase *gcValEntityPathDistance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityPathDistance *result = 0;
    gcValEntityPathDistance *obj =
        (gcValEntityPathDistance *)entry->fn(base, 0x44, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x0" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,0x7b98" : "+r"(type0));
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
        int sub_or_1 = (int)sub | 1;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = sub_or_1;
        ((int *)obj)[13] = 0;
        ((int *)obj)[14] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[15] = 0;
        ((int *)obj)[16] = obj_or_1;
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcValEntityScreenValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityScreenValue *result = 0;
    gcValEntityScreenValue *obj =
        (gcValEntityScreenValue *)entry->fn(base, 0x98, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x0" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,0x7d58" : "+r"(type0));
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
        int sub_or_1 = (int)sub | 1;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = sub_or_1;
        ((short *)obj)[0x24] = 0;
        ((short *)obj)[0x25] = 0;
        ((char *)obj)[0x34] = 0;
        gcDesiredCamera_gcDesiredCamera((char *)obj + 0x4C, (cBase *)obj);
        ((int *)obj)[33] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[34] = 0;
        ((int *)obj)[35] = obj_or_1;
        ((int *)obj)[36] = obj_or_1;
        ((int *)obj)[37] = obj_or_1;
        result = obj;
    }
    return (cBase *)result;
}

void gcValEntityPathDistance::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);

    const cTypeMethod *entity_write =
        &((const gcDesiredObject *)((const char *)this + 8))->mType->write_m;
    char *entity_base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entity_write->fn)(entity_base + entity_write->offset, wb.file);

    wb.Write(*(const unsigned int *)((const char *)this + 0x34));
    wb.Write(*(const int *)((const char *)this + 0x38));
    wb.Write(*(const int *)((const char *)this + 0x3C));
    ((const gcDesiredValue *)((const char *)this + 0x40))->Write(wb);
    wb.End();
}

int gcValEntityPathDistance::Read(cFile &file, cMemPool *pool) {
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

    cFileSystem_Read(*(void **)rb.file, (char *)this + 0x34, 4);
    cFileSystem_Read(*(void **)rb.file, (char *)this + 0x38, 4);
    cFileSystem_Read(*(void **)rb.file, (char *)this + 0x3C, 4);
    ((gcDesiredValue *)((char *)this + 0x40))->Read(rb);
    return 1;
}

void gcValEntityPathDistance::GetText(char *buf) const {
    const cTypeMethod *entityText =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x78);
    const char *entityBase = (const char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)entityText->fn)((void *)(entityBase + entityText->offset), buf);

    const char *space = (const char *)0x36DAF0;
    cStrAppend(buf, (const char *)0x36F374, space);
    cStrAppend(buf, (const char *)0x36F37C, space);
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityPathDistance;

const cType *gcValEntityPathDistance::GetType(void) const {
    if (!type_gcValEntityPathDistance) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcValEntityPathDistance_base_name,
                        gcValEntityPathDistance_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_value = cType::InitializeType(
                0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
        }
        type_gcValEntityPathDistance = cType::InitializeType(
            0, 0, 0xE3, type_value, gcValEntityPathDistance::New, 0, 0, 0);
    }
    return type_gcValEntityPathDistance;
}
