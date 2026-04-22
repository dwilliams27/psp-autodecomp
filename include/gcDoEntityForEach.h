#ifndef GCDOENTITYFOREACH_H
#define GCDOENTITYFOREACH_H

class gcExpression;

class gcDoEntityForEach {
public:
    char _pad[0x2C];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
};

class cBase;
class cFile;

class gcDoEntityForEachAttached {
public:
    char _pad[0x64];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    void GetText(char *) const;
    void Write(cFile &) const;
    float Evaluate(void) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

#endif
