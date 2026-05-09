// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched gcDoSwitchCase translation unit.

class cBase;
class cFile;
class cMemPool;
class cReadBlock;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcExpressionList {
public:
    void Read(cReadBlock &);
};

class gcDoSwitchCase : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// 0x0014fd88 - gcDoSwitchCase::Read(cFile &, cMemPool *)
int gcDoSwitchCase::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if ((unsigned int)rb[3] == 1 &&
        ((gcAction *)this)->gcAction::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    ((gcExpressionList *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    {
        int i = 0;
        int off = 0;
        cBase *child;
        do {
            cBase **slot = *(cBase ***)((char *)this + 0x0C) + i;
            child = *slot;
            ((cReadBlock *)rb)->ReadBase(pool, (cBase *)this, child);
            *slot = child;
            i += 1;
            off += 4;
        } while (i < 3);
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
