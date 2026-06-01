// ODR-WARNING: split-TU local redeclaration for one method (eSpriteMtl::Read).
// Do not include this class definition from headers.

class cBase;
class cFile;
class cMemPool;
struct cFileHandle;

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class eMaterial {
public:
    char _pad[0x5C];
    int Read(cFile &, cMemPool *);
};

class eSpriteMtl : public eMaterial {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

#pragma control sched=2

int eSpriteMtl::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] == 2 && eMaterial::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x5C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x60, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

#pragma control sched=2
