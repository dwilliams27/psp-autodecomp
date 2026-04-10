#ifndef GCDOPLAYCINEMATIC_H
#define GCDOPLAYCINEMATIC_H

class cMemPool;
class cBase;
class cFile;
class cType;
class gcExpression;

class gcDoPlayCinematic {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
    gcExpression *GetChild(int) const;
    void SetChild(int, gcExpression *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

#endif
