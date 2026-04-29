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
    char _pad0[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcLValue {
public:
    void Write(cFile &) const;
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcValLookAtControllerVariable : public gcLValue {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValLookAtControllerVariable_base_name[];
extern const char gcValLookAtControllerVariable_base_desc[];

void gcValLookAtControllerVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcLValue *)this)->Write(file);

    typedef void (*WriteFn)(void *, cFile *);
    const cTypeMethod *e;
    char *base;

    e = (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    base = (char *)this + 8;
    short off = e->offset;
    ((WriteFn)e->fn)(base + off, wb.file);

    e = (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 52))->mType + 40);
    base = (char *)this + 52;
    ((WriteFn)e->fn)(base + e->offset, wb.file);

    wb.Write(*(const int *)((const char *)this + 76));
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValLookAtControllerVariable;

const cType *gcValLookAtControllerVariable::GetType(void) const {
    if (!type_gcValLookAtControllerVariable) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcValLookAtControllerVariable_base_name,
                            gcValLookAtControllerVariable_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValLookAtControllerVariable = cType::InitializeType(
            0, 0, 0x10F, type_variable,
            gcValLookAtControllerVariable::New, 0, 0, 0);
    }
    return type_gcValLookAtControllerVariable;
}
