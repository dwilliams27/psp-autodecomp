// ODR-WARNING: split TU local redeclaration for eTextureMap::Read only.
// Do not include eTextureMap.h here; adding Read to the shared header can
// perturb already matched eTextureMap siblings.

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
    static void Read(void *, void *, unsigned int);
};

class eTexture {
public:
    int Read(cFile &, cMemPool *);
};

class eTextureMap : public eTexture {
public:
    int Read(cFile &, cMemPool *);
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1
int eTextureMap::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 3, true);
    char temp;
    if ((unsigned int)rb._data[3] == 3 && eTexture::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &temp, 1);
    }
    *(unsigned char *)((char *)this + 0x4C) = temp != 0;
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x4D, 1);
    }
    CreateData();
    PlatformRead(file, pool);
    return result;
}
#pragma control sched=2
