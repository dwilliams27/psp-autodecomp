class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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
    static cBase *New(cMemPool *, cBase *);
};

class nwConfigPSP : public nwConfigBase {
public:
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

class eBumpOffsetMap {
public:
    void PlatformRead(cFile &, cMemPool *);
};

class eOnePassModelMtl {
public:
    void PlatformRead(cFile &, cMemPool *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void nwConfigPSP__nwConfigPSP_cBaseptr(void *self, cBase *parent);
}

void cFile_SetCurrentPos(void *, unsigned int);

extern cType *D_000385DC;
extern cType *D_0009F924;
extern cType *D_0009F928;

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

// ── nwConfigPSP::GetType(void) const @ 0x0036b6a8 ──
const cType *nwConfigPSP::GetType(void) const {
    if (D_0009F924 == 0) {
        if (D_0009F928 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x371158,
                                                   (const char *)0x371160,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F928 = cType::InitializeType(0, 0, 0x28C, D_000385DC,
                                               &nwConfigBase::New, 0, 0, 0);
        }
        D_0009F924 = cType::InitializeType(0, 0, 0x298, D_0009F928,
                                           &nwConfigPSP::New, 0, 0, 0);
    }
    return D_0009F924;
}

#pragma control sched=1

// ── eBumpOffsetMap::PlatformRead(cFile &, cMemPool *) @ 0x00080334 ──
void eBumpOffsetMap::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}

// ── eOnePassModelMtl::PlatformRead(cFile &, cMemPool *) @ 0x00081e6c ──
void eOnePassModelMtl::PlatformRead(cFile &file, cMemPool *pool) {
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }
    return;
}
