#include "gcDoEntitySendMessage.h"
#include "cBase.h"

class cFile;
class cMemPool;

extern "C" {
    void cStrCat(char *, const char *);
    void gcAction_gcAction(void *, cBase *);
    void gcAction_Write(const void *, cFile &);
    void gcExpressionList_gcExpressionList(void *, cBase *);
    float gcExpressionList_Evaluate(const void *);
    void gcExpressionList_Write(const void *, void *);
}

extern char gcDoEvaluationvirtualtable[];

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

typedef float (*gcExprVFn)(const void *);
struct gcExprVEntry {
    short adj;
    short pad;
    gcExprVFn fn;
};


inline void *operator new(unsigned int, void *p) { return p; }

int gcDoEvaluation::GetMaxBranches(void) const {
    return 2;
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
