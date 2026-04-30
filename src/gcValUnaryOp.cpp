#include "gcValUnaryOp.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char gcValUnaryOpvirtualtable[];
extern char cBaseclassdesc[];
void cStrCat(char *, const char *);
void gcValUnaryOp_gcValUnaryOp(gcValUnaryOp *, cBase *);

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValUnaryOp asm("D_0009F91C");

gcValUnaryOp::gcValUnaryOp(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(char **)((char *)this + 4) = gcValUnaryOpvirtualtable;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0xC) = (int)this | 1;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oMgcValUnaryOpdtv, 0xd4\n");

gcValUnaryOp::~gcValUnaryOp(void) {
    *(char **)((char *)this + 4) = gcValUnaryOpvirtualtable;
    char *slot = (char *)this + 0x0C;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            gcValUnaryOp_DeleteRecord *rec = (gcValUnaryOp_DeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

int gcValUnaryOp::GetExprFlags(void) const {
    int op = *(int *)((char *)this + 8);
    int result = 8;
    if (op != 0 && op != 0xA) {
        result = 0xB;
    }
    return result;
}

gcExpression *gcValUnaryOp::GetChild(int index) const {
    gcExpression *result = 0;
    if (index == 0) {
        int val = ((int *)this)[3];
        int flag = 0;
        if (val & 1) {
            flag = 1;
        }
        gcExpression *ptr;
        if (flag != 0) {
            ptr = 0;
        } else {
            ptr = (gcExpression *)val;
        }
        result = ptr;
    }
    return result;
}

void gcValUnaryOp::GetText(char *buf) const {
    const char *text = "???";
    char *dest = buf;
    cStrCat(dest, text);
}

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcValUnaryOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValUnaryOp *result = 0;
    gcValUnaryOp *obj = (gcValUnaryOp *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        gcValUnaryOp_gcValUnaryOp(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValUnaryOp::GetType(void) const {
    if (!type_gcValUnaryOp) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValUnaryOp = cType::InitializeType(0, 0, 0x6E, type_value,
                                                  gcValUnaryOp::New,
                                                  0, 0, 0);
    }
    return type_gcValUnaryOp;
}
