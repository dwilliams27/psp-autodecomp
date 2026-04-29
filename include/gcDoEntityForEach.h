#ifndef GCDOENTITYFOREACH_H
#define GCDOENTITYFOREACH_H

class gcExpression;
class cBase;
class cFile;
class cMemPool;
class cType;

class gcDoEntityForEach {
public:
    char _pad[0x2C];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntityFindAttachedBase {
public:
    void GetText(char *, char *, bool) const;
    int Read(cFile &, cMemPool *);
};

class gcDoEntityForEachAttached : public gcDoEntityFindAttachedBase {
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
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

#endif
