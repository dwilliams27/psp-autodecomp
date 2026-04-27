// eRigidBodyControllerConfig — extends eSimulatedControllerConfig.
//
// Functions matched here:
//   eRigidBodyControllerConfig::Write(cFile &) const                  @ 0x00076f40 (eAll_psp.obj, sched=1)
//   eRigidBodyControllerConfig::~eRigidBodyControllerConfig(void)     @ 0x0007707c (eAll_psp.obj, sched=1)
//   eRigidBodyControllerConfig::New(cMemPool *, cBase *) static       @ 0x0020fa98 (eAll_psp.obj, sched=2)
//
// Same overall shape as eKeyframedControllerConfig.cpp: SNC's ABI auto-
// generates the (this != 0) guard, the chain call to the parent destructor,
// and the deleting-tail dispatch through `operator delete`.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class eSimulatedControllerConfig {
public:
    char _pad[0x8];
    eSimulatedControllerConfig(cBase *);
    ~eSimulatedControllerConfig();
    void Write(cFile &) const;
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRecord {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool_shim {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eRigidBodyControllerConfig : public eSimulatedControllerConfig {
public:
    char _data[0x58];  // total size 0x60

    eRigidBodyControllerConfig(cBase *);
    ~eRigidBodyControllerConfig();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ── Write ──  @ 0x00076f40, 76B
#pragma control sched=1
void eRigidBodyControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eSimulatedControllerConfig *)this)->Write(file);
    wb.End();
}
#pragma control sched=2

// ── Destructor ──  @ 0x0007707c, 124B
#pragma control sched=1
eRigidBodyControllerConfig::~eRigidBodyControllerConfig() {
    *(void **)((char *)this + 4) = (void *)0x384188;
}
#pragma control sched=2

// ── New ──  @ 0x0020fa98, 124B
extern "C" void eRigidBodyControllerConfig__eRigidBodyControllerConfig_cBaseptr(void *self, cBase *parent);

#pragma control sched=1
cBase *eRigidBodyControllerConfig::New(cMemPool *pool, cBase *parent) {
    eRigidBodyControllerConfig *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eRigidBodyControllerConfig *obj = (eRigidBodyControllerConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        eRigidBodyControllerConfig__eRigidBodyControllerConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
