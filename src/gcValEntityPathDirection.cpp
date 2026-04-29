// gcValEntityPathDirection - gcAll_psp.obj
//   0x00338d0c AssignCopy(const cBase *)

#include "cBase.h"

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityPathDirection {
public:
    void AssignCopy(const cBase *);
};

template <class T> T *dcast(const cBase *);

void gcValEntityPathDirection::AssignCopy(const cBase *base) {
    gcValEntityPathDirection *other = dcast<gcValEntityPathDirection>(base);
    char *src = (char *)other;
    const gcDesiredEntity *srcEntity = (const gcDesiredEntity *)(src + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcEntity);
    other = (gcValEntityPathDirection *)src;
    *(int *)((char *)this + 0x34) = *(const int *)((char *)other + 0x34);
    *(int *)((char *)this + 0x38) = *(const int *)((char *)other + 0x38);
    *(int *)((char *)this + 0x3C) = *(const int *)((char *)other + 0x3C);
}
