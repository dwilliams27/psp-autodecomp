// ODR-WARNING: this TU intentionally redeclares eProjectedModelMtl with a
// minimal member list because include/eTextureMap.h does not yet declare Read.
// Keep this split from src/eProjectedModelMtl.cpp so existing matches in the
// canonical TU are not perturbed.

class cFile;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class eModelMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eProjectedModelMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

int eProjectedModelMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eModelMtl *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x48);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }
    *(int *)((char *)this + 0x4C) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x4C);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}
