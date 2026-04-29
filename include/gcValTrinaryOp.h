#ifndef GCVALTRINARYOP_H
#define GCVALTRINARYOP_H

class cBase;
class cFile;
class cMemPool;
class cType;
class gcExpression;

struct gcValTrinaryOpData {
    char _pad[0x08];
    int mField08;
    int mOp;
    gcExpression **mChildren;
};

class gcValTrinaryOp {
public:
    gcValTrinaryOp(cBase *);
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    int GetExprFlags(void) const;
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
