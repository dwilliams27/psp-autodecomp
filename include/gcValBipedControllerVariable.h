#ifndef GCVALBIPEDCONTROLLERVARIABLE_H
#define GCVALBIPEDCONTROLLERVARIABLE_H

class cBase;
class cFile;
class cMemPool;
class cType;

class gcValBipedControllerVariable {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
