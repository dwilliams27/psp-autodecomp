#include "gcDoReturn.h"
#include "cBase.h"

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &);
};

gcDoSwitchCase *dcast(const cBase *);

void gcDoSwitchCase__gcDoSwitchCase_cBaseptr(gcDoSwitchCase *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

void gcDoSwitchCase::AssignCopy(const cBase *base) {
    gcDoSwitchCase *other = dcast(base);
    int *self = (int *)this;
    int *src = (int *)other;
    self[2] = self[2] & ~3;
    self[2] = self[2] | (src[2] & 3);
    ((cBaseArray *)((char *)this + 0xC))->operator=(*(const cBaseArray *)((char *)other + 0xC));
    ((gcExpressionList *)((char *)this + 0x14))->operator=(*(const gcExpressionList *)((char *)other + 0x14));
}

cBase *gcDoSwitchCase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSwitchCase *result = 0;
    gcDoSwitchCase *obj = (gcDoSwitchCase *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcDoSwitchCase__gcDoSwitchCase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

int gcDoSwitchCase::GetMaxChildren(void) const {
    return 3;
}

int gcDoSwitchCase::GetExprFlags(void) const {
    return 0x18;
}

int gcValCaseRange::GetMaxChildren(void) const {
    return 2;
}

int gcValCaseRange::GetExprFlags(void) const {
    return 3;
}

int gcDoSwitch::GetMaxChildren(void) const {
    return 1;
}

int gcDoSwitch::GetMaxBranches(void) const {
    return 2;
}
