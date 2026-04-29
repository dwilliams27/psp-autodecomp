#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;

const cType *cBase::GetType(void) const {
    if (D_000385DC == 0) {
        D_000385DC = cType::InitializeType((const char *)0x36C830,
                                           (const char *)0x36C838,
                                           1, 0, 0, 0, 0, 0);
    }
    return D_000385DC;
}

int cBase::GetProperties(void) const {
    return 0;
}

void cBase::OnMemPoolReset(const cMemPool *, unsigned int) {
}

void cBase::Write(cFile &) const {
}

int cBase::Read(cFile &, cMemPool *) {
    return 1;
}

int gcExpression::GetMaxChildren(void) const {
    return 0;
}
