// gcLightingConfig — lighting configuration class.
// Functions matched here:
//   gcLightingConfig::Write(cFile &) const         @ 0x000f0ba4  (gcAll_psp.obj)
//   gcLightingConfig::New(cMemPool *, cBase *)     @ 0x002468bc  (gcAll_psp.obj)
//   gcLightingConfig::GetType(void) const          @ 0x00246980  (gcAll_psp.obj)
//   gcLightingConfig::~gcLightingConfig(void)      @ 0x00246a20  (gcAll_psp.obj)

class cFile;
class cMemPool;
class cType;
class cBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

extern char cBaseclassdesc[];                       // @ 0x37E6A8
extern char gcLightingConfigvirtualtable[];

extern const char gcLightingConfig_base_name[];     // @ 0x36D894
extern const char gcLightingConfig_base_desc[];     // @ 0x36D89C

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct gcLC_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcLC_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct gcLC_AllocRecord {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLightingConfig_cMemPoolNS {
public:
    static gcLightingConfig_cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcLightingConfig {
public:
    ~gcLightingConfig();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcLightingConfig_cMemPoolNS *pool =
            gcLightingConfig_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcLC_DeleteRecord *rec =
            (gcLC_DeleteRecord *)(((gcLC_PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mId;
};

// ── gcLightingConfig::AssignCopy(const cBase *) @ 0x00246854 ──
void gcLightingConfig::AssignCopy(const cBase *base) {
    gcLightingConfig *other = dcast<gcLightingConfig>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(float *)((char *)this + 12) = *(const float *)((char *)other + 12);
    *(int *)((char *)this + 16) = *(const int *)((char *)other + 16);
    *(float *)((char *)this + 20) = *(const float *)((char *)other + 20);
    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);
    *(float *)((char *)this + 28) = *(const float *)((char *)other + 28);
    *(cHandle *)((char *)this + 32) = *(const cHandle *)((char *)other + 32);
}

// ── Write ──  @ 0x000f0ba4, 136B
void gcLightingConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    wb.Write(*(int *)((char *)this + 0x08));
    wb.Write(*(float *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x10));
    wb.Write(*(float *)((char *)this + 0x14));
    wb.Write(*(int *)((char *)this + 0x18));
    wb.Write(*(float *)((char *)this + 0x1C));
    wb.Write(*(unsigned int *)((char *)this + 0x20));
    wb.End();
}

// 0x002468bc - gcLightingConfig::New(cMemPool *, cBase *) static
cBase *gcLightingConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((gcLC_PoolBlock *)block)->allocTable;
    gcLC_AllocRecord *entry = (gcLC_AllocRecord *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLightingConfig *result = 0;
    gcLightingConfig *obj =
        (gcLightingConfig *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcLightingConfigvirtualtable;
        ((int *)obj)[2] = 8;
        *(float *)((char *)obj + 0x0C) = 1.5f;
        ((int *)obj)[4] = 1;
        result = obj;
        *(float *)((char *)obj + 0x14) = 2.0f;
        ((int *)obj)[6] = 0x1388;
        *(float *)((char *)obj + 0x1C) = 1.0f;
        ((int *)obj)[8] = 0;
    }
    return (cBase *)result;
}

// ── GetType ──  @ 0x00246980, 160B
static cType *type_base;
static cType *type_gcLightingConfig;

const cType *gcLightingConfig::GetType(void) const {
    if (!type_gcLightingConfig) {
        if (!type_base) {
            type_base = cType::InitializeType(gcLightingConfig_base_name,
                                              gcLightingConfig_base_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_gcLightingConfig = cType::InitializeType(0, 0, 0xFC, type_base,
                                                       &gcLightingConfig::New,
                                                       0, 0, 0);
    }
    return type_gcLightingConfig;
}

// ── ~gcLightingConfig ── @ 0x00246a20, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through operator delete on (flag & 1). Body just resets the
// classdesc pointer at offset 4 to the parent (cBase) classdesc.
gcLightingConfig::~gcLightingConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
