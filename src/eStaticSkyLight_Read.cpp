// ODR-WARNING: split-TU method addition. This file locally redeclares only the
// methods needed for eStaticSkyLight::Read so existing eStaticSkyLight siblings
// keep their original compile context.

class cFile;
class cMemPool;
class cBase;

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

void cFile_SetCurrentPos(void *, unsigned int);

class eStaticLight {
public:
    int Read(cFile &, cMemPool *);
};

class eStaticSkyLight : public eStaticLight {
public:
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1
int eStaticSkyLight::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] == 2 && eStaticLight::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read(h, (char *)this + 0x90, 4);
    }
    return result;
}
#pragma control sched=2
