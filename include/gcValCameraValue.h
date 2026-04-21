#ifndef GCVALCAMERAVALUE_H
#define GCVALCAMERAVALUE_H

class cBase;
class cFile;
class cMemPool;

class gcValCameraValue {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    void Write(cFile &) const;
    float Evaluate(void) const;
};

#endif
