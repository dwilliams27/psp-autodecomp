#ifndef GCVALSTATETIMER_H
#define GCVALSTATETIMER_H

class cBase;
class cFile;
class cMemPool;

class gcValStateTimer {
public:
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    float Evaluate(void) const;
    void Write(cFile &) const;
};

#endif
