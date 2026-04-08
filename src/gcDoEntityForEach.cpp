#include "gcDoEntityForEach.h"

gcExpression *gcDoEntityForEach::GetBranch(int) const {
    return branch;
}

void gcDoEntityForEach::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

int gcDoEntityForEachAttached::GetMaxBranches(void) const {
    return 1;
}

gcExpression *gcDoEntityForEachAttached::GetBranch(int) const {
    return branch;
}

void gcDoEntityForEachAttached::SetBranch(int, gcExpression *expr) {
    branch = expr;
}
