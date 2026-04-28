#ifndef GCVALTRINARYOP_H
#define GCVALTRINARYOP_H

class cBase;
class cMemPool;
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
    static cBase *New(cMemPool *, cBase *);
};

#endif
