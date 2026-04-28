#include "ePath.h"
#include "cObject.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

extern "C" void ePathPoint_Write(const ePathPoint *, cWriteBlock &) asm("__0fKePathPointFWriteR6LcWriteBlockK");

#pragma control sched=1
void ePath::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cObject *)this)->Write(file);

    ePathPoint *points = mPoints;
    int count = 0;
    if (points != 0) {
        count = *(int *)((char *)points - 4) & 0x3FFFFFFF;
    }
    wb.Write(count);

    points = mPoints;
    int limit = 0;
    if (points != 0) {
        limit = *(int *)((char *)points - 4) & 0x3FFFFFFF;
    }
    int i = 0;
    if (i < limit) {
        int offset = 0;
        do {
            i++;
            ePathPoint *point = (ePathPoint *)((char *)points + offset);
            ePathPoint_Write(point, wb);
            offset += 0x20;
        } while (i < limit);
    }

    wb.Write(mTotalLength);
    wb.End();
}

void ePath::AssignCopy(const cBase *base) {
    ePath *other = dcast<ePath>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cArrayBase<ePathPoint> *)((char *)this + 0x44))->operator=(
        *(const cArrayBase<ePathPoint> *)((char *)other + 0x44));
    mTotalLength = other->mTotalLength;
}

ePath *ePath::New(cMemPool *pool, cBase *parent) {
    ePath *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    ePath *obj = (ePath *)rec->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        new (obj) ePath(parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2
