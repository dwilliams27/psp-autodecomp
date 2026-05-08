// ODR-WARNING: this TU intentionally redeclares eBumpOffsetMap with a minimal
// member list because include/eTextureMap.h does not yet declare Read. Keep
// this split from src/eBumpOffsetMap.cpp so existing matches in the canonical
// TU are not perturbed.

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

class eBumpOffsetMap : public eTexture {
public:
    int Read(cFile &, cMemPool *);
    void CreateData(void);
    void PlatformRead(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

// -- eBumpOffsetMap::Read(cFile &, cMemPool *) @ 0x0008022c --
int eBumpOffsetMap::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);

    if ((unsigned int)rb._data[3] == 2 && eTexture::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    char temp;
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &temp, 1);
    }
    *(unsigned char *)((char *)this + 0x4C) = temp != 0;
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x50, 4);
    }
    CreateData();
    PlatformRead(file, pool);
    return result;
}

#pragma control sched=2
