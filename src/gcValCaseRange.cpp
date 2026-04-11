#include "gcDoReturn.h"
#include "cBase.h"

void cStrCat(char *, const char *);
gcValCaseRange *dcast(const cBase *);

struct DispatchEntry {
    short offset;
    short _pad;
    void (*func)();
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

void gcAction_Write(const gcDoReturn *, cFile &);

class cBaseArray {
public:
    cBase **mData;
    cBaseArray &operator=(const cBaseArray &);
};

unsigned int gcValCaseRange::GetTextColor(void) const {
    return 0xFF808000;
}

gcExpression *gcValCaseRange::GetChild(int index) const {
    gcValCaseRangeData *self = (gcValCaseRangeData *)this;
    return self->mChildren[index];
}

void gcValCaseRange::SetChild(int index, gcExpression *child) {
    gcValCaseRangeData *self = (gcValCaseRangeData *)this;
    self->mChildren[index] = child;
}

void gcValCaseRange::GetText(char *buf) const {
    cStrCat(buf, "case_range");
}

void gcValCaseRange::AssignCopy(const cBase *base) {
    gcValCaseRange *other = dcast(base);
    cBaseArray &src = *(cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(src);
}

extern float D_37D8C4;

float gcValCaseRange::Evaluate(void) const {
    char *child = (char *)(*(gcExpression ***)((char *)this + 8))[0];
    DispatchEntry *e = (DispatchEntry *)(((char **)child)[1] + 0x70);
    float (*fn)(char *) = (float (*)(char *))e->func;
    float result = fn(child + e->offset);

    if (D_37D8C4 < result) goto ret_result;

    child = (char *)(*(gcExpression ***)((char *)this + 8))[1];
    e = (DispatchEntry *)(((char **)child)[1] + 0x70);
    fn = (float (*)(char *))e->func;
    result = fn(child + e->offset);

    if (D_37D8C4 <= result) goto ret_threshold;

ret_result:
    return result;

ret_threshold:
    return D_37D8C4;
}

void gcDoReturn::Write(cFile &file) const {
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
    wb.End();
}
