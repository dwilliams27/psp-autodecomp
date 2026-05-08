// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched gcDoUserOp translation unit.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

class gcDoUserOp {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// -- gcDoUserOp::Read @ 0x0031717c --
int gcDoUserOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcAction *)this)->gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0xC, 4);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
