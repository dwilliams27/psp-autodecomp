// eSimpleMotorConfig — derives from ePhysicsMotorConfig (size 0x40).
//
// Functions matched here:
//   eSimpleMotorConfig::~eSimpleMotorConfig(void)       @ 0x00076494  124B
//   eSimpleMotorConfig::New(cMemPool *, cBase *) static @ 0x0020e758  124B

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class ePhysicsMotorConfig {
public:
    char _pad[0x40];
    ePhysicsMotorConfig(cBase *);
    ~ePhysicsMotorConfig();
};

extern char eSimpleMotorConfigvirtualtable[];

class eSimpleMotorConfig : public ePhysicsMotorConfig {
public:
    eSimpleMotorConfig(cBase *);
    ~eSimpleMotorConfig();
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

#pragma control sched=1

// ── eSimpleMotorConfig::~eSimpleMotorConfig(void) @ 0x00076494 ──
eSimpleMotorConfig::~eSimpleMotorConfig() {
    *(void **)((char *)this + 4) = eSimpleMotorConfigvirtualtable;
}

// ── eSimpleMotorConfig::New(cMemPool *, cBase *) static @ 0x0020e758 ──
cBase *eSimpleMotorConfig::New(cMemPool *pool, cBase *parent) {
    eSimpleMotorConfig *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSimpleMotorConfig *obj = (eSimpleMotorConfig *)rec->fn(base, 0x40, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eSimpleMotorConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
