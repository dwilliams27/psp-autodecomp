#ifndef GCDORETURN_H
#define GCDORETURN_H

class gcDoReturn {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
};

class gcDoSetValue {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
};

class gcDoStateFunction {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcDoStateParentHandler {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcDoSwitchCase {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    int GetExprFlags(void) const;
};

class gcValCaseRange {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
};

class gcDoSwitch {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
};

#endif
