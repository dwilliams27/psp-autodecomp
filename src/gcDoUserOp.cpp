#include "gcDoUserOp.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

void gcAction_Write(const gcDoUserOp *, cFile &);
void gcAction_gcAction(gcDoUserOp *, cBase *);

extern char gcDoUserOpvirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUserOp asm("D_0009F758");

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void gcDoUserOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    wb.End();
}

cBase *gcDoUserOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUserOp *result = 0;
    gcDoUserOp *obj = (gcDoUserOp *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUserOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoUserOp::GetType(void) const {
    if (!type_gcDoUserOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUserOp = cType::InitializeType(
            0, 0, 0x288, type_action, gcDoUserOp::New, 0, 0, 0);
    }
    return type_gcDoUserOp;
}
