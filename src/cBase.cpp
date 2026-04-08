#include "cBase.h"

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
