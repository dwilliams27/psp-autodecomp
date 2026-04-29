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
    void SetSize(int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const char *);
    void WriteBase(const cBase *);
    void End(void);
};

struct gcDoLogData {
    char _pad[0x08];
    int mFlags;
    int mNumChildren;
    cArrayBase<char> mText;
    gcExpression **mChildren;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcDoLog *dcast(const cBase *);
void gcAction_gcAction(gcDoLog *, cBase *);
void gcAction_Write(const gcDoLog *, cFile &);
extern char gcDoLogvirtualtable[];

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

cBase *gcDoLog::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLog *result = 0;
    gcDoLog *obj = (gcDoLog *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLogvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = 0;
        ((int *)obj)[6] = (int)obj;
        ((cBaseArray *)((char *)obj + 0x14))->SetSize(3);
        result = obj;
        ((int *)obj)[2] = ((int *)obj)[2] | 1;
    }
    return (cBase *)result;
}

void gcDoLog::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    gcDoLogData *self = (gcDoLogData *)this;

    int s1 = 0;
    if (self->mText.mData != 0) {
        s1 = ((int *)self->mText.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(s1);

    int s2 = 0;
    if (self->mText.mData != 0) {
        s2 = ((int *)self->mText.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(s2, (const char *)self->mText.mData);

    wb.Write(self->mNumChildren);

    int i = 0;
    int offset = 0;
    do {
        wb.WriteBase(*(cBase **)((char *)self->mChildren + offset));
        i++;
        offset += 4;
    } while (i < 3);

    wb.End();
}
