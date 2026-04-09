#ifndef GCDOENTITYSENDMESSAGE_H
#define GCDOENTITYSENDMESSAGE_H

class gcExpression;
class cBase;

class gcDoEntitySendMessage {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcDoEntitySendPartialControllerMessage {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcDoEvaluation {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
};

class gcDoFunction {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcDoLog {
public:
    float Evaluate(void) const;
    int GetMaxChildren(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    int GetExprFlags(void) const;
    void AssignCopy(const cBase *);
};

#endif
