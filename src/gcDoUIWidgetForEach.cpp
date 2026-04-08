#include "gcDoUIFade.h"

gcExpression *gcDoUIWidgetForEach::GetBranch(int) const {
    return branch;
}

void gcDoUIWidgetForEach::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

int gcDoWhile::GetMaxChildren(void) const {
    return 1;
}

int gcDoWhile::GetMaxBranches(void) const {
    return 1;
}

gcExpression *gcDoWhile::GetBranch(int) const {
    return branch;
}
