// gcDoUISetTextSprite.cpp - split TU for gcDoUISetTextSprite::Read.

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int pos;
    int _pad[3];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDesiredUIWidgetHelper {
public:
    void Read(cReadBlock &);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

struct ReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoUISetTextSprite : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

// 0x00315878 - gcDoUISetTextSprite::Read(cFile &, cMemPool *)
int gcDoUISetTextSprite::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._pad[1] == 1 && gcAction::Read(file, pool)) goto success;
    cFile_SetCurrentPos(rb.file, rb.pos);
    return 0;
success:
    ((gcDesiredUIWidgetHelper *)((char *)this + 0x0C))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(rb);
    {
        register char *typeInfo __asm__("$5") = *(char **)((char *)this + 0x20);
        register void *base __asm__("$4") = (char *)this + 0x1C;
        ReadSlot *slot = (ReadSlot *)(typeInfo + 0x30);
        short off = slot->offset;
        cFile *f = rb.file;
        slot->fn((char *)base + off, f,
                 cMemPool::GetPoolFromPtr(base));
    }
    return result;
}
