class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cNamed {
public:
    void Write(cFile &) const;
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcState {
public:
    char _pad0[0x20];
    unsigned int mField20;
    cHandle mField24;
    unsigned int mField28;
    cBaseArray mField2C;

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void gcState__gcState_cBaseptr(void *self, cBase *parent);
}

// ── gcState::Write(cFile &) const @ 0x0010ac68 ──
void gcState::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cNamed *)this)->Write(file);
    wb.Write(mField20);
    mField24.Write(wb);
    wb.Write(mField28);
    mField2C.Write(wb);
    wb.End();
}

// ── gcState::New(cMemPool *, cBase *) static @ 0x0025908c ──
cBase *gcState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcState *result = 0;
    gcState *obj = (gcState *)rec->fn(base, 0x34, 4, 0, 0);
    if (obj != 0) {
        gcState__gcState_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
