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

class gcDoEntityForEachAttached {
public:
    char _pad[0x64];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
};

#endif
