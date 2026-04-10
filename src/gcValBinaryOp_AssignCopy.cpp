#include "gcDoUIFade.h"

gcValBinaryOp *dcast(const cBase *);

class cBaseArray {
public:
    cBase **mData;
    cBaseArray &operator=(const cBaseArray &);
};

void gcValBinaryOp::AssignCopy(const cBase *base) {
    gcValBinaryOp *other = dcast(base);
    gcValBinaryOpData *self = (gcValBinaryOpData *)this;
    gcValBinaryOpData *src = (gcValBinaryOpData *)other;
    self->mOp = src->mOp;
    ((cBaseArray *)((char *)this + 0x0C))->operator=(*(cBaseArray *)((char *)other + 0x0C));
}
