// eBodyWorldConstraintConfig - config for a rigid-body-to-world constraint.

class cBase;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class ePhysicsConstraintConfig {
public:
    char _pad[0x20];
    ePhysicsConstraintConfig(cBase *);
    ~ePhysicsConstraintConfig();
};

extern char eBodyWorldConstraintConfigvirtualtable[];

class eBodyWorldConstraintConfig : public ePhysicsConstraintConfig {
public:
    char _pad2[0x40];

    eBodyWorldConstraintConfig(cBase *);
    ~eBodyWorldConstraintConfig();

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

// eBodyWorldConstraintConfig::~eBodyWorldConstraintConfig(void) @ 0x0007617c
eBodyWorldConstraintConfig::~eBodyWorldConstraintConfig() {
    *(void **)((char *)this + 4) = eBodyWorldConstraintConfigvirtualtable;
}

// eBodyWorldConstraintConfig::New(cMemPool *, cBase *) static @ 0x0020e49c
cBase *eBodyWorldConstraintConfig::New(cMemPool *pool, cBase *parent) {
    eBodyWorldConstraintConfig *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBodyWorldConstraintConfig *obj = (eBodyWorldConstraintConfig *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eBodyWorldConstraintConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
