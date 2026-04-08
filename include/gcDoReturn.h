#ifndef GCDORETURN_H
#define GCDORETURN_H

class cMemPool;
class cBase;
class cFile;
class gcExpression;

class gcDoReturn {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
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
