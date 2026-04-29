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

class gcValEntityPartialControllerVariable : public gcLValue {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValEntityPartialControllerVariable_base_name[];
extern const char gcValEntityPartialControllerVariable_base_desc[];

void gcValEntityPartialControllerVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);

    const cTypeMethod *entity_write = &((const gcDesiredObject *)((const char *)this + 8))->mType->write_m;
    char *entity_base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entity_write->fn)(entity_base + entity_write->offset, wb.file);

    const cTypeMethod *template_write = &((const gcDesiredObject *)((const char *)this + 0x34))->mType->write_m;
    char *template_base = (char *)this + 0x34;
    ((WriteFn)template_write->fn)(template_base + template_write->offset, wb.file);

    wb.Write(*(const int *)((const char *)this + 0x4C));
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_lvalue;
static cType *type_gcValEntityPartialControllerVariable;

const cType *gcValEntityPartialControllerVariable::GetType(void) const {
    if (!type_gcValEntityPartialControllerVariable) {
        if (!type_lvalue) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcValEntityPartialControllerVariable_base_name,
                            gcValEntityPartialControllerVariable_base_desc,
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
        type_gcValEntityPartialControllerVariable = cType::InitializeType(
            0, 0, 0x207, type_lvalue,
            gcValEntityPartialControllerVariable::New, 0, 0, 0);
    }
    return type_gcValEntityPartialControllerVariable;
}
