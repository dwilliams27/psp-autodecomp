#ifndef GCVALUNARYOP_H
#define GCVALUNARYOP_H

class cBase;
class cMemPool;
class gcExpression;

class gcValUnaryOp {
public:
    gcValUnaryOp(cBase *parent);
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    int GetExprFlags(void) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
