#include "gcDoReturn.h"

int gcDoSwitchCase::GetMaxChildren(void) const {
    return 3;
}

int gcDoSwitchCase::GetExprFlags(void) const {
    return 0x18;
}

int gcValCaseRange::GetMaxChildren(void) const {
    return 2;
}

int gcValCaseRange::GetExprFlags(void) const {
    return 3;
}

int gcDoSwitch::GetMaxChildren(void) const {
    return 1;
}

int gcDoSwitch::GetMaxBranches(void) const {
    return 2;
}
