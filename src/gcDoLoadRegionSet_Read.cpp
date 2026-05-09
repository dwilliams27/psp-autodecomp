// ODR-WARNING: split-TU method addition for gcDoLoadRegionSet::Read.

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

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoLoadRegionSet {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                              bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" int gcAction_Read(void *, cFile &, cMemPool *)
    __asm__("__0fIgcActionEReadR6FcFileP6IcMemPool");

int gcDoLoadRegionSet::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char flag0;
    char flag1;

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] >= 6 || (unsigned int)rb[3] < 3 ||
        gcAction_Read(this, file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);

    if ((unsigned int)rb[3] >= 4) {
        ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x1C);
        char *base = (char *)this + 0x18;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x38);
        char *base = (char *)this + 0x34;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], &flag0, 1);
    unsigned int version = rb[3];
    *(unsigned char *)((char *)this + 0x50) = flag0 != 0;

    if (version >= 5) {
        cFileSystem::Read(*(cFileHandle **)rb[0], &flag1, 1);
        *(unsigned char *)((char *)this + 0x51) = flag1 != 0;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
