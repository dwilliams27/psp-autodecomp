#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoChangeState {
public:
    void AssignCopy(const cBase *);
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    gcDoChangeState &operator=(const gcDoChangeState &);
    static void operator delete(void *);
    ~gcDoChangeState(void);
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

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoChangeStatevirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoChangeState asm("D_0009F5CC");

inline void gcDoChangeState::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

void gcDoChangeState::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoChangeState) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoChangeState = cType::InitializeType(
                0, 0, 0xC2, type_action,
                gcDoChangeState::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoChangeState;
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
    *this = *(const gcDoChangeState *)copy;
}

int gcDoChangeState::GetMaxChildren(void) const {
    return 4;
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcDoChangeStatedtv, 0x144\n");

gcDoChangeState::~gcDoChangeState(void) {
    register void *vtable __asm__("$4");
    __asm__ volatile("lui %0, 0\n"
                     "addiu %0, %0, 0x1d28"
                     : "=r"(vtable));
    *(void **)((char *)this + 4) = vtable;
    char *last = (char *)this + 0x20;
    char *array = (char *)this + 0x18;
    char *children = (char *)this + 0x14;

    if ((void *)last != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x20);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x20) = 0;
            }
        }
    }

    if ((void *)array != 0) {
        ((cBaseArray *)array)->RemoveAll();
    }

    if ((void *)children != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
