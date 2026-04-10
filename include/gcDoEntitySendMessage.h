#ifndef GCDOENTITYSENDMESSAGE_H
#define GCDOENTITYSENDMESSAGE_H

class gcExpression;
class cBase;
class cFile;
class cMemPool;
class cType;

class gcDoEntitySendMessage {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

class gcDoEntitySendPartialControllerMessage {
public:
    gcDoEntitySendPartialControllerMessage(cBase *);
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
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
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    ~gcDoFunction(void);
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
