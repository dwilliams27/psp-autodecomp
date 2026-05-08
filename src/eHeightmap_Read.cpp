// ODR-WARNING: this TU intentionally redeclares eHeightmap with only the
// members needed for Read(cFile &, cMemPool *). Including eHeightmap.h would
// require adding shared method declarations and can perturb matched siblings.

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

void cFile_SetCurrentPos(void *, unsigned int);

class eStaticGeom {
public:
    int Read(cFile &, cMemPool *);
};

class eHeightmap : public eStaticGeom {
public:
    char _pad[0x90];

    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
};

#pragma control sched=1

// eHeightmap::Read(cFile &, cMemPool *) — 0x000527b0
int eHeightmap::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && eStaticGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    void *h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, (char *)this + 0x40, 12);
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, (char *)this + 0x10, 12);
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, (char *)this + 0x20, 12);
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, (char *)this + 0x30, 12);
    PlatformRead(file, pool);
    return result;
}

#pragma control sched=2
