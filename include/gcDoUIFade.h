#ifndef GCDOUIFADE_H
#define GCDOUIFADE_H

class cMemPool;
class cBase;
class cFile;
class cType;
class gcExpression;

class gcDoUIFade {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    gcDoUIFade &operator=(const gcDoUIFade &);
};

class gcDoUISendMessage {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

class gcExpression;

class gcDoUIWidgetForEach {
public:
    char _pad[0x18];
    gcExpression *branch;

    ~gcDoUIWidgetForEach(void);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    void GetText(char *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcDoWhile {
public:
    char _pad[0x10];
    gcExpression *branch;

    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
};

struct gcValBinaryOpData {
    char _pad[0x08];
    int mOp;
    gcExpression **mChildren;
};

class gcValBinaryOp {
public:
    gcValBinaryOp(cBase *);
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    int GetExprFlags(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

class gcValCameraFollowEntity3rdVariable {
public:
    float Evaluate(void) const;
    void Set(float);
};

class gcValCameraValue {
public:
    float Evaluate(void) const;
};

#endif
