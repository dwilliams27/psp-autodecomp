// cConfigBase — runtime config block (paired strings + typed arrays)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const bool *);
    void Write(int, const int *);
    void End(void);
};

class cStr {
public:
    char _data[256];
    void Write(cWriteBlock &) const;
};

class cConfigBase {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void cConfigBase__cConfigBase_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── cConfigBase::Write(cFile &) const @ 0x000084d8 ──
void cConfigBase::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cStr *)((char *)this + 8))->Write(wb);
    ((const cStr *)((char *)this + 0x108))->Write(wb);
    wb.Write(0xC);
    wb.Write(0xC, (const bool *)((char *)this + 0x208));
    wb.Write(0xC, (const int *)((char *)this + 0x214));
    wb.End();
}

// ── cConfigBase::New(cMemPool *, cBase *) static @ 0x001c6848 ──
cBase *cConfigBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    cConfigBase *result = 0;
    cConfigBase *obj = (cConfigBase *)rec->fn(base, 0x244, 4, 0, 0);
    if (obj != 0) {
        cConfigBase__cConfigBase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
