// ODR-WARNING: split-TU method addition for gcDoCameraOp::Read.

class cFileHandle;
class cMemPool;
class cReadBlock;

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoCameraOp {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                              bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" int gcAction_Read(void *, cFile &, cMemPool *)
    __asm__("__0fIgcActionEReadR6FcFileP6IcMemPool");

int gcDoCameraOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char flag;

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1 ||
        gcAction_Read(this, file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    {
        char *typeInfo0 = *(char **)((char *)this + 0x10);
        char *base0 = (char *)this + 0x0C;
        ReadRec *rec0 = (ReadRec *)(typeInfo0 + 0x30);
        short off0 = rec0->offset;
        cFile *f0 = *(cFile **)&rb[0];
        rec0->fn(base0 + off0, f0, cMemPool::GetPoolFromPtr(base0));
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x44, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], &flag, 1);
    *(unsigned char *)((char *)this + 0x48) = flag != 0;

    ((gcDesiredValue *)((char *)this + 0x4C))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x50))->Read(*(cReadBlock *)rb);

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x54, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x58, 4);

    cHandle *handle = (cHandle *)((char *)this + 0x5C);
    *(int *)((char *)this + 0x5C) = 0;
    handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));

    if ((unsigned int)rb[3] >= 2) {
        char *typeInfo1 = *(char **)((char *)this + 0x64);
        char *base1 = (char *)this + 0x60;
        ReadRec *rec1 = (ReadRec *)(typeInfo1 + 0x30);
        short off1 = rec1->offset;
        cFile *f1 = *(cFile **)&rb[0];
        rec1->fn(base1 + off1, f1, cMemPool::GetPoolFromPtr(base1));
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
