#ifndef GCDOUSEROP_H
#define GCDOUSEROP_H

class cMemPool;
class cBase;
class cFile;
class cType;

class gcDoUserOp {
public:
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
    ~gcDoUserOp(void);
    void Write(cFile &) const;
};

#endif
