// eBipedControllerConfig.cpp — decompiled from eAll_psp.obj
// eBipedControllerConfig inherits from ePhysicsControllerConfig.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class ePhysicsControllerConfig {
public:
    ePhysicsControllerConfig(cBase *);
    ~ePhysicsControllerConfig();
    void Write(cFile &) const;
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

class eBipedControllerConfig : public ePhysicsControllerConfig {
public:
    eBipedControllerConfig(cBase *);
    ~eBipedControllerConfig();
    void Write(cFile &) const;
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

// ── eBipedControllerConfig::Write(cFile &) const @ 0x00062610 ──
void eBipedControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const ePhysicsControllerConfig *)this)->Write(file);
    wb.Write(*(float *)((char *)this + 0x20));
    wb.End();
}

#pragma control sched=2

#pragma control sched=1

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

#pragma control sched=2
