#ifndef GCVALFLYINGCONTROLLERVARIABLE_H
#define GCVALFLYINGCONTROLLERVARIABLE_H

class cBase;
class cFile;
class cMemPool;
class cType;

class gcValFlyingControllerVariable {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Set(float);
    void GetText(char *) const;
    const cType *GetType(void) const;
};

#endif
