#ifndef GCVALUNARYOP_H
#define GCVALUNARYOP_H

class cBase;
class cMemPool;
class cType;
class gcExpression;

class gcValUnaryOp {
public:
    gcValUnaryOp(cBase *parent);
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    int GetExprFlags(void) const;
    void GetText(char *) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
