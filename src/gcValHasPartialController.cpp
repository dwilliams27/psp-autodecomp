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

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValHasPartialController : public gcValue {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValHasPartialController_base_name[];
extern const char gcValHasPartialController_base_desc[];

void gcValHasPartialController::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    const cTypeMethod *entityWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    const char *entityBase = (const char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entityWrite->fn)((void *)(entityBase + entityWrite->offset), wb.file);

    const cTypeMethod *templateWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 0x28);
    const char *templateBase = (const char *)this + 0x34;
    ((WriteFn)templateWrite->fn)((void *)(templateBase + templateWrite->offset), wb.file);

    wb.Write(*(const bool *)((const char *)this + 0x4C));
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValHasPartialController;

const cType *gcValHasPartialController::GetType(void) const {
    if (!type_gcValHasPartialController) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcValHasPartialController_base_name,
                        gcValHasPartialController_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValHasPartialController =
            cType::InitializeType(0, 0, 0x1C8, type_value,
                                  gcValHasPartialController::New, 0, 0, 0);
    }
    return type_gcValHasPartialController;
}
