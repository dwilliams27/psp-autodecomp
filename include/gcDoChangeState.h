#ifndef GCDOCHANGESTATE_H
#define GCDOCHANGESTATE_H

class cMemPool;
class cBase;
class cFile;
class cType;
class gcExpression;

class gcDoChangeState {
public:
    void AssignCopy(const cBase *);
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    gcDoChangeState &operator=(const gcDoChangeState &);
};

#endif
