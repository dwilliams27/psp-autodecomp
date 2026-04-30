// gcValEntityPathDirection - gcAll_psp.obj
//   0x00338d0c AssignCopy(const cBase *)
//   0x00338e70 GetType(void) const
//   0x00338f88 Write(cFile &) const

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

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityPathDirection : public gcValue {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

extern const char gcValEntityPathDirection_base_name[];
extern const char gcValEntityPathDirection_base_desc[];

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

void gcValEntityPathDirection::AssignCopy(const cBase *base) {
    gcValEntityPathDirection *other = dcast<gcValEntityPathDirection>(base);
    char *src = (char *)other;
    const gcDesiredEntity *srcEntity = (const gcDesiredEntity *)(src + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcEntity);
    other = (gcValEntityPathDirection *)src;
    *(int *)((char *)this + 0x34) = *(const int *)((char *)other + 0x34);
    *(int *)((char *)this + 0x38) = *(const int *)((char *)other + 0x38);
    *(int *)((char *)this + 0x3C) = *(const int *)((char *)other + 0x3C);
}

cBase *gcValEntityPathDirection::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityPathDirection *result = 0;
    gcValEntityPathDirection *obj =
        (gcValEntityPathDirection *)entry->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x0" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,0x7ab8" : "+r"(type0));
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
        ((int *)obj)[15] = 0;
        result = obj;
    }
    return (cBase *)result;
}

void gcValEntityPathDirection::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    const cTypeMethod *entityWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    const char *entityBase = (const char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entityWrite->fn)((void *)(entityBase + entityWrite->offset),
                               wb.file);

    wb.Write(*(const int *)((const char *)this + 0x34));
    wb.Write(*(const unsigned int *)((const char *)this + 0x3C));
    wb.Write(*(const int *)((const char *)this + 0x38));
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityPathDirection;

const cType *gcValEntityPathDirection::GetType(void) const {
    if (!type_gcValEntityPathDirection) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcValEntityPathDirection_base_name,
                        gcValEntityPathDirection_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityPathDirection =
            cType::InitializeType(0, 0, 0xA4, type_value,
                                  gcValEntityPathDirection::New, 0, 0, 0);
    }
    return type_gcValEntityPathDirection;
}
