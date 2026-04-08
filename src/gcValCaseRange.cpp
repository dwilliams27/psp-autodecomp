#include "gcDoReturn.h"

void cStrCat(char *, const char *);
gcValCaseRange *dcast(const cBase *);

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
