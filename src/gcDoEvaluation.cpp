#include "gcDoEntitySendMessage.h"
#include "cBase.h"

class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" {
    void cStrCat(char *, const char *);
    void gcAction_gcAction(void *, cBase *);
    void gcAction_Write(const void *, cFile &);
    void gcAction___dtor_gcAction_void(void *, int);
    void gcExpressionList_gcExpressionList(void *, cBase *);
    float gcExpressionList_Evaluate(const void *);
    void gcExpressionList_Write(const void *, void *);
}

extern char gcDoEvaluationvirtualtable[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEvaluation;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
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

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

class gcExpressionList {
public:
    ~gcExpressionList(void);
};

typedef float (*gcExprVFn)(const void *);
struct gcExprVEntry {
    short adj;
    short pad;
    gcExprVFn fn;
};


inline void *operator new(unsigned int, void *p) { return p; }

void *cMemPool_GetPoolFromPtr(const void *);

inline void gcDoEvaluation::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

int gcDoEvaluation::GetMaxBranches(void) const {
    return 2;
}

gcExpression *gcDoEvaluation::GetBranch(int index) const {
    int saved = index;
    gcExpression *result;
    if (index == 0) {
        result = *(gcExpression **)((const char *)this + 0x10);
    } else {
        index = 0;
        if (saved == 1) {
            index = (int)*(gcExpression **)((const char *)this + 0x18);
        }
        result = (gcExpression *)index;
    }
    return result;
}

// 0x0014b048, 84B
gcDoEvaluation::gcDoEvaluation(cBase *parent) {
    gcAction_gcAction(this, parent);
    ((void **)this)[1] = gcDoEvaluationvirtualtable;
    *(unsigned int *)((char *)this + 0xC) = ((unsigned int)this) | 1;
    gcExpressionList_gcExpressionList((char *)this + 0x10, (cBase *)this);
    gcExpressionList_gcExpressionList((char *)this + 0x18, (cBase *)this);
}

// 0x002d872c, 124B — static
cBase *gcDoEvaluation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEvaluation *result = 0;
    gcDoEvaluation *obj = (gcDoEvaluation *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEvaluation(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x002d87a8, 280B
const cType *gcDoEvaluation::GetType(void) const {
    if (!type_gcDoEvaluation) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEvaluation = cType::InitializeType(0, 0, 0x76, type_action,
                                                     gcDoEvaluation::New,
                                                     0, 0, 0);
    }
    return type_gcDoEvaluation;
}

// 0x0014b12c, 72B
void gcDoEvaluation::GetText(char *buf) const {
    const char *s;
    if (IsElseIf()) {
        s = (const char *)0x36EBA0;
    } else {
        s = (const char *)0x36EBA8;
    }
    cStrCat(buf, s);
}

// 0x0014b174, 472B
int gcDoEvaluation::IsElseIf(void) const {
    const cBase *base = *(const cBase **)this;
    const cBase *prev = 0;
    if (base != 0) {
        if (!type_gcDoEvaluation) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(0, 0, 0x6A,
                                                            type_base,
                                                            0, 0, 0, 0);
                }
                type_action = cType::InitializeType(0, 0, 0x6B,
                                                    type_expression,
                                                    0, 0, 0, 0);
            }
            type_gcDoEvaluation = cType::InitializeType(0, 0, 0x76,
                                                        type_action,
                                                        gcDoEvaluation::New,
                                                        0, 0, 0);
        }

        VTableSlot *slot = (VTableSlot *)((char *)((void **)base)[1] + 8);
        const cType *myType = type_gcDoEvaluation;
        short offset = slot->offset;
        const cType *type = slot->getType((char *)base + offset);
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
            type = (const cType *)((const cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            prev = base;
        }
    }

    const cBase *candidate = prev;
    int result = 0;
    if (candidate != 0 && *(const gcDoEvaluation **)((const char *)candidate + 0x18) == this) {
        VTableSlot *slot = (VTableSlot *)((char *)((void **)this)[1] + 0xA8);
        if (slot->getType((char *)this + slot->offset) == 0) {
            result = 1;
        }
    }
    return result & 0xFF;
}

// 0x0014b09c, 144B
float gcDoEvaluation::Evaluate(void) const {
    int val = *(int *)((char *)this + 0xC);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    const void *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (const void *)val;
    }
    const gcExprVEntry *e = *(const gcExprVEntry **)((const char *)ptr + 4) + 14;
    float result = e->fn((const char *)ptr + e->adj);
    float r;
    if (result != 0.0f) {
        r = gcExpressionList_Evaluate((char *)this + 0x10);
    } else {
        r = gcExpressionList_Evaluate((char *)this + 0x18);
    }
    return r;
}

// 0x0014ae58, 148B
void gcDoEvaluation::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    int val = ((int *)this)[3];
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    gcExpressionList_Write((char *)this + 0x10, &wb);
    gcExpressionList_Write((char *)this + 0x18, &wb);
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoEvaluation::~gcDoEvaluation(void) {
    *(void **)((char *)this + 4) = gcDoEvaluationvirtualtable;

    ((gcExpressionList *)((char *)this + 0x18))->~gcExpressionList();
    ((gcExpressionList *)((char *)this + 0x10))->~gcExpressionList();

    if ((void *)((char *)this + 0x0C) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
