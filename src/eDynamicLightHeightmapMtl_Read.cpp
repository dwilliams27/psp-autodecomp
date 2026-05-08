// ODR-WARNING: split-TU local redeclaration for one method. Do not include this
// class definition from headers; it intentionally lists only methods referenced here.

class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eHeightmapMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eDynamicLightHeightmapMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

#pragma control sched=1

int eDynamicLightHeightmapMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eHeightmapMtl *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    PlatformRead(file, pool);
    CreateData();
    return result;
}

#pragma control sched=2
