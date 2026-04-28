class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

extern "C" void gcEvent_ctor(void *, cBase *, const char *) __asm__("__0oHgcEventctP6FcBasePCc");
extern "C" void gcEvent_dtor(void *, int) __asm__("__0oHgcEventdtv");

class gcTimer : public cObject {
public:
    gcTimer(cBase *);
    ~gcTimer(void);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    }
};

cBase *gcTimer::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcTimer *result = 0;
    gcTimer *obj = (gcTimer *)rec->fn(base, 0xC0, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcTimer(parent);
        result = obj;
    }
    return (cBase *)result;
}

gcTimer::gcTimer(cBase *parent) : cObject(parent) {
    *(char **)((char *)this + 4) = (char *)0x38A0A0;
    gcEvent_ctor((char *)this + 0x44, (cBase *)this, (const char *)0x36DB40);
    gcEvent_ctor((char *)this + 0x60, (cBase *)this, (const char *)0x36DB4C);
    gcEvent_ctor((char *)this + 0x7C, (cBase *)this, (const char *)0x36DB64);
    gcEvent_ctor((char *)this + 0x98, (cBase *)this, (const char *)0x36DB58);
    *(int *)((char *)this + 0xB4) = 0;
    *(int *)((char *)this + 0xB8) = 0;
    *(int *)((char *)this + 0xBC) = 0;
}

gcTimer::~gcTimer(void) {
    *(char **)((char *)this + 4) = (char *)0x38A0A0;
    gcEvent_dtor((char *)this + 0x98, 2);
    gcEvent_dtor((char *)this + 0x7C, 2);
    gcEvent_dtor((char *)this + 0x60, 2);
    gcEvent_dtor((char *)this + 0x44, 2);
}
