#include "gcValUnaryOp.h"

extern char gcValUnaryOpvirtualtable[];
void cStrCat(char *, const char *);
void gcValUnaryOp_gcValUnaryOp(gcValUnaryOp *, cBase *);

gcValUnaryOp::gcValUnaryOp(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(char **)((char *)this + 4) = gcValUnaryOpvirtualtable;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0xC) = (int)this | 1;
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
