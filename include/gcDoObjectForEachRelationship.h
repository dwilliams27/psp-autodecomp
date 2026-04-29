#ifndef GCDOOBJECTFOREACHRELATIONSHIP_H
#define GCDOOBJECTFOREACHRELATIONSHIP_H

class gcExpression;
class cBase;
class cMemPool;

class gcDoObjectForEachRelationship {
public:
    char _pad[0x28];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
};

#endif
