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
    cObject &operator=(const cObject &);
};

class eWeatherEffectTemplate : public cObject {
public:
    eWeatherEffectTemplate(cBase *);
    ~eWeatherEffectTemplate(void);
    void AssignCopy(const cBase *);
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

template <class T> T *dcast(const cBase *);

#pragma control sched=1

// eWeatherEffectTemplate::~eWeatherEffectTemplate(void) @ 0x00061448
eWeatherEffectTemplate::~eWeatherEffectTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

// eWeatherEffectTemplate::AssignCopy(const cBase *) @ 0x002078d0
struct eWeatherEffectTemplate_block_4 { int _[1]; };

void eWeatherEffectTemplate::AssignCopy(const cBase *src) {
    eWeatherEffectTemplate *other = dcast<eWeatherEffectTemplate>(src);
    cObject::operator=(*other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(float *)((char *)other + 0x48);
    *(float *)((char *)this + 0x4C) = *(float *)((char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)other + 0x64);
    *(float *)((char *)this + 0x68) = *(float *)((char *)other + 0x68);
    *(float *)((char *)this + 0x6C) = *(float *)((char *)other + 0x6C);
    __asm__ volatile("" ::: "memory");
    eWeatherEffectTemplate_block_4 *dst70 = (eWeatherEffectTemplate_block_4 *)((char *)this + 0x70);
    eWeatherEffectTemplate_block_4 *src70 = (eWeatherEffectTemplate_block_4 *)((char *)other + 0x70);
    *dst70 = *src70;
    *(unsigned char *)((char *)this + 0x74) = *(unsigned char *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    *(float *)((char *)this + 0x7C) = *(float *)((char *)other + 0x7C);
    *(float *)((char *)this + 0x80) = *(float *)((char *)other + 0x80);
    *(float *)((char *)this + 0x84) = *(float *)((char *)other + 0x84);
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
