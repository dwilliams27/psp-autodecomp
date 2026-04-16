#include "gcDoReturn.h"
#include "gcExpressionList.h"
#include "cBase.h"

int gcDoReturn::GetMaxChildren(void) const {
    return 1;
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
