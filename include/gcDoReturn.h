#ifndef GCDORETURN_H
#define GCDORETURN_H

class cMemPool;
class cBase;
class cFile;
class cType;
class gcExpression;

class gcDoReturn {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void GetText(char *) const;
    void Write(cFile &) const;
    float Evaluate(void) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
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
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    void GetText(char *) const;
};

class gcDoStateParentHandler {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    void GetText(char *) const;
};

struct gcDoSwitchCaseData {
    char _pad[0x0C];
    gcExpression **mChildren;
    char _pad2[0x04];
    gcExpression *mBranch;
};

class gcDoSwitchCase {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
};

struct gcValCaseRangeData {
    char _pad[0x08];
    gcExpression **mChildren;
};

class gcValCaseRange {
public:
    int GetMaxChildren(void) const;
    int GetExprFlags(void) const;
    unsigned int GetTextColor(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    float Evaluate(void) const;
};

class gcDoSwitch {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
};

#endif
