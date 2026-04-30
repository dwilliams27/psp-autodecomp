#include "gcDoReturn.h"

class cBaseArray {
public:
    void RemoveAll(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoStateParentHandler asm("D_0009F700");

extern char gcDoStateParentHandlervirtualtable[];
extern "C" {
    void gcAction___dtor_gcAction_void(void *, int);
    void *cMemPool_GetPoolFromPtr_dtor(void *);
}

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlockForDelete {
    char pad[0x1C];
    char *allocTable;
};

inline void gcDoStateParentHandler::operator delete(void *p) {
    void *pool = cMemPool_GetPoolFromPtr_dtor(p);
    char *block = *(char **)((char *)pool + 0x24);
    DtorDeleteRecord *rec = (DtorDeleteRecord *)(((PoolBlockForDelete *)block)->allocTable + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oWgcDoStateParentHandlerdtv, 0xec\n");

gcDoStateParentHandler::~gcDoStateParentHandler(void) {
    *(void **)((char *)this + 4) = gcDoStateParentHandlervirtualtable;

    cBaseArray *items = (cBaseArray *)((char *)this + 0x0C);

    if ((void *)((char *)this + 0x14) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            short off = slot->offset;
            void (*fn)(void *, void *) = slot->fn;
            fn((char *)val + off, (void *)3);
            *(int *)((char *)this + 0x14) = 0;
        }
    }

    if (items != 0) {
        items->RemoveAll();
    }

    gcAction___dtor_gcAction_void(this, 0);
}

int gcDoStateParentHandler::GetExprFlags(void) const {
    return 0xA;
}

const cType *gcDoStateParentHandler::GetType(void) const {
    if (!type_gcDoStateParentHandler) {
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
        type_gcDoStateParentHandler = cType::InitializeType(
            0, 0, 0x2B6, type_action, gcDoStateParentHandler::New, 0, 0, 0x80);
    }
    return type_gcDoStateParentHandler;
}
