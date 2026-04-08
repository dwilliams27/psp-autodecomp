#include "gcDoReturn.h"

void cStrCat(char *, const char *);

void gcDoReturn::GetText(char *buf) const {
    cStrCat(buf, "return");
}
