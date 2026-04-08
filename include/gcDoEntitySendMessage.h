#ifndef GCDOENTITYSENDMESSAGE_H
#define GCDOENTITYSENDMESSAGE_H

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
};

#endif
