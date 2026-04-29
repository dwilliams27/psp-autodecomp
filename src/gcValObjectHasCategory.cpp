#include "cBase.h"

class cFile;
class cMemPool;

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
};

class gcValObjectHasCategory : public gcValue {
public:
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
