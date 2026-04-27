class cBase;
class cFile;
class cMemPool;
class cFactory;
class gcGameGlobals;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cFactory {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcGameGlobals : public cFactory {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void gcGameGlobals__gcGameGlobals_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcGameGlobals::Write(cFile &) const @ 0x00105a30 ──
void gcGameGlobals::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cFactory *)this)->Write(file);
    wb.End();
}

// ── gcGameGlobals::Read(cFile &, cMemPool *) @ 0x00105a7c ──
int gcGameGlobals::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cFactory::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcGameGlobals::New(cMemPool *, cBase *) static @ 0x0024dc68 ──
cBase *gcGameGlobals::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcGameGlobals *result = 0;
    gcGameGlobals *obj = (gcGameGlobals *)rec->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        gcGameGlobals__gcGameGlobals_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
