// ODR-WARNING: split-TU local redeclaration for one method. Do not include this
// class definition from headers; it intentionally lists only methods referenced here.

class cBase;
class cFile;
class cMemPool;
struct cFileHandle;

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

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class eSpriteMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eStandardSpriteMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

int eStandardSpriteMtl::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 3, true);

    if ((unsigned int)rb._data[3] >= 4) goto fail;
    if ((unsigned int)rb._data[3] < 2) goto fail;
    int ok = ((eSpriteMtl *)this)->Read(file, pool);
    __asm__ volatile("" : "+r"(ok));

    cHandle *handle = (cHandle *)((char *)this + 0x48);
    if (ok == 0) goto fail;
    *(int *)((char *)this + 0x48) = 0;
    {
        handle->Read(rb, cMemPool::GetPoolFromPtr(handle));
    }
    {
        cFileSystem::Read(*(cFileHandle **)rb._data[0],
                          (char *)this + 0x64, 4);
    }
    {
        cBase *base;
        register cBase **basePtr __asm__("$7") = &base;
        base = *(cBase **)((char *)this + 0x68);
        __asm__ volatile("" : "+r"(basePtr));
        rb.ReadBase(pool, (cBase *)this, base);
        cBase *newBase = base;
        unsigned int version = rb._data[3];
        __asm__ volatile("" : "+r"(version));
        *(cBase **)((char *)this + 0x68) = newBase;
        if (version < 3) goto defaults;
    }

    {
        char flag;
        void *h = *(void **)rb._data[0];
        cFileSystem::Read((cFileHandle *)h, &flag, 1);
        void **fileObj = (void **)rb._data[0];
        *(unsigned char *)((char *)this + 0x6C) = flag != 0;
        __asm__ volatile("" : "+r"(fileObj));
        cFileSystem::Read((cFileHandle *)*fileObj, (char *)this + 0x70, 4);
        cFileSystem::Read(*(cFileHandle **)rb._data[0],
                          (char *)this + 0x74, 4);
        cFileSystem::Read(*(cFileHandle **)rb._data[0],
                          (char *)this + 0x78, 4);
        cFileSystem::Read(*(cFileHandle **)rb._data[0],
                          (char *)this + 0x7C, 4);
    }
    goto finish;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

defaults:
    {
        float zero = 0.0f;
        *(unsigned char *)((char *)this + 0x6C) = 0;
        *(float *)((char *)this + 0x70) = zero;
        *(float *)((char *)this + 0x74) = zero;
        *(float *)((char *)this + 0x78) = 256.0f;
        *(float *)((char *)this + 0x7C) = 256.0f;
    }

finish:
    PlatformRead(file, pool);
    CreateData();
    return result;
}

#pragma control sched=2
