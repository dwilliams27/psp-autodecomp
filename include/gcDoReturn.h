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
};

#endif
