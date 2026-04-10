#include "gcValTrinaryOp.h"

gcValTrinaryOp *dcast(const cBase *);

class cBaseArray {
public:
    cBase **mData;
    cBaseArray &operator=(const cBaseArray &);
};

void gcValTrinaryOp::AssignCopy(const cBase *base) {
    gcValTrinaryOp *other = dcast(base);
    gcValTrinaryOpData *self = (gcValTrinaryOpData *)this;
    gcValTrinaryOpData *src = (gcValTrinaryOpData *)other;
    self->mField08 = src->mField08;
    self->mOp = src->mOp;
    ((cBaseArray *)((char *)this + 0x10))->operator=(*(cBaseArray *)((char *)other + 0x10));
}
