#include "gcDoReturn.h"
#include "cBase.h"

class cMemPool;
class cType;

extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoStateFunctionvirtualtable[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

void *cMemPool_GetPoolFromPtr(const void *);

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoStateFunction asm("D_0009F6FC");

inline void gcDoStateFunction::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

int gcDoStateFunction::GetMaxChildren(void) const {
    return 4;
}

const cType *gcDoStateFunction::GetType(void) const {
    if (!type_gcDoStateFunction) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoStateFunction = cType::InitializeType(
            0, 0, 0xC9, type_action, gcDoStateFunction::New, 0, 0, 0x80);
    }
    return type_gcDoStateFunction;
}

void gcDoStateFunction::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoStateFunction) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoStateFunction = cType::InitializeType(
                0, 0, 0xC9, type_action, gcDoStateFunction::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoStateFunction;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoStateFunction *)copy;
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoStateFunction::~gcDoStateFunction(void) {
    *(void **)((char *)this + 4) = gcDoStateFunctionvirtualtable;

    cBaseArray *items = (cBaseArray *)((char *)this + 0x14);

    if ((void *)((char *)this + 0x1C) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x1C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x1C) = 0;
        }
    }

    if (items != 0) {
        items->RemoveAll();
    }

    gcAction___dtor_gcAction_void(this, 0);
}
