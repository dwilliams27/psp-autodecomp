// Days of Thunder decompilation: gcValBipedControllerVariable
//   0x00320f74 AssignCopy(const cBase *)
//   0x00321218 Write(cFile &) const
//   0x00321ab4 GetText(char *) const

#include "gcValBipedControllerVariable.h"
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
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78

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

gcValBipedControllerVariable *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValBipedControllerVariable *, cFile &);

extern const char gcValBipedControllerVariable_text_fmt[];
extern const char gcValBipedControllerVariable_text_arg[];

// 0x00320f74 (72B) — AssignCopy
void gcValBipedControllerVariable::AssignCopy(const cBase *base) {
    gcValBipedControllerVariable *other = dcast(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 52) = *(const int *)((char *)other + 52);
}

// 0x00321218 (120B) — Write
void gcValBipedControllerVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.Write(*(const int *)((const char *)this + 52));
    wb.End();
}

// 0x00321ab4 (88B) — GetText
void gcValBipedControllerVariable::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValBipedControllerVariable_text_fmt,
               gcValBipedControllerVariable_text_arg);
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValBipedControllerVariable;

const cType *gcValBipedControllerVariable::GetType(void) const {
    if (!type_gcValBipedControllerVariable) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
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
        type_gcValBipedControllerVariable = cType::InitializeType(
            0, 0, 0xD8, type_variable, gcValBipedControllerVariable::New,
            0, 0, 0);
    }
    return type_gcValBipedControllerVariable;
}
