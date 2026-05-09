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
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];

    cReadBlock(cFile &, unsigned int, bool);
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

class gcLValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcValLookAtControllerVariable : public gcLValue {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValLookAtControllerVariable_base_name[];
extern const char gcValLookAtControllerVariable_base_desc[];

extern "C" {
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

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

int gcValLookAtControllerVariable::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 2, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    int tag = rb._pad[1];
    int version;
    __asm__ volatile("ori %0, $0, 2" : "=r"(version));
    if (tag != version || gcLValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(rb.file, rb._pos);
        return 0;
    }

    char *entity = (char *)this + 8;
    char *entityType = *(char **)((char *)this + 12);
    const cTypeMethod *entityRead = (const cTypeMethod *)(entityType + 0x30);
    cFile *f = rb.file;
    typedef void (*ReadFn)(void *, cFile *, void *);
    register char *entityTarget __asm__("$20") =
        entity + entityRead->offset;
    ((ReadFn)entityRead->fn)(entityTarget, f, cMemPool::GetPoolFromPtr(entity));

    char *controller = (char *)this + 0x34;
    char *controllerType = *(char **)((char *)this + 0x38);
    const cTypeMethod *controllerRead =
        (const cTypeMethod *)(controllerType + 0x30);
    f = rb.file;
    register char *controllerTarget __asm__("$20") =
        controller + controllerRead->offset;
    ((ReadFn)controllerRead->fn)(controllerTarget, f,
                                 cMemPool::GetPoolFromPtr(controller));

    cFileSystem_Read(*(void **)rb.file, (char *)this + 0x4C, 4);
    return result;
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
