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

    gcEntityGeomConfig(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct gcGeomTrailConfig_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcGeomTrailConfig_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcGeomTrailConfig_cMemPoolNS {
public:
    static gcGeomTrailConfig_cMemPoolNS *GetPoolFromPtr(const void *);
};

extern char cBaseclassdesc[];                  // 0x37E6A8

class gcGeomTrailConfig : public gcEntityGeomConfig {
public:
    gcGeomTrailConfig(cBase *);
    ~gcGeomTrailConfig();
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcGeomTrailConfig_cMemPoolNS *pool =
            gcGeomTrailConfig_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcGeomTrailConfig_DeleteRecord *rec =
            (gcGeomTrailConfig_DeleteRecord *)
            (((gcGeomTrailConfig_PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
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

// ── gcGeomTrailConfig::gcGeomTrailConfig(cBase *) @ 0x00154a34 ──
gcGeomTrailConfig::gcGeomTrailConfig(cBase *parent) : gcEntityGeomConfig(parent) {
    *(void **)((char *)this + 4) = (void *)0x38CAE0;
}

// ── gcGeomTrailConfig::~gcGeomTrailConfig(void) @ 0x0031cd0c ──
//
// Canonical destructor. SNC's ABI auto-emits the (this != 0) guard and
// the deleting-tail dispatch through operator delete on (flag & 1). Body
// just resets the classdesc pointer at offset 4 to the parent classdesc.
gcGeomTrailConfig::~gcGeomTrailConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
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
