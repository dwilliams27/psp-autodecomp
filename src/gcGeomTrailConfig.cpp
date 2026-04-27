class cBase;
class cFile;
class cMemPool;

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

class gcEntityGeomConfig {
public:
    int base;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcGeomTrailConfig : public gcEntityGeomConfig {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void gcGeomTrailConfig__gcGeomTrailConfig_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcGeomTrailConfig::Write(cFile &) const @ 0x0015492c ──
void gcGeomTrailConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityGeomConfig::Write(file);
    wb.End();
}

// ── gcGeomTrailConfig::Read(cFile &, cMemPool *) @ 0x00154978 ──
int gcGeomTrailConfig::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityGeomConfig::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcGeomTrailConfig::New(cMemPool *, cBase *) static @ 0x0031cad0 ──
cBase *gcGeomTrailConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcGeomTrailConfig *result = 0;
    gcGeomTrailConfig *obj = (gcGeomTrailConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcGeomTrailConfig__gcGeomTrailConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
