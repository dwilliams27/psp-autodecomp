#include "cBase.h"

class cFile;
class cMemPool;
class cReadBlock;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                             bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;

    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _pad0[0x1C];
    cType *mParent;
    char _pad1[0x8];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcValue {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcValObjectHasCategory : public gcValue {
public:
    gcValObjectHasCategory &operator=(const gcValObjectHasCategory &);
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValObjectHasCategory_base_name[];
extern const char gcValObjectHasCategory_base_desc[];

void gcValObjectHasCategory::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue::Write(file);

    int value = *(const int *)((const char *)this + 8);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);

    const cTypeMethod *write_method =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0xC))->mType + 0x28);
    const char *base = (const char *)this + 0xC;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)write_method->fn)((void *)(base + write_method->offset), wb.file);

    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValObjectHasCategory;

void gcValObjectHasCategory::AssignCopy(const cBase *base) {
    const gcValObjectHasCategory *other = 0;

    if (base != 0) {
        if (!type_gcValObjectHasCategory) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcValObjectHasCategory_base_name,
                            gcValObjectHasCategory_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValObjectHasCategory = cType::InitializeType(
                0, 0, 0x133, type_value, gcValObjectHasCategory::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValObjectHasCategory;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
        fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValObjectHasCategory *)base;
        }
    }

    operator=(*other);
}

const cType *gcValObjectHasCategory::GetType(void) const {
    if (!type_gcValObjectHasCategory) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcValObjectHasCategory_base_name,
                        gcValObjectHasCategory_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValObjectHasCategory = cType::InitializeType(
            0, 0, 0x133, type_value, gcValObjectHasCategory::New, 0, 0, 0);
    }
    return type_gcValObjectHasCategory;
}

int gcValObjectHasCategory::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value = *(int *)((char *)this + 0x08);
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

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x08);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x08) = newValue;

    char *typeInfo = *(char **)((char *)this + 0x10);
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    char *desired = (char *)this + 0x0C;
    short off = rec->offset;
    char *target = desired + off;
    cFile *f = *(cFile **)&rb[0];
    cMemPool *desiredPool = cMemPool::GetPoolFromPtr(desired);
    rec->fn(target, f, desiredPool);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
