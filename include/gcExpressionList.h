#ifndef GCEXPRESSIONLIST_H
#define GCEXPRESSIONLIST_H

class cBase;

struct gcEvalCtx {
    char _pad0[4];
    char *currentExpr;  // 0x04
    char _pad8[8];
    char *evalState;    // 0x10
};

extern gcEvalCtx *g_gcEvalCtx;

class gcExpressionList {
public:
    gcExpressionList(cBase *);
    float Evaluate(void) const;
};

#endif
