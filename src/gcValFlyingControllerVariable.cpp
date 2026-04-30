// Days of Thunder decompilation: gcValFlyingControllerVariable
//   0x003436fc AssignCopy(const cBase *)
//   0x00343744 New(cMemPool *, cBase *) static
//   0x0034399c Write(cFile &) const
//   0x00343a14 Read(cFile &, cMemPool *)
//   0x00343df0 Set(float)  (already matched)
//   0x00343df8 GetText(char *) const

#include "gcValFlyingControllerVariable.h"
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
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78
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

gcValFlyingControllerVariable *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValFlyingControllerVariable *, cFile &);
int gcLValue_Read(gcValFlyingControllerVariable *, cFile &, cMemPool *);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

extern const char gcValFlyingControllerVariable_text_fmt[];
extern const char gcValFlyingControllerVariable_text_arg[];

extern char gcValFlyingControllerVariable_base_vtbl[];
extern char gcValFlyingControllerVariable_vtbl[];
extern char gcValFlyingControllerVariable_desired_vtbl[];
extern char gcValFlyingControllerVariable_entity_vtbl[];
extern char gcValFlyingControllerVariable_type_tag[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValFlyingControllerVariable;

// 0x003436fc (72B) — AssignCopy
void gcValFlyingControllerVariable::AssignCopy(const cBase *base) {
    gcValFlyingControllerVariable *other = dcast(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 52) = *(const int *)((char *)other + 52);
}

// 0x00343744 (260B) — New (static factory)
cBase *gcValFlyingControllerVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValFlyingControllerVariable *result = 0;
    gcValFlyingControllerVariable *obj =
        (gcValFlyingControllerVariable *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValFlyingControllerVariable_base_vtbl;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = gcValFlyingControllerVariable_vtbl;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = gcValFlyingControllerVariable_type_tag;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((char **)obj)[3] = gcValFlyingControllerVariable_desired_vtbl;
        ((char **)obj)[8] = gcValFlyingControllerVariable_base_vtbl;
        ((char **)obj)[7] = sub;
        ((char **)obj)[8] = gcValFlyingControllerVariable_entity_vtbl;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        ((int *)obj)[13] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0034399c (120B) — Write
void gcValFlyingControllerVariable::Write(cFile &file) const {
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

// 0x00343a14 (268B) — Read
int gcValFlyingControllerVariable::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    if (rb._pad[1] != 1 || gcLValue_Read(this, file, pool) == 0) {
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

// 0x00343df0 (8B) — Set (already matched)
void gcValFlyingControllerVariable::Set(float) {
}

// 0x00343df8 (88B) — GetText
void gcValFlyingControllerVariable::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValFlyingControllerVariable_text_fmt,
               gcValFlyingControllerVariable_text_arg);
}

const cType *gcValFlyingControllerVariable::GetType(void) const {
    if (!type_gcValFlyingControllerVariable) {
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
        type_gcValFlyingControllerVariable = cType::InitializeType(
            0, 0, 0x1B0, type_variable, gcValFlyingControllerVariable::New,
            0, 0, 0);
    }
    return type_gcValFlyingControllerVariable;
}
