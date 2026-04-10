#ifndef GCVALTRINARYOP_H
#define GCVALTRINARYOP_H

class cBase;
class gcExpression;

struct gcValTrinaryOpData {
    char _pad[0x08];
    int mField08;
    int mOp;
    gcExpression **mChildren;
};

class gcValTrinaryOp {
public:
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    int GetExprFlags(void) const;
    void GetText(char *) const;
    void AssignCopy(const cBase *);
};

#endif
