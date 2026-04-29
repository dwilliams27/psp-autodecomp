// src/gcParticleSystemConfig.cpp
//
// Methods of gcParticleSystemConfig (gcAll_psp.obj). Sibling layout to
// gcDynamicModelConfig — same 0x60-byte body, same VFPU quads at 0x30/0x40,
// same dcast → struct-copy AssignCopy and same operator-delete-dispatch dtor.
//
//   - Write(cFile &) const                          @ 0x001555f8
//   - Read(cFile &, cMemPool *)                     @ 0x00155644
//   - gcParticleSystemConfig(cBase *)               @ (matched in eVolume.cpp)
//   - New(cMemPool *, cBase *) static               @ 0x0031dd14
//   - AssignCopy(const cBase *)                     @ 0x0031dc7c
//   - ~gcParticleSystemConfig(void)                 @ 0x0031df50
//
// Patterns:
//   - AssignCopy : gcDynamicModelConfig::AssignCopy — dcast then struct-copy
//             groups of contiguous fields. Layout includes int @8, 6 ints
//             at 0xC..0x20, int @0x24, two 16-byte VFPU quads at 0x30/0x40,
//             and an unsigned char at 0x50.
//   - ~Dtor : eDynamicModelLookAtTemplate::~ — write cBaseclassdesc at offset
//             4, then on (flag & 1) call operator delete (inlined).

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

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];                       // @ 0x37E6A8

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

struct gcPSC_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcPSC_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcParticleSystemConfig : public gcEntityGeomConfig {
public:
    ~gcParticleSystemConfig();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcPSC_DeleteRecord *rec =
            (gcPSC_DeleteRecord *)
            (((gcPSC_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F7A0;

extern "C" {
    void gcParticleSystemConfig__gcParticleSystemConfig_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcParticleSystemConfig::Write(cFile &) const @ 0x001555f8 ──
void gcParticleSystemConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcEntityGeomConfig::Write(file);
    wb.End();
}

// ── gcParticleSystemConfig::Read(cFile &, cMemPool *) @ 0x00155644 ──
int gcParticleSystemConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] == 1 && this->gcEntityGeomConfig::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcParticleSystemConfig::New(cMemPool *, cBase *) static @ 0x0031dd14 ──
cBase *gcParticleSystemConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcParticleSystemConfig *result = 0;
    gcParticleSystemConfig *obj = (gcParticleSystemConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcParticleSystemConfig__gcParticleSystemConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcParticleSystemConfig::GetType(void) const @ 0x0031dd90 ──
const cType *gcParticleSystemConfig::GetType(void) const {
    if (D_0009F7A0 == 0) {
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
        D_0009F7A0 = cType::InitializeType(0, 0, 0x1F9, D_0009F43C,
                                           &gcParticleSystemConfig::New,
                                           0, 0, 0);
    }
    return D_0009F7A0;
}

// ── gcParticleSystemConfig::AssignCopy(const cBase *) @ 0x0031dc7c ──
//
// NOT YET MATCHED — 19/152 bytes differ. Same shape as gcDynamicModelConfig::
// AssignCopy (also unmatched, also 19/152). Layout and field types are
// correct (sizes line up) but our SNC's scheduler hoists the next addiu
// before the previous sw, forcing it into a different register (a2 instead
// of reusing a0). Memory barriers, operator= dispatch, and 180s of permuter
// search all failed to recover the original register-allocation pattern.
struct gcPSC_W4 { int v; };
struct gcPSC_B24 { int v[6]; };

typedef int gcPSC_v4sf_t __attribute__((mode(V4SF)));

void gcParticleSystemConfig::AssignCopy(const cBase *src) {
    gcParticleSystemConfig *other = dcast<gcParticleSystemConfig>(src);
    *(gcPSC_W4 *)((char *)this + 8) = *(const gcPSC_W4 *)((const char *)other + 8);
    *(gcPSC_B24 *)((char *)this + 0xC) = *(const gcPSC_B24 *)((const char *)other + 0xC);
    *(gcPSC_W4 *)((char *)this + 0x24) = *(const gcPSC_W4 *)((const char *)other + 0x24);
    *(gcPSC_v4sf_t *)((char *)this + 0x30) = *(const gcPSC_v4sf_t *)((const char *)other + 0x30);
    *(gcPSC_v4sf_t *)((char *)this + 0x40) = *(const gcPSC_v4sf_t *)((const char *)other + 0x40);
    *(unsigned char *)((char *)this + 0x50) = *(const unsigned char *)((const char *)other + 0x50);
}

// ── gcParticleSystemConfig::~gcParticleSystemConfig(void) @ 0x0031df50 ──
gcParticleSystemConfig::~gcParticleSystemConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
