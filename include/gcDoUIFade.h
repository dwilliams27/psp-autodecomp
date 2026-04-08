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

class gcDoUIWidgetForEach {
public:
    int GetMaxBranches(void) const;
};

#endif
