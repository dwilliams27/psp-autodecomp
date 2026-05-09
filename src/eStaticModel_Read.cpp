// ODR-WARNING: this TU intentionally redeclares eStaticModel with a minimal
// member list because include/eStaticModel.h does not yet declare Read. Keep
// this split from src/eStaticModel.cpp so existing matches in the canonical TU
// are not perturbed.

class cBase;
class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eStaticGeom {
public:
    int Read(cFile &, cMemPool *);
};

class eStaticModel {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
};

void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

#pragma control sched=2

// -- eStaticModel::Read(cFile &, cMemPool *) @ 0x00041b94 --
int eStaticModel::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    unsigned int version = (unsigned int)rb[3];
    if (version >= 3 || version < 1) goto fail;
    if (!((eStaticGeom *)this)->Read(file, pool)) goto fail;

    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x40, 0xC);
    }
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x10, 0xC);
    }
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x20, 0xC);
    }
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x30, 0xC);
    }
    if ((unsigned int)rb[3] >= 2) goto read_field98;
    goto platform;

fail:
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;

read_field98:
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x98, 4);
    }
platform:
    PlatformRead(file, pool);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
