#ifndef GCDOUSEROP_H
#define GCDOUSEROP_H

class cMemPool;
class cBase;
class cFile;
class cType;

class gcDoUserOp {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

#endif
