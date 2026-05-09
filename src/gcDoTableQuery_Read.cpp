// ODR-WARNING: this TU intentionally redeclares gcDoTableQuery with a minimal
// member list so adding Read does not perturb codegen for matched siblings in
// gcDoTableQuery.cpp.
#include "cBase.h"

class cFileHandle;
class cReadBlock;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cBaseArray {
public:
    void Read(cReadBlock &);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

struct ReadRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct DesiredReadBase {
    int pad;
    char *typeInfo;
};

class gcDoTableQuery : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

int gcDoTableQuery::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] >= 4 || (unsigned int)rb[3] < 1 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    if ((unsigned int)rb[3] >= 3) {
        char *base0 = (char *)this + 0x0C;
        char *typeInfo0 = ((DesiredReadBase *)base0)->typeInfo;
        register ReadRec *rec0 __asm__("$17") = (ReadRec *)(typeInfo0 + 0x30);
        register cFile *f0 __asm__("$16") = *(cFile **)&rb[0];
        short off0 = rec0->offset;
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    } else {
        *(int *)((char *)this + 0x18) = 7;
        cHandle *handle0 = (cHandle *)((char *)this + 0x1C);
        *(int *)((char *)this + 0x1C) = 0;
        handle0->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle0));
    }

    ((cBaseArray *)((char *)this + 0x20))->Read(*(cReadBlock *)rb);
    ((cBaseArray *)((char *)this + 0x28))->Read(*(cReadBlock *)rb);

    {
        char *typeInfo1 = *(char **)((char *)this + 0x34);
        char *base1 = (char *)this + 0x30;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    {
        char *typeInfo2 = *(char **)((char *)this + 0x50);
        char *base2 = (char *)this + 0x4C;
        ReadRec *rec2 = (ReadRec *)(typeInfo2 + 0x30);
        short off2 = rec2->offset;
        cFile *f2 = *(cFile **)&rb[0];
        rec2->fn(base2 + off2, f2, cMemPool::GetPoolFromPtr(base2));
    }

    if ((unsigned int)rb[3] >= 2) {
        char *typeInfo3 = *(char **)((char *)this + 0x6C);
        char *base3 = (char *)this + 0x68;
        ReadRec *rec3 = (ReadRec *)(typeInfo3 + 0x30);
        short off3 = rec3->offset;
        cFile *f3 = *(cFile **)&rb[0];
        rec3->fn(base3 + off3, f3, cMemPool::GetPoolFromPtr(base3));
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
