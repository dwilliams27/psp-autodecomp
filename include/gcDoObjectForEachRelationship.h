#ifndef GCDOOBJECTFOREACHRELATIONSHIP_H
#define GCDOOBJECTFOREACHRELATIONSHIP_H

class gcExpression;
class cBase;
class cFile;
class cMemPool;
class cType;

class gcDoObjectForEachRelationship {
public:
    char _pad[0x28];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    gcDoObjectForEachRelationship &
    operator=(const gcDoObjectForEachRelationship &);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

#endif
