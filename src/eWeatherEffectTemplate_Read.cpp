// ODR-WARNING: this TU intentionally redeclares eWeatherEffectTemplate with a
// minimal member list because include/eWeatherEffectTemplate.h does not yet
// declare Read. Keep this split from src/eWeatherEffectTemplate.cpp so
// existing matches in the canonical TU are not perturbed.

class cFile;
class cFileHandle;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
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

class eGeomTemplate {
public:
    int Read(cFile &, cMemPool *);
};

class eWeatherEffectTemplate {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

int eWeatherEffectTemplate::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 4, true);

    if ((unsigned int)rb._data[3] != 4) goto fail;
    if (((eGeomTemplate *)this)->Read(file, pool)) goto success;
    goto fail;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    *(int *)((char *)this + 0x70) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x70);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }

    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x44, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x60, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x48, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x4C, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x50, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x58, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x64, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x5C, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x54, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x68, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x6C, 4);

    char flag;
    cFileSystem::Read(*(cFileHandle **)rb._data[0], &flag, 1);
    *(unsigned char *)((char *)this + 0x74) = flag != 0;

    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x78, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x7C, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x80, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x84, 4);
    return result;
}

#pragma control sched=2
