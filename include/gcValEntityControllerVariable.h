#ifndef GCVALENTITYCONTROLLERVARIABLE_H
#define GCVALENTITYCONTROLLERVARIABLE_H

class cBase;
class cFile;
class cMemPool;

class gcValEntityControllerVariable {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
    const class cType *GetType(void) const;
};

#endif
