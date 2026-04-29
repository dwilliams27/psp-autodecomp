// eBipedControllerConfig.cpp — decompiled from eAll_psp.obj
// eBipedControllerConfig inherits from ePhysicsControllerConfig.

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class ePhysicsControllerConfig {
public:
    char _pad[0x8];
    ePhysicsControllerConfig(cBase *);
    ~ePhysicsControllerConfig();
    void Write(cFile &) const;
};

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
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

struct eBipedControllerConfigData {
    int _w[6];
};

struct cHandle {
    int mId;
};

class eBipedControllerConfig : public ePhysicsControllerConfig {
public:
    int mField8;
    cHandle mFieldC;
    cHandle mField10;
    int mField14;
    int mField18;
    int mField1C;
    float mField20;

    eBipedControllerConfig(cBase *);
    ~eBipedControllerConfig();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

class ePhysicsControllerTemplate {
public:
    ePhysicsControllerTemplate(cBase *);
    ~ePhysicsControllerTemplate();
};

class eBipedControllerTemplate : public ePhysicsControllerTemplate {
public:
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

template <class T>
T *dcast(const cBase *);

static cType *type_cBase;
static cType *type_ePhysicsControllerConfig;
static cType *type_eBipedControllerConfig;
static cType *type_ePhysicsControllerTemplate;
static cType *type_eBipedControllerTemplate;

#pragma control sched=1

// ── eBipedControllerConfig::Write(cFile &) const @ 0x00062610 ──
void eBipedControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const ePhysicsControllerConfig *)this)->Write(file);
    wb.Write(*(float *)((char *)this + 0x20));
    wb.End();
}

#pragma control sched=2

#pragma control sched=1

// ── eBipedControllerConfig::AssignCopy(const cBase *) @ 0x002083ec ──
void eBipedControllerConfig::AssignCopy(const cBase *base) {
    eBipedControllerConfig *other = dcast<eBipedControllerConfig>(base);
    *(eBipedControllerConfigData *)((char *)this + 8) =
        *(eBipedControllerConfigData *)((char *)other + 8);
    mField20 = other->mField20;
}

// ── eBipedControllerConfig::GetType(void) const @ 0x002084d0 ──
const cType *eBipedControllerConfig::GetType(void) const {
    if (!type_eBipedControllerConfig) {
        if (!type_ePhysicsControllerConfig) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_ePhysicsControllerConfig =
                cType::InitializeType(0, 0, 0x23D, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_ePhysicsControllerConfig;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x208454;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eBipedControllerConfig =
            cType::InitializeType(0, 0, 0x240, parentType, factory, 0, 0, 0);
    }
    return type_eBipedControllerConfig;
}

// ── eBipedControllerConfig::~eBipedControllerConfig(void) @ 0x000627dc ──
eBipedControllerConfig::~eBipedControllerConfig() {
    *(void **)((char *)this + 4) = (void *)0x382BF0;
}

// ── eBipedControllerConfig::New(cMemPool *, cBase *) static @ 0x00208454 ──
cBase *eBipedControllerConfig::New(cMemPool *pool, cBase *parent) {
    eBipedControllerConfig *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBipedControllerConfig *obj = (eBipedControllerConfig *)rec->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        new (obj) eBipedControllerConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eBipedControllerTemplate::GetType(void) const @ 0x002086b4 ──
const cType *eBipedControllerTemplate::GetType(void) const {
    if (!type_eBipedControllerTemplate) {
        if (!type_ePhysicsControllerTemplate) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_ePhysicsControllerTemplate =
                cType::InitializeType(0, 0, 0x22F, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_ePhysicsControllerTemplate;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x208638;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eBipedControllerTemplate =
            cType::InitializeType(0, 0, 0x237, parentType, factory, 0, 0, 0);
    }
    return type_eBipedControllerTemplate;
}

#pragma control sched=2
