// src/gcDynamicModelConfig.cpp
//
// Methods of gcDynamicModelConfig (gcAll_psp.obj). The constructor lives in
// src/eVolume.cpp (already matched) so it is not duplicated here.
//
//   - Write(cFile &) const                       @ 0x00152344
//   - Read(cFile &, cMemPool *)                  @ 0x00152390
//   - New(cMemPool *, cBase *) static            @ 0x00319d3c
//   - AssignCopy(const cBase *)                  @ 0x00319ca4
//   - ~gcDynamicModelConfig(void)                @ 0x00319f78
//
// Patterns:
//   - Write : cFactory::Write — wrap base-class write in cWriteBlock(file, 1)/End().
//   - Read  : eWeatherEffect::Read — open a cReadBlock(file, 1, true), check the
//             block tag (rb._data[3] == 1), call base Read; on failure rewind file
//             and return 0; on success the dtor runs and we return 1.
//   - New   : eConfigBase::New / gcGeomCurveController::New — pool[9] -> block;
//             block[0x1C] -> allocTable; entry at allocTable+0x28; alloc with
//             size=0x60, alignment=0x10.
//   - AssignCopy : eTexCoordGen::AssignCopy — dcast then struct-copy a series of
//             fields. Layout includes two 16-byte VFPU quads at 0x30 and 0x40.
//   - ~Dtor : eDynamicModelLookAtTemplate::~ — write cBaseclassdesc at offset 4,
//             then on (flag & 1) call operator delete (inlined).

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
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];                       // @ 0x37E6A8

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

struct gcDMC_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcDMC_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDynamicModelConfig : public gcEntityGeomConfig {
public:
    ~gcDynamicModelConfig();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcDMC_DeleteRecord *rec =
            (gcDMC_DeleteRecord *)
            (((gcDMC_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern "C" {
    void gcDynamicModelConfig__gcDynamicModelConfig_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F768;

// ── gcDynamicModelConfig::Write(cFile &) const @ 0x00152344 ──
void gcDynamicModelConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityGeomConfig::Write(file);
    wb.End();
}

// ── gcDynamicModelConfig::Read(cFile &, cMemPool *) @ 0x00152390 ──
int gcDynamicModelConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityGeomConfig::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcDynamicModelConfig::New(cMemPool *, cBase *) static @ 0x00319d3c ──
cBase *gcDynamicModelConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDynamicModelConfig *result = 0;
    gcDynamicModelConfig *obj = (gcDynamicModelConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcDynamicModelConfig__gcDynamicModelConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDynamicModelConfig::GetType(void) const @ 0x00319db8 ──
const cType *gcDynamicModelConfig::GetType(void) const {
    if (D_0009F768 == 0) {
        if (D_0009F43C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F43C = cType::InitializeType(0, 0, 0xA0, D_000385DC,
                                               &gcEntityGeomConfig::New,
                                               0, 0, 0);
        }
        D_0009F768 = cType::InitializeType(0, 0, 0xE1, D_0009F43C,
                                           &gcDynamicModelConfig::New,
                                           0, 0, 0);
    }
    return D_0009F768;
}

// ── gcDynamicModelConfig::AssignCopy(const cBase *) @ 0x00319ca4 ──
struct gcDMC_W4 { int v; };
struct gcDMC_B24 { int v[6]; };

typedef int gcDMC_v4sf_t __attribute__((mode(V4SF)));

void gcDynamicModelConfig::AssignCopy(const cBase *src) {
    gcDynamicModelConfig *other = dcast<gcDynamicModelConfig>(src);
    *(gcDMC_W4 *)((char *)this + 8) = *(const gcDMC_W4 *)((const char *)other + 8);
    *(gcDMC_B24 *)((char *)this + 0xC) = *(const gcDMC_B24 *)((const char *)other + 0xC);
    *(gcDMC_W4 *)((char *)this + 0x24) = *(const gcDMC_W4 *)((const char *)other + 0x24);
    *(gcDMC_v4sf_t *)((char *)this + 0x30) = *(const gcDMC_v4sf_t *)((const char *)other + 0x30);
    *(gcDMC_v4sf_t *)((char *)this + 0x40) = *(const gcDMC_v4sf_t *)((const char *)other + 0x40);
    *(unsigned char *)((char *)this + 0x50) = *(const unsigned char *)((const char *)other + 0x50);
}

// ── gcDynamicModelConfig::~gcDynamicModelConfig(void) @ 0x00319f78 ──
gcDynamicModelConfig::~gcDynamicModelConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
