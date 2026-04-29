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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A48;

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

const cType *ePath::GetType(void) const {
    if (D_00046A48 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&ePath::New;
        const char *kindName = (const char *)0x36CEA4;
        const char *kindDesc = (const char *)0x36CEAC;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory), "+r"(kindName),
                         "+r"(kindDesc));
        D_00046A48 = cType::InitializeType(0, 0, 0x18, parentType, factory,
                                           kindName, kindDesc, 0);
    }
    return D_00046A48;
}
#pragma control sched=2
