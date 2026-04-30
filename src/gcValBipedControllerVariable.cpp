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

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
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
int gcLValue_Read(gcValBipedControllerVariable *, cFile &, cMemPool *);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

extern const char gcValBipedControllerVariable_text_fmt[];
extern const char gcValBipedControllerVariable_text_arg[];
extern char cBaseclassdesc[];
extern char gcValBipedControllerVariablevirtualtable[];
extern char D_00000338[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x00320f74 (72B) — AssignCopy
void gcValBipedControllerVariable::AssignCopy(const cBase *base) {
    gcValBipedControllerVariable *other = dcast(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 52) = *(const int *)((char *)other + 52);
}

// 0x00320fbc (264B) — New (static factory)
cBase *gcValBipedControllerVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValBipedControllerVariable *result = 0;
    gcValBipedControllerVariable *obj =
        (gcValBipedControllerVariable *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = gcValBipedControllerVariablevirtualtable;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((void **)obj)[8] = cBaseclassdesc;
        ((void **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        ((int *)obj)[13] = 6;
        result = obj;
    }
    return (cBase *)result;
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

// SNC emits this file-scope asm after the following function, padding Read.
__asm__(".word 0x00000000\n");
__asm__(".size __0fcgcValBipedControllerVariableEReadR6FcFileP6IcMemPool, 0x10c\n");

// 0x00321290 (268B) — Read
int gcValBipedControllerVariable::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int tag = rb._pad[1];
    int version;
    __asm__ volatile("ori %0, $0, 1" : "=r"(version));
    if (tag != version || gcLValue_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(rb.file, rb._pos);
        return 0;
    }
    char *sub = (char *)this + 8;
    char *mType = *(char **)((char *)this + 12);
    const cTypeMethod *e = (const cTypeMethod *)(mType + 48);
    cFile *f = rb.file;
    typedef void (*ReadFn)(void *, cFile *, void *);
    ((ReadFn)e->fn)(sub + e->offset, f, cMemPool_GetPoolFromPtr(sub));
    cFileSystem_Read(*(void **)rb.file, (char *)this + 52, 4);
    return 1;
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
