#include "gcDoReturn.h"
#include "gcExpressionList.h"
#include "cBase.h"

extern "C" {
    void cStrCat(char *, const char *);
}

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern const char gcDoReturn_base_name[];
extern const char gcDoReturn_base_desc[];

static cType *gcDoReturn_type_base;
static cType *gcDoReturn_type_expression;
static cType *gcDoReturn_type_action;
static cType *gcDoReturn_type_self;

extern char gcDoReturnvirtualtable[];
extern "C" {
    void gcAction___dtor_gcAction_void(void *, int);
    void *cMemPool_GetPoolFromPtr_dtor(void *);
}

struct DtorDeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// 0x002fa530, 212B
extern "C" {

void gcDoReturn___dtor_gcDoReturn_void(gcDoReturn *self, int flags) {
    if (self == 0) goto dt_end;
    ((void **)self)[1] = gcDoReturnvirtualtable;
    if ((char *)self + 0xC == 0) goto dt_action;
    {
        int val = ((int *)self)[3];
        int owned = 1;
        if (val & 1) owned = 0;
        if (owned == 0) goto dt_action;
        if (val == 0) goto dt_action;
        {
            void *vt = *(void **)((char *)val + 4);
            DtorDeleteRecord *rec = (DtorDeleteRecord *)((char *)vt + 0x50);
            short off = rec->offset;
            rec->fn((char *)val + off, (void *)3);
            ((int *)self)[3] = 0;
        }
    }
dt_action:
    gcAction___dtor_gcAction_void(self, 0);
    if ((flags & 1) == 0) goto dt_end;
    {
        void *pool = cMemPool_GetPoolFromPtr_dtor(self);
        void *block = *(void **)((char *)pool + 0x24);
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, self);
    }
dt_end:;
}

}

// 0x002f9d80, 280B
const cType *gcDoReturn::GetType(void) const {
    if (!gcDoReturn_type_self) {
        if (!gcDoReturn_type_action) {
            if (!gcDoReturn_type_expression) {
                if (!gcDoReturn_type_base) {
                    gcDoReturn_type_base = cType::InitializeType(gcDoReturn_base_name, gcDoReturn_base_desc, 1, 0, 0, 0, 0, 0);
                }
                gcDoReturn_type_expression = cType::InitializeType(0, 0, 0x6A, gcDoReturn_type_base, 0, 0, 0, 0);
            }
            gcDoReturn_type_action = cType::InitializeType(0, 0, 0x6B, gcDoReturn_type_expression, 0, 0, 0, 0);
        }
        gcDoReturn_type_self = cType::InitializeType(0, 0, 0x77, gcDoReturn_type_action, gcDoReturn::New, 0, 0, 0);
    }
    return gcDoReturn_type_self;
}

int gcDoReturn::GetMaxChildren(void) const {
    return 1;
}

// 0x002fa118, 40B
void gcDoReturn::GetText(char *buf) const {
    cStrCat(buf, "return");
}

// 0x002fa17c, 240B
void gcDoReturn::SetChild(int, gcExpression *child) {
    int val = ((int *)this)[3];
    int a = 1;
    int tag = val & 1;
    if (tag != 0) a = 0;

    if (a != 0) {
        int b = 0;
        if (tag != 0) b = 1;
        int newVal;
        if (b != 0) {
            newVal = val & ~1;
            newVal |= 1;
        } else {
            newVal = *(int *)val;
            newVal |= 1;
        }
        val = newVal;
        ((int *)this)[3] = val;
    }

    if ((gcExpression *)val != child) {
        int c = 1;
        int tag2 = val & 1;
        if (tag2 != 0) c = 0;

        if (c != 0) {
            int d = 0;
            if (tag2 != 0) d = 1;
            int newVal2;
            if (d != 0) {
                newVal2 = val & ~1;
                newVal2 |= 1;
            } else {
                newVal2 = *(int *)val;
                newVal2 |= 1;
            }
            ((int *)this)[3] = newVal2;

            if (val != 0) {
                void *vt = *(void **)((char *)val + 4);
                DtorDeleteRecord *rec = (DtorDeleteRecord *)((char *)vt + 0x50);
                short off = rec->offset;
                rec->fn((char *)val + off, (void *)3);
            }
        }

        if (child != 0) {
            ((int *)this)[3] = (int)child;
        }
    }
}

struct EvalResult {
    int flags;
    float val;
};

struct DispatchEntry {
    short offset;
    short _pad;
    float (*func)(char *);
};

float gcDoReturn::Evaluate(void) const {
    int val = ((int *)this)[3];
    gcEvalCtx *ctx = g_gcEvalCtx;
    EvalResult *result = 0;
    int tag = val & 1;

    if (ctx != 0) {
        result = (EvalResult *)ctx->evalState;
    }

    int hasTag = 0;
    if (tag != 0) {
        hasTag = 1;
    }

    int hasChild;
    if (hasTag != 0) {
        hasChild = 0;
    } else {
        hasChild = (bool)val;
    }

    float f;
    if (hasChild != 0) {
        int isTagged = 0;
        if (tag != 0) {
            isTagged = 1;
        }
        DispatchEntry *de;
        char *ptr;
        if (isTagged != 0) {
            ptr = 0;
            de = (DispatchEntry *)(((char **)ptr)[1] + 0x70);
        } else {
            ptr = (char *)val;
            de = (DispatchEntry *)(((char **)ptr)[1] + 0x70);
        }
        short adj = de->offset;
        f = de->func(ptr + adj);
    } else {
        f = 0.0f;
    }

    result->val = f;
    result->flags |= 1;
    return result->val;
}

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, int);
    ~cReadBlock(void);
    void ReadBase(void *, int, int *);
};

void cFile_SetCurrentPos(void *, unsigned int);
void *cMemPool_GetPoolFromPtr(void *);
int gcAction_Read(void *, cFile &, cMemPool *);

int gcDoReturn::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) goto fail;
    if (!gcAction_Read(this, file, pool)) goto fail;
    {
        int val = ((int *)this)[3];
        int tag = val & 1;
        int flag = 0;
        if (tag != 0) goto tagged_ref;
        goto merge_ref;
    fail:
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    tagged_ref:
        flag = 1;
    merge_ref:;
        int ref;
        if (flag != 0) {
            ref = 0;
        } else {
            ref = val;
        }
        int sp20 = ref;

        int isTagged = 0;
        if (tag != 0) {
            isTagged = 1;
        }
        int parent;
        if (isTagged != 0) {
            parent = val & ~1;
        } else {
            parent = *(int *)val;
        }

        void *childPool = cMemPool_GetPoolFromPtr((char *)this + 12);
        rb.ReadBase(childPool, parent, &sp20);

        int newChild = sp20;
        if (newChild == 0) {
            newChild = parent | 1;
        }
        ((int *)this)[3] = newChild;
    }
    return result;
}
