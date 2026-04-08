#include "gcDoEntitySendMessage.h"
#include "cBase.h"

template <class T>
class cArrayBase {
public:
    int mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cBaseArray {
public:
    cBase **mData;
    cBaseArray &operator=(const cBaseArray &);
};

struct gcDoLogData {
    char _pad[0x08];
    int mFlags;
    int mNumChildren;
    cArrayBase<char> mText;
    gcExpression **mChildren;
};

gcDoLog *dcast(const cBase *);

unsigned int gcDoLog::GetTextColor(void) const {
    return 0xFF008000;
}

gcExpression *gcDoLog::GetChild(int index) const {
    gcDoLogData *self = (gcDoLogData *)this;
    return self->mChildren[index];
}

void gcDoLog::SetChild(int index, gcExpression *child) {
    gcDoLogData *self = (gcDoLogData *)this;
    self->mChildren[index] = child;
}

int gcDoLog::GetExprFlags(void) const {
    int *p = (int *)this;
    int result = 8;
    if (p[3] != 0) {
        gcExpression **children = ((gcExpression ***)this)[5];
        int f = 8;
        if (children[0] != 0) {
            f = 0xA;
        }
        result = f;
    }
    return result;
}

void gcDoLog::AssignCopy(const cBase *base) {
    gcDoLogData *self = (gcDoLogData *)this;
    gcDoLog *other = dcast(base);
    gcDoLogData *src = (gcDoLogData *)other;

    self->mFlags = self->mFlags & ~3;
    self->mFlags = self->mFlags | (src->mFlags & 3);
    self->mNumChildren = src->mNumChildren;
    self->mText = src->mText;
    ((cBaseArray *)&self->mChildren) ->operator=( *(cBaseArray *)&src->mChildren );

    int f2 = self->mFlags;
    int n2 = 0;
    if (f2 & 1) {
        n2 = 1;
    }
    n2 = (unsigned char)n2;
    n2 = (unsigned char)n2;
    if (n2) {
        *(volatile int *)((char *)this + 8) = f2 | 1;
    } else {
        *(volatile int *)((char *)this + 8) = f2 & ~1;
    }
}
