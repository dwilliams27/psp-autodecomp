#ifndef GCDOUIFADE_H
#define GCDOUIFADE_H

class gcDoUIFade {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcDoUISendMessage {
public:
    int GetExprFlags(void) const;
    int GetMaxChildren(void) const;
};

class gcExpression;

class gcDoUIWidgetForEach {
public:
    char _pad[0x18];
    gcExpression *branch;

    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
};

class gcDoWhile {
public:
    char _pad[0x10];
    gcExpression *branch;

    int GetMaxChildren(void) const;
    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
};

#endif
