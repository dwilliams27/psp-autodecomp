#include "gcExpressionList.h"

#define NULL 0

struct DispatchEntry {
    short offset;
    short _pad;
    void *(*func)(char *);
};

struct ExprNode {
    ExprNode *next;
    char *dispatch;
    int flags;
};

static inline bool testBit(int val, int mask) {
    return (val & mask) != 0;
}

float gcExpressionList::Evaluate(void) const {
    gcEvalCtx *ctx = g_gcEvalCtx;
    ExprNode *node = *(ExprNode **)(char *)this;
    char *savedExpr = ctx->currentExpr;

    if (node != NULL) {
        do {
            char *state = ctx->evalState;
            int flags = *(int *)state;
            int earlyReturn = 0;
            int skipEval = 0;

            if (flags & 1) {
                skipEval = 1;
            }

            if (skipEval != 0) goto setEarly;
            __asm__ volatile("" ::: "memory");
            {
                int raw = flags & 2;
                unsigned char bit2 = (unsigned char)(raw != 0);
                if (bit2) goto setEarly;
            }
            {
                bool check = (bool)earlyReturn;
                if (!check) goto doWork;
            }
            goto earlyExit;
        setEarly:
            earlyReturn = 1;
            {
                bool check = (bool)earlyReturn;
                if (!check) goto doWork;
            }
        earlyExit:
            ctx->currentExpr = savedExpr;
            goto done;
        doWork:

            ctx->currentExpr = (char *)node;

            int dflags = node->flags;
            int skipCall = 0;
            char *vt = node->dispatch;
            if (dflags & 1) {
                skipCall = 1;
            }

            if (skipCall == 0) {
                DispatchEntry *de = (DispatchEntry *)(vt + 0x70);
                short adj = de->offset;
                de->func((char *)node + adj);
                vt = node->dispatch;
            }

            {
                DispatchEntry *de = (DispatchEntry *)(vt + 0xA8);
                short adj = de->offset;
                node = (ExprNode *)de->func((char *)node + adj);
            }
        } while (node != NULL);
    }

    ctx->currentExpr = savedExpr;
done:
    return ((float *)ctx->evalState)[1];
}
