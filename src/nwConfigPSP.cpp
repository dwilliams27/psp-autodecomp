class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const char *);
    void End(void);
};

class nwConfigBase {
public:
    void Write(cFile &) const;
};

class nwConfigPSP : public nwConfigBase {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void nwConfigPSP__nwConfigPSP_cBaseptr(void *self, cBase *parent);
}

// ── nwConfigPSP::Write(cFile &) const @ 0x001a4ff4 ──
void nwConfigPSP::Write(cFile &file) const {
    cWriteBlock block(file, 1);
    nwConfigBase::Write(file);
    block.Write(0x20, (const char *)((const char *)this + 0x60));
    block.Write(0x40, (const char *)((const char *)this + 0x80));
    block.End();
}

// ── nwConfigPSP::New(cMemPool *, cBase *) static @ 0x00243f90 ──
cBase *nwConfigPSP::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    nwConfigPSP *result = 0;
    nwConfigPSP *obj = (nwConfigPSP *)rec->fn(base, 0xC0, 4, 0, 0);
    if (obj != 0) {
        nwConfigPSP__nwConfigPSP_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
