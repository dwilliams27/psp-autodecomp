#ifndef GCVALNUMBER_H
#define GCVALNUMBER_H

class cBase;
class cFile;

class gcValNumber {
public:
    gcValNumber(cBase *, float);
    ~gcValNumber(void);
    void Write(cFile &) const;
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    float Evaluate(void) const;

    int pad0;
    int pad4;
    float mValue;
};

#endif
