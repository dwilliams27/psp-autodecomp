#include "gcDoUIFade.h"
#include "gcExpressionList.h"
#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

extern "C" {
    void cStrCat(char *, const char *);
    void gcAction_gcAction(void *, cBase *);
    void gcAction_Write(const void *, cFile &);
    void gcExpressionList_Write(void *, cWriteBlock &);
    void gcExpressionList_ctor(void *, cBase *);
}

extern char gcDoWhilevirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

void gcDoWhile::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

gcExpression *gcDoWhile::GetChild(int index) const {
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

void gcDoWhile::GetText(char *buf) const {
    cStrCat(buf, "while");
}

gcDoWhile::gcDoWhile(cBase *parent) {
    gcAction_gcAction(this, parent);
    ((void **)this)[1] = gcDoWhilevirtualtable;
    ((int *)this)[3] = (int)this | 1;
    gcExpressionList_ctor((char *)this + 16, (cBase *)this);
}

cBase *gcDoWhile::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoWhile *result = 0;
    gcDoWhile *obj = (gcDoWhile *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoWhile(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcDoWhile::Write(cFile &file) const {
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
    gcExpressionList_Write((char *)this + 16, wb);
    wb.End();
}

int gcValBinaryOp::GetMaxChildren(void) const {
    return 2;
}

float gcValCameraFollowEntity3rdVariable::Evaluate(void) const {
    return 0.0f;
}

void gcValCameraFollowEntity3rdVariable::Set(float) {
}

float gcValCameraValue::Evaluate(void) const {
    return 0.0f;
}
