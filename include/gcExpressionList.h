#ifndef GCEXPRESSIONLIST_H
#define GCEXPRESSIONLIST_H

class cBase;
class cWriteBlock;

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
    gcExpressionList &operator=(const gcExpressionList &);
    void DeleteExpressions(void);
    float Evaluate(void) const;
    void Write(cWriteBlock &) const;
};

#endif
