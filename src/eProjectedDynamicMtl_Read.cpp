// ODR-WARNING: this TU intentionally redeclares eProjectedDynamicMtl with a
// minimal member list because include/eTextureMap.h does not yet declare Read.
// Keep this split from src/eProjectedDynamicMtl.cpp so existing matches in the
// canonical TU are not perturbed.

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
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class eGeomMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eProjectedDynamicMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

int eProjectedDynamicMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] >= 3) goto fail;
    if ((unsigned int)rb._data[3] < 1) goto fail;
    if (((eGeomMtl *)this)->Read(file, pool)) goto success;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x48);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }
    if ((unsigned int)rb._data[3] >= 2) {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x68, 4);
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}
