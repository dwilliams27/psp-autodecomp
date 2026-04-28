// gcConfig — core game configuration block.
//
// Functions matched here:
//   gcConfig::~gcConfig(void)                       @ 0x000f0050
//   gcConfig::New(cMemPool *, cBase *) static       @ 0x00246534
//   gcConfig::GetType(void) const                   @ 0x002465b0

class cBase;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcConfig {
public:
    gcConfig(cBase *);
    ~gcConfig();

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcConfigvirtualtable[];
extern char cBaseclassdesc[];

extern cType *D_000385DC;
extern cType *D_00099AF0;
extern void *gcConfig__s_pInstance__0037D84C;

extern "C" {
    void gcConfig__gcConfig_cBaseptr(void *self, cBase *parent);
}

// ── gcConfig::New(cMemPool *, cBase *) static @ 0x00246534 ──
cBase *gcConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcConfig *result = 0;
    gcConfig *obj = (gcConfig *)rec->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        gcConfig__gcConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcConfig::GetType(void) const @ 0x002465b0 ──
const cType *gcConfig::GetType(void) const {
    if (D_00099AF0 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AF0 = cType::InitializeType(0, 0, 0x28D, D_000385DC,
                                           &gcConfig::New, 0, 0, 0);
    }
    return D_00099AF0;
}

// ── gcConfig::~gcConfig(void) @ 0x000f0050 ──
gcConfig::~gcConfig() {
    *(void **)((char *)this + 4) = gcConfigvirtualtable;
    gcConfig__s_pInstance__0037D84C = 0;
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
