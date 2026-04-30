#include "gcExpressionList.h"

#define NULL 0

class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

struct DispatchEntry {
    short offset;
    short _pad;
    void *(*func)(char *);
};

struct ExprNode {
    int _pad0;
    char *dispatch;
    int flags;
};

struct CloneEntry {
    short offset;
    short _pad;
    ExprNode *(*func)(void *, cMemPool *, cBase *);
};

struct LinkEntry {
    short offset;
    short _pad;
    void (*func)(void *, ExprNode *);
};

static inline bool testBit(int val, int mask) {
    return (val & mask) != 0;
}

gcExpressionList &gcExpressionList::operator=(const gcExpressionList &other) {
    DeleteExpressions();

    if (*(ExprNode **)(char *)&other != NULL) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        ExprNode *src = *(ExprNode **)(char *)&other;
        CloneEntry *clone = (CloneEntry *)(src->dispatch + 0x10);
        *(ExprNode **)(char *)this =
            clone->func((char *)src + clone->offset, pool,
                        *(cBase **)((char *)this + 4));

        src = *(ExprNode **)(char *)&other;
        DispatchEntry *headNext = (DispatchEntry *)(src->dispatch + 0xA8);
        src = (ExprNode *)headNext->func((char *)src + headNext->offset);
        ExprNode *dst = *(ExprNode **)(char *)this;
        if (dst != NULL) {
            do {
                if (src != NULL) {
                    LinkEntry *link = (LinkEntry *)(dst->dispatch + 0xB0);
                    char *target = (char *)dst + link->offset;
                    CloneEntry *srcClone = (CloneEntry *)(src->dispatch + 0x10);
                    link->func(target,
                               srcClone->func((char *)src + srcClone->offset,
                                              pool,
                                              *(cBase **)((char *)this + 4)));

                    DispatchEntry *srcNext =
                        (DispatchEntry *)(src->dispatch + 0xA8);
                    src =
                        (ExprNode *)srcNext->func((char *)src + srcNext->offset);
                } else {
                    LinkEntry *link = (LinkEntry *)(dst->dispatch + 0xB0);
                    link->func((char *)dst + link->offset, NULL);
                }

                DispatchEntry *dstNext = (DispatchEntry *)(dst->dispatch + 0xA8);
                dst = (ExprNode *)dstNext->func((char *)dst + dstNext->offset);
            } while (dst != NULL);
        }
    }

    return *this;
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

void gcExpressionList::Write(cWriteBlock &wb) const {
    cWriteBlock wb2(*wb._file, 1);
    int count = 0;

    for (ExprNode *node = *(ExprNode **)(char *)this; node != NULL;) {
        char *dispatch = node->dispatch;
        DispatchEntry *next = (DispatchEntry *)(dispatch + 0xA8);
        count += 1;
        node = (ExprNode *)next->func((char *)node + next->offset);
    }

    wb2.Write(count);

    for (ExprNode *node = *(ExprNode **)(char *)this; node != NULL;) {
        wb2.WriteBase((cBase *)node);

        char *dispatch = node->dispatch;
        DispatchEntry *next = (DispatchEntry *)(dispatch + 0xA8);
        node = (ExprNode *)next->func((char *)node + next->offset);
    }

    wb2.End();
}
