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
    void AssignCopy(const cBase *);
    ~gcShadowConfig();
    static void operator delete(void *);
};

extern "C" {
    void gcShadowConfig__gcShadowConfig_cBaseptr(void *self, cBase *parent);
}

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];                       // @ 0x37E6A8

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gcSC_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcSC_DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcSC_cMemPoolNS {
public:
    static gcSC_cMemPoolNS *GetPoolFromPtr(const void *);
};

typedef int v4sf_t __attribute__((mode(V4SF)));

struct gcSC_Word { int v; };
struct gcSC_Block6 { int v[6]; };

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

// ── gcShadowConfig::AssignCopy(const cBase *) @ 0x0031fa18, 152B ──
void gcShadowConfig::AssignCopy(const cBase *src) {
    gcShadowConfig *other = dcast<gcShadowConfig>(src);
    *(gcSC_Word *)((char *)this + 8) = *(const gcSC_Word *)((const char *)other + 8);
    *(gcSC_Block6 *)((char *)this + 0xC) = *(const gcSC_Block6 *)((const char *)other + 0xC);
    *(gcSC_Word *)((char *)this + 0x24) = *(const gcSC_Word *)((const char *)other + 0x24);
    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((const char *)other + 0x30);
    *(v4sf_t *)((char *)this + 0x40) = *(const v4sf_t *)((const char *)other + 0x40);
    *(unsigned char *)((char *)this + 0x50) = *(const unsigned char *)((const char *)other + 0x50);
}

// ── gcShadowConfig::~gcShadowConfig() @ 0x0031fcec, 100B ──
gcShadowConfig::~gcShadowConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

inline void gcShadowConfig::operator delete(void *p) {
    gcSC_cMemPoolNS *pool = gcSC_cMemPoolNS::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    gcSC_DeleteRecord *rec = (gcSC_DeleteRecord *)(((gcSC_PoolBlock *)block)->allocTable + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
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
