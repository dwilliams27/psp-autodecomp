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
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcShadowConfig : public gcEntityGeomConfig {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void gcShadowConfig__gcShadowConfig_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcShadowConfig::Write(cFile &) const @ 0x00157bc0 ──
void gcShadowConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityGeomConfig::Write(file);
    wb.End();
}

// ── gcShadowConfig::Read(cFile &, cMemPool *) @ 0x00157c0c ──
int gcShadowConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityGeomConfig::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcShadowConfig::New(cMemPool *, cBase *) static @ 0x0031fab0 ──
cBase *gcShadowConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcShadowConfig *result = 0;
    gcShadowConfig *obj = (gcShadowConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcShadowConfig__gcShadowConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
