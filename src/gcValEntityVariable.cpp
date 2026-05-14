#include "cBase.h"

class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
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

class gcValue {
public:
    void Write(cFile &) const;
};

class gcLValue : public gcValue {
public:
    void Write(cFile &) const;
};

class gcDesiredObject {
public:
    int parent;
    cType *mType;
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

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValEntityVariable : public gcLValue {
public:
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
    ~gcValEntityVariable(void);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValEntityVariable_base_name[];
extern const char gcValEntityVariable_base_desc[];
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

void gcValEntityVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);

    const cTypeMethod *entity_write = &((const gcDesiredObject *)((const char *)this + 8))->mType->write_m;
    char *entity_base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entity_write->fn)(entity_base + entity_write->offset, wb.file);

    const cTypeMethod *template_write = &((const gcDesiredObject *)((const char *)this + 0x34))->mType->write_m;
    char *template_base = (char *)this + 0x34;
    ((WriteFn)template_write->fn)(template_base + template_write->offset, wb.file);

    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_lvalue;
static cType *type_gcValEntityVariable;

const cType *gcValEntityVariable::GetType(void) const {
    if (!type_gcValEntityVariable) {
        if (!type_lvalue) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcValEntityVariable_base_name,
                            gcValEntityVariable_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_lvalue = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValEntityVariable = cType::InitializeType(
            0, 0, 0xD2, type_lvalue, gcValEntityVariable::New, 0, 0, 0);
    }
    return type_gcValEntityVariable;
}

cBase *gcValEntityVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityVariable *result = 0;
    gcValEntityVariable *obj =
        (gcValEntityVariable *)entry->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x1" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,-0x7ff8" : "+r"(type0));
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
        ((gcValEntityVariable **)obj)[13] = obj;
        ((void **)obj)[14] = (void *)0x388568;
        ((char *)obj)[0x3C] = 1;
        ((char *)obj)[0x3D] = 0;
        ((int *)obj)[16] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[17] = 0;
        ((int *)obj)[18] = obj_or_1;
        result = obj;
    }
    return (cBase *)result;
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oTgcValEntityVariabledtv, 0x1dc\n");

gcValEntityVariable::~gcValEntityVariable(void) {
    register void *vt asm("a0");
    __asm__ volatile("lui %0,0x1\n\taddiu %0,%0,-0x7ff8" : "=r"(vt));
    *(void **)((char *)this + 4) = vt;
    char *p34 = (char *)this + 0x34;
    void *baseDesc = (void *)0x37E6A8;
    char *outer = (char *)this + 0x8;

    if ((void *)p34 != 0) {
        *(void **)((char *)this + 0x38) = (void *)0x388568;
        if ((void *)((char *)this + 0x48) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x48);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x48) = 0;
            }
        }
        *(void **)((char *)this + 0x38) = baseDesc;
    }
    if ((void *)outer != 0) {
        *(void **)((char *)this + 0xC) = (void *)0x388A48;
        if ((void *)((char *)this + 0x1C) != 0) {
            *(void **)((char *)this + 0x20) = (void *)0x388568;
            if ((void *)((char *)this + 0x30) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x30);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0 && val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x30) = 0;
                }
            }
            *(void **)((char *)this + 0x20) = baseDesc;
        }
        *(void **)((char *)this + 0xC) = (void *)0x3889A8;
        if ((void *)((char *)this + 0x10) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x10);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
        *(void **)((char *)this + 0xC) = baseDesc;
    }
    *(void **)((char *)this + 4) = baseDesc;
}
