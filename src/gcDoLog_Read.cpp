// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched gcDoLog translation unit.

class cBase;
class cFile;
class cMemPool;

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoLog : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void cFileSystem_Read(void *, void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void cReadBlock_Read_cArray_char(cReadBlock *, void *)
    asm("__0fKcReadBlockERead76GcArray7c__R9BA_v");

// gcDoLog::Read(cFile &, cMemPool *) - 0x002e6e5c
int gcDoLog::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if ((unsigned int)rb[3] != 1 || gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cReadBlock_Read_cArray_char((cReadBlock *)rb, (char *)this + 0x10);
    cFileSystem_Read(*(void **)rb[0], (char *)this + 0x0C, 4);

    int i = 0;
    int offset;
    cBase *child;
    offset = 0;
    do {
        cBase **slot = (cBase **)((void **)*(int *)((char *)this + 0x14) + offset);
        child = *slot;
        ((cReadBlock *)rb)->ReadBase(pool, (cBase *)this, child);
        *slot = child;
        i++;
        offset++;
    } while (i < 3);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
