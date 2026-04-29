// eBodyWorldConstraintConfig - config for a rigid-body-to-world constraint.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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
extern cType *D_000385DC;
extern cType *D_000469EC;
extern cType *D_00046BFC;

class eBodyWorldConstraintConfig : public ePhysicsConstraintConfig {
public:
    char _pad2[0x40];

    eBodyWorldConstraintConfig(cBase *);
    ~eBodyWorldConstraintConfig();
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

#pragma control sched=1

// eBodyWorldConstraintConfig::GetType(void) const @ 0x0020e518
const cType *eBodyWorldConstraintConfig::GetType(void) const {
    if (D_00046BFC == 0) {
        if (D_000469EC == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_000469EC = cType::InitializeType(0, 0, 0x25C, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_000469EC;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20E49C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BFC = cType::InitializeType(0, 0, 0x25D, parentType, factory, 0, 0, 0);
    }
    return D_00046BFC;
}

#pragma control sched=2
