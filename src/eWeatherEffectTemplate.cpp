class cBase;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
};

class eWeatherEffectTemplate : public cObject {
public:
    eWeatherEffectTemplate(cBase *);
    ~eWeatherEffectTemplate(void);
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

extern char eGeomTemplatevirtualtable[];

extern "C" {
    void eWeatherEffectTemplate__eWeatherEffectTemplate_cBaseptr(void *self, cBase *parent);
}

#pragma control sched=1

// eWeatherEffectTemplate::~eWeatherEffectTemplate(void) @ 0x00061448
eWeatherEffectTemplate::~eWeatherEffectTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

// eWeatherEffectTemplate::New(cMemPool *, cBase *) static @ 0x002079a0
cBase *eWeatherEffectTemplate::New(cMemPool *pool, cBase *parent) {
    eWeatherEffectTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eWeatherEffectTemplate *obj = (eWeatherEffectTemplate *)rec->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        eWeatherEffectTemplate__eWeatherEffectTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
