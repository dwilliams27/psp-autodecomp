class cFile;
class cBase;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, int);
    ~cReadBlock(void);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class cFactory {
public:
    int Read(cFile &, cMemPool *);
    void ReadGroups(cReadBlock &, cMemPool *);
};

void cFile_SetCurrentPos(void *, unsigned int);
void *cMemPool_GetPoolFromPtr(void *);

class gcLoadingScreen : public cFactory {
public:
    int Read(cFile &, cMemPool *);
};

int gcLoadingScreen::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] >= 3 || (unsigned int)rb._data[3] < 1)
        goto fail;
    if (!cFactory::Read(file, pool))
        goto fail;
    if ((unsigned int)rb._data[3] >= 2)
        goto do_groups;
    goto after_groups;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
do_groups:
    cFactory::ReadGroups(rb, pool);
after_groups:
    *(int *)((char *)this + 0x4C) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x4C);
        void *handlePool = cMemPool_GetPoolFromPtr(handle);
        handle->Read(rb, (cMemPool *)handlePool);
    }
    return result;
}
