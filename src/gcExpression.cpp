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

unsigned int gcExpression::GetTextColor(void) const {
    return 0xFF000000;
}

gcExpression *gcExpression::GetNext(void) const {
    return 0;
}

void gcExpression::SetNext(gcExpression *) {
}

int gcExpression::GetExprFlags(void) const {
    return 0;
}

int gcAction::GetExprFlags(void) const {
    return 0xB;
}
