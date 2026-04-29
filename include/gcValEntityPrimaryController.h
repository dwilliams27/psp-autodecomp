#ifndef GCVALENTITYPRIMARYCONTROLLER_H
#define GCVALENTITYPRIMARYCONTROLLER_H

class cBase;
class cFile;
class cMemPool;

class gcValEntityPrimaryController {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
    float Evaluate(void) const;
};

#endif
