// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched gcDoTriggerActivate translation unit.

class cFile;
class cFileHandle;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cReadBlock;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

struct ReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoTriggerActivate : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// 0x003089a8 - gcDoTriggerActivate::Read(cFile &, cMemPool *)
int gcDoTriggerActivate::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    char active;
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcAction *)this)->gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x10);
        void *base = (char *)this + 0x0C;
        ReadSlot *slot = (ReadSlot *)(typeInfo + 0x30);
        short off = slot->offset;
        cFile *f = *(cFile **)&rb[0];
        slot->fn((char *)base + off, f, cMemPool::GetPoolFromPtr(base));
    }
    cFileSystem::Read(*(cFileHandle **)rb[0], &active, 1);
    *(char *)((char *)this + 0x20) = active != 0;
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
