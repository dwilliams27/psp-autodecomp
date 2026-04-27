#ifndef GCVALBIPEDCONTROLLERVARIABLE_H
#define GCVALBIPEDCONTROLLERVARIABLE_H

class cBase;
class cFile;
class cMemPool;

class gcValBipedControllerVariable {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
};

#endif
