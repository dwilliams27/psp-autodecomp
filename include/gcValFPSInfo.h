#ifndef GCVALFPSINFO_H
#define GCVALFPSINFO_H

class cBase;
class cType;

class gcValFPSInfo {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
    void AssignCopy(const cBase *);
    void GetText(char *) const;
    float Evaluate(void) const;
    const cType *GetType(void) const;

    int pad0;
    int pad4;
    float f8;
};

#endif
