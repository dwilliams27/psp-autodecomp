// ODR-WARNING: split-TU method addition. This file locally redeclares only the
// methods needed for eStaticLight::Read so existing eStaticLight siblings keep
// their original compile context.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class cObject {
public:
    int Read(cFile &, cMemPool *);
};

class eStaticLight : public cObject {
public:
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1
int eStaticLight::Read(cFile &file, cMemPool *pool) {
    int result;
    void *h;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 4, true);
    unsigned int version = (unsigned int)rb._data[3];

    if (version >= 5 || version < 3) goto fail;
    if (((cObject *)this)->Read(file, pool) == 0) goto fail;

    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x44, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x48, 4);
    }

    version = (unsigned int)rb._data[3];
    if (version < 4) goto read_old;
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    goto read_field80;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

read_old:
    {
        int unused;
        h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, &unused, 4);
        h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
    }

read_field80:
    cFileSystem::Read((cFileHandle *)h, (char *)this + 0x80, 0xC);
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x50, 0xC);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x60, 0xC);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x70, 0xC);
    }
    return result;
}
#pragma control sched=2
