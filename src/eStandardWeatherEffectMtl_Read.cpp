// ODR-WARNING: this split TU intentionally redeclares only the methods needed
// for eStandardWeatherEffectMtl::Read. Do not include or edit the canonical
// header just to add this method; adding declarations to shared headers can
// perturb already-matched siblings in SNC's context-sensitive allocator.

class cBase;
class cFile;
class cMemPool;

void cFile_SetCurrentPos(void *, unsigned int);

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cHandle {
public:
    int mIndex;
    void Read(cReadBlock &, cMemPool *);
};

class eWeatherEffectMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eStandardWeatherEffectMtl : public eWeatherEffectMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

#pragma control sched=1

int eStandardWeatherEffectMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eWeatherEffectMtl::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    cHandle *handle = (cHandle *)((char *)this + 0x48);
    handle->Read(rb, cMemPool::GetPoolFromPtr(handle));
    PlatformRead(file, pool);
    CreateData();
    return result;
}
