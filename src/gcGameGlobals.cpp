class cBase;
class cFile;
class cMemPool;
class cFactory;
class gcGameGlobals;

class cObject {
public:
    cObject &operator=(const cObject &);
};

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
    char _pad0[0x54];
    int mGroups[1];

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

template <class T> T *dcast(const cBase *);

struct WordCopy {
    int value;
};

// ── gcGameGlobals::AssignCopy(const cBase *) @ 0x0024dbd4 ──
void gcGameGlobals::AssignCopy(const cBase *base) {
    gcGameGlobals *other = dcast<gcGameGlobals>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(int *)((char *)this + 0x44) = *(const int *)((char *)other + 0x44);
    *(int *)((char *)this + 0x48) = *(const int *)((char *)other + 0x48);
    *(WordCopy *)((char *)this + 0x4C) =
        *(const WordCopy *)((char *)other + 0x4C);
    *(WordCopy *)((char *)this + 0x50) =
        *(const WordCopy *)((char *)other + 0x50);
    int i = 0;
    gcGameGlobals *src = other;
    gcGameGlobals *dst = this;
    do {
        ++i;
        dst->mGroups[0] = src->mGroups[0];
        src = (gcGameGlobals *)((char *)src + 4);
        dst = (gcGameGlobals *)((char *)dst + 4);
    } while (i <= 0);
}

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
