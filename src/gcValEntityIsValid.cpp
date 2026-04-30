#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
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

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityTemplate : public gcDesiredObject {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityIsValid : public gcValue {
public:
    char _pad0[0x34];
    bool field_34;

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

gcValEntityIsValid *dcast(const cBase *);

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern const char gcValEntityIsValid_base_name[];
extern const char gcValEntityIsValid_base_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcValEntityIsValid::AssignCopy(const cBase *base) {
    gcValEntityIsValid *other = dcast(base);
    const gcDesiredEntity *otherEntity =
        (const gcDesiredEntity *)((const char *)other + 8);
    __asm__ volatile("" :: "r"(otherEntity) : "memory");
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*otherEntity);
    gcDesiredEntityTemplate *destTemplate =
        (gcDesiredEntityTemplate *)((char *)this + 0x38);
    field_34 = other->field_34;
    destTemplate->operator=(
        *(const gcDesiredEntityTemplate *)((const char *)other + 0x38));
}

void gcValEntityIsValid::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    const cTypeMethod *entityWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    const char *entityBase = (const char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entityWrite->fn)((void *)(entityBase + entityWrite->offset), wb.file);

    const cTypeMethod *templateWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x38))->mType + 0x28);
    const char *templateBase = (const char *)this + 0x38;
    ((WriteFn)templateWrite->fn)((void *)(templateBase + templateWrite->offset), wb.file);

    wb.Write(field_34);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityIsValid;

cBase *gcValEntityIsValid::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityIsValid *result = 0;
    gcValEntityIsValid *obj =
        (gcValEntityIsValid *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        register void *type0 asm("a0");
        register void *type1 asm("a0");
        register void *helper_desc asm("a0");

        ((void **)obj)[1] = (void *)0x37E6A8;
        __asm__ volatile("lui %0,0x0" : "=r"(type0));
        ((cBase **)obj)[0] = parent;
        __asm__ volatile("addiu %0,%0,0x7808" : "+r"(type0));
        ((void **)obj)[1] = type0;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                         : "=r"(type1));
        ((void **)obj)[3] = type1;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                         : "=r"(helper_desc));
        ((void **)obj)[8] = (void *)0x37E6A8;
        ((void **)obj)[3] = helper_desc;
        register void *desired_desc asm("a0");
        __asm__ volatile("lui %0,0x39" : "=r"(desired_desc) :: "memory");
        ((char **)obj)[7] = sub;
        __asm__ volatile("addiu %0,%0,-0x7a98" : "+r"(desired_desc));
        ((void **)obj)[8] = desired_desc;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        int sub_or_1 = (int)sub | 1;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = sub_or_1;
        ((char *)obj)[0x34] = 0;
        gcDesiredObject_ctor((char *)obj + 0x38, obj);
        register void *template_desc asm("a0");
        __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x6af8"
                         : "=r"(template_desc));
        __asm__ volatile("ori $5,$0,0x7" ::: "memory");
        ((void **)obj)[15] = template_desc;
        __asm__ volatile("sw $5,0x44(%0)" :: "r"(obj) : "memory");
        int obj_or_1 = (int)obj | 1;
        __asm__ volatile("" : "+r"(obj_or_1) :: "memory");
        ((int *)obj)[18] = 0;
        ((int *)obj)[19] = obj_or_1;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValEntityIsValid::GetType(void) const {
    if (!type_gcValEntityIsValid) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcValEntityIsValid_base_name,
                                                       gcValEntityIsValid_base_desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityIsValid = cType::InitializeType(0, 0, 0xE4, type_value,
                                                        gcValEntityIsValid::New,
                                                        0, 0, 0);
    }
    return type_gcValEntityIsValid;
}
