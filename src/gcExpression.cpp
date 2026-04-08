#include "cBase.h"

int gcExpression::GetMaxBranches(void) const {
    return 0;
}

void gcExpression::SetChild(int, gcExpression *) {
}

gcExpression *gcExpression::GetChild(int) const {
    return 0;
}

gcExpression *gcExpression::GetBranch(int) const {
    return 0;
}

void gcExpression::SetBranch(int, gcExpression *) {
}
