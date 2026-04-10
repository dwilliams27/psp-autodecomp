#include "gcDoReturn.h"
#include "cBase.h"

void cHandlePairT_GetName(void *, char *, int, int);
void cStrAppend(char *, const char *, ...);
int gcDesiredEventParams_GetText(void *, char *, int);
void cStrCat(char *, const char *);

extern const char gcDoStateFunction_fmt[];
extern const char gcDoStateFunction_str1[];
extern const char gcDoStateFunction_str2[];

struct gcDoStateFunctionData2 {
    char _pad[0x14];
    gcExpression **mChildren;
};

void gcDoStateFunction::GetText(char *buf) const {
    char localBuf[256];
    localBuf[0] = '\0';
    cHandlePairT_GetName((char *)this + 0x0C, localBuf, 0, 1);
    cStrAppend(buf, gcDoStateFunction_fmt, localBuf);
    int val = ((int *)this)[7];
    int isOwned = 0;
    if (val & 1) {
        isOwned = 1;
    }
    if (isOwned != 0) {
        isOwned = 0;
    } else {
        isOwned = (val != 0);
        isOwned &= 0xFF;
        isOwned = isOwned != 0;
    }
    if (isOwned != 0) {
        int flag = 0;
        if (val & 1) {
            flag = 1;
        }
        cBase *ptr;
        if (flag != 0) {
            ptr = 0;
        } else {
            ptr = (cBase *)val;
        }
        if (gcDesiredEventParams_GetText(ptr, buf, 0)) {
            gcDoStateFunctionData2 *self = (gcDoStateFunctionData2 *)this;
            gcExpression *child = self->mChildren[0];
            const char *str;
            if (child != 0) {
                str = gcDoStateFunction_str1;
            } else {
                str = gcDoStateFunction_str2;
            }
            cStrCat(buf, str);
        }
    }
}
