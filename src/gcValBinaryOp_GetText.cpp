#include "gcDoUIFade.h"

void cStrCat(char *, const char *);

void gcValBinaryOp::GetText(char *buf) const {
    char *dest = buf;
    const char *src = "???";
    cStrCat(dest, src);
}
