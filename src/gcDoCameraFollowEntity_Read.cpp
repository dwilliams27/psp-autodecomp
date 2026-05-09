// ODR-WARNING: this TU intentionally redeclares gcDoCameraFollowEntity with a
// minimal member list so adding Read does not perturb codegen for matched
// siblings in gcDoCameraFollowEntity.cpp.
#include "cBase.h"

class cFileHandle;
class cReadBlock;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

struct ReadRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoCameraFollowEntity : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

int gcDoCameraFollowEntity::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    int inner[5];
    char flag;
    int ok;

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1) {
        goto fail_outer;
    }

    __0oKcReadBlockctR6FcFileUib(inner, file, 1, true);
    if (inner[3] != 1) {
        goto fail_inner;
    }
    if (((gcAction *)this)->Read(file, pool) != 0) {
        goto read_inner;
    }

fail_inner:
    cFile_SetCurrentPos(*(void **)&inner[0], inner[1]);
    __0oKcReadBlockdtv(inner, 2);
    ok = 0;
    goto after_inner;

read_inner:
    {
        char *typeInfo0 = *(char **)((char *)this + 0x10);
        char *base0 = (char *)this + 0x0C;
        ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
        short off0 = rec0->offset;
        cFile *f0 = *(cFile **)&inner[0];
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    }

    ((gcDesiredValue *)((char *)this + 0x44))->Read(*(cReadBlock *)inner);

    cFileSystem::Read(*(cFileHandle **)inner[0], &flag, 1);
    *(unsigned char *)((char *)this + 0x48) = flag != 0;

    __0oKcReadBlockdtv(inner, 2);
    ok = 1;

after_inner:
    if (ok != 0) {
        goto read_outer;
    }

fail_outer:
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;

read_outer:
    {
        char *typeInfo1 = *(char **)((char *)this + 0x50);
        char *base1 = (char *)this + 0x4C;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    if ((unsigned int)rb[3] >= 2) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x78, 4);
    } else {
        *(int *)((char *)this + 0x78) = 1;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
