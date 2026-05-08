// ODR-WARNING: this TU intentionally redeclares eOnePassModelMtl with a
// minimal member list because include/eTextureMap.h does not yet declare Read.
// Keep this split from src/eOnePassModelMtl.cpp so existing matches in the
// canonical TU are not perturbed.

class cBase;
class cFile;
class cFileHandle;
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
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class eModelMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eOnePassModelMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

int eOnePassModelMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 5, true);
    if ((unsigned int)rb._data[3] == 5 && ((eModelMtl *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x48);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x74, 4);
    }
    {
        cBase *base = *(cBase **)((char *)this + 0x78);
        rb.ReadBase(pool, (cBase *)this, base);
        *(cBase **)((char *)this + 0x78) = base;
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}
