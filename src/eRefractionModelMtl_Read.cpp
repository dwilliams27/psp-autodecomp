// ODR-WARNING: this split TU intentionally redeclares only the methods needed
// for eRefractionModelMtl::Read. Do not add Read to include/eTextureMap.h just
// for this method; shared header changes can perturb matched sibling methods.

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

class eModelMtl {
public:
    int Read(cFile &, cMemPool *);
};

struct eRefractionModelMtl_ReadEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *, cMemPool *);
};

class eRefractionModelMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

int eRefractionModelMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 3, true);
    if ((unsigned int)rb._data[3] != 3) goto fail;
    if (((eModelMtl *)this)->Read(file, pool)) goto success;

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
    *(int *)((char *)this + 0x4C) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x4C);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }
    {
        void *desc = *(void **)((char *)this + 0x80);
        eRefractionModelMtl_ReadEntry *entry =
            (eRefractionModelMtl_ReadEntry *)((char *)desc + 0x30);
        short off = entry->offset;
        char *base = (char *)this + 0x7C;
        void *target = base + off;
        void *f = *(void **)&rb._data[0];
        cMemPool *targetPool = cMemPool::GetPoolFromPtr(base);
        entry->fn(target, f, targetPool);
    }
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x74, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x78, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0xF0, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0xF4, 4);
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}
