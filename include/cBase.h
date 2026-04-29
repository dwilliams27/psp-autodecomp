#ifndef CBASE_H
#define CBASE_H

class cMemPool;
class cFile;
class cType;

class cBase {
public:
    const cType *GetType(void) const;
    void SetDirty(void);
    int IsEditable(void) const;
    void Reset(cMemPool *, bool);
    void GetName(char *) const;
    int GetProperties(void) const;
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcExpression {
public:
    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    void SetChild(int, gcExpression *);
    gcExpression *GetChild(int) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    unsigned int GetTextColor(void) const;
    gcExpression *GetNext(void) const;
    void SetNext(gcExpression *);
    int GetExprFlags(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    const char *GetTextSeparator(int) const;
    void GetTextRecursive(char *) const;
};

class gcAction : public gcExpression {
public:
    int GetExprFlags(void) const;
};

#endif
