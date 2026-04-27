#ifndef GCVALENTITYCONTROLLERVARIABLE_H
#define GCVALENTITYCONTROLLERVARIABLE_H

class cBase;
class cFile;
class cMemPool;

class gcValEntityControllerVariable {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
};

#endif
