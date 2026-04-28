#ifndef GCDOUSEROP_H
#define GCDOUSEROP_H

class cMemPool;
class cBase;
class cFile;

class gcDoUserOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

#endif
