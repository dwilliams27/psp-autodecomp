#include "cBase.h"
#include "cFile.h"

void cStrCat(char *, const char *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

typedef const void *(*gcExprVFn)(const void *);
struct gcExprVEntry {
    short adj;
    short pad;
    gcExprVFn fn;
};

void gcExpression::GetText(char *buf) const {
    const gcExprVEntry *vt = *(const gcExprVEntry **)((char *)this + 4);
    const gcExprVEntry *e = vt + 1;
    const void *r = e->fn((const char *)this + e->adj);
    cStrCat(buf, *(const char **)((char *)r + 0xC));
}

typedef int (*gcExprVFnFlags)(const void *);
typedef int (*gcExprFnMaxChildren)(const void *);
typedef void (*gcExprFnGetText)(const void *, char *);
typedef gcExpression *(*gcExprFnGetChild)(const void *, int);
typedef const char *(*gcExprFnGetSep)(const void *, int);

extern const char gcExpression_lparen[];
extern const char gcExpression_space[];
extern const char gcExpression_rparenA[];
extern const char gcExpression_rparenB[];

void gcExpression::GetTextRecursive(char *buf) const {
    const gcExprVEntry *vt = *(const gcExprVEntry **)((char *)this + 4);

    const gcExprVEntry *eFlags = vt + 23;
    int flags = ((gcExprVFnFlags)eFlags->fn)((const char *)this + eFlags->adj);
    int hasParen = 0;
    if (flags & 1) {
        hasParen = 1;
    }

    const gcExprVEntry *eMax = vt + 15;
    int noFlag4 = ((flags & 4) == 0);
    int hasBit2 = (flags & 2) != 0;
    int hasBit8 = (flags & 8) != 0;
    int hasBit10 = (flags & 0x10) != 0;
    int maxChildren = ((gcExprFnMaxChildren)eMax->fn)((const char *)this + eMax->adj);

    if (noFlag4 && (maxChildren != 0)) {
        if (hasBit8) {
            const gcExprVEntry *eText = vt + 25;
            ((gcExprFnGetText)eText->fn)((const char *)this + eText->adj, buf);
        }
        int i;
        if (hasParen) {
            cStrCat(buf, gcExpression_lparen);
            i = 0;
        } else {
            i = 0;
            if (hasBit10) {
                cStrCat(buf, gcExpression_space);
                i = 0;
            }
        }
        int childCount = 0;
        if (maxChildren > 0) {
            do {
                const gcExprVEntry *eChild = vt + 18;
                gcExpression *child = ((gcExprFnGetChild)eChild->fn)((const char *)this + eChild->adj, i);
                int skip = hasBit8 | i;
                if (child != 0) {
                    const gcExprVEntry *eSep = vt + 24;
                    const char *sep = ((gcExprFnGetSep)eSep->fn)((const char *)this + eSep->adj, i);
                    cStrCat(buf, sep);
                    const gcExprVEntry *childVt = *(const gcExprVEntry **)((char *)child + 4);
                    const gcExprVEntry *eRec = childVt + 26;
                    ((gcExprFnGetText)eRec->fn)((const char *)child + eRec->adj, buf);
                    childCount++;
                    skip = hasBit8 | i;
                }
                if (skip == 0) {
                    cStrCat(buf, gcExpression_space);
                    const gcExprVEntry *eText = vt + 25;
                    ((gcExprFnGetText)eText->fn)((const char *)this + eText->adj, buf);
                    cStrCat(buf, gcExpression_space);
                }
                i++;
            } while (i < maxChildren);
        }
        if (hasBit2) {
            const char *suffix;
            if (childCount > 0) {
                suffix = gcExpression_rparenA;
            } else {
                suffix = gcExpression_rparenB;
            }
            cStrCat(buf, suffix);
        }
    } else {
        const gcExprVEntry *eText = vt + 25;
        ((gcExprFnGetText)eText->fn)((const char *)this + eText->adj, buf);
    }
}

extern const char gcExpression_sepTab[];
extern const char gcExpression_sepSpace[];

const char *gcExpression::GetTextSeparator(int n) const {
    const gcExprVEntry *vt = *(const gcExprVEntry **)((char *)this + 4);
    const gcExprVEntry *e = vt + 23;
    int flags = ((gcExprVFnFlags)e->fn)((const char *)this + e->adj);
    const char *result;
    if ((flags & 8) && (n > 0)) {
        result = gcExpression_sepTab;
    } else {
        result = gcExpression_sepSpace;
    }
    return result;
}

void gcExpression::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

int gcExpression::Read(cFile &file, cMemPool *) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    return result;
}

int gcExpression::GetMaxBranches(void) const {
    return 0;
}

void gcExpression::SetChild(int, gcExpression *) {
}

gcExpression *gcExpression::GetChild(int) const {
    return 0;
}

gcExpression *gcExpression::GetBranch(int) const {
    return 0;
}

void gcExpression::SetBranch(int, gcExpression *) {
}

unsigned int gcExpression::GetTextColor(void) const {
    return 0xFF000000;
}

gcExpression *gcExpression::GetNext(void) const {
    return 0;
}

void gcExpression::SetNext(gcExpression *) {
}

int gcExpression::GetExprFlags(void) const {
    return 0;
}

int gcAction::GetExprFlags(void) const {
    return 0xB;
}
