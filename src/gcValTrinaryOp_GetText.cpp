#include "gcValTrinaryOp.h"

void cStrCat(char *, const char *);

void gcValTrinaryOp::GetText(char *buf) const {
    const char *text = "?";
    char *dest = buf;
    cStrCat(dest, text);
}
