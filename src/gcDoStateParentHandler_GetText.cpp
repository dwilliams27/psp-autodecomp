#include "gcDoReturn.h"
#include "cBase.h"

void cStrAppend(char *, const char *, ...);
int gcDesiredEventParams_GetText(void *, char *, int);
void cStrCat(char *, const char *);

extern const char gcDoStateParentHandler_fmt[];
extern const char gcDoStateParentHandler_str1[];
extern const char gcDoStateParentHandler_str2[];

struct gcDoStateParentHandlerData {
    char _pad[0x0C];
    gcExpression **mChildren;
};

void gcDoStateParentHandler::GetText(char *buf) const {
    cStrAppend(buf, gcDoStateParentHandler_fmt);
    int val = ((int *)this)[5];
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
            gcDoStateParentHandlerData *self = (gcDoStateParentHandlerData *)this;
            gcExpression *child = self->mChildren[0];
            const char *str;
            if (child != 0) {
                str = gcDoStateParentHandler_str1;
            } else {
                str = gcDoStateParentHandler_str2;
            }
            cStrCat(buf, str);
        }
    }
}
