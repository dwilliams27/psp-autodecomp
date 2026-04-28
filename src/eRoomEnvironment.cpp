// eRoomEnvironment.cpp - decompiled from eAll_psp.obj.

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    ~cObject();
};

class eRoomEnvironment : public cObject {
public:
    eRoomEnvironment(cBase *);
    ~eRoomEnvironment();
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eRoomEnvironmentvirtualtable[];

extern "C" void eRoomEnvironment__eRoomEnvironment_cBaseptr(void *self, cBase *parent);

#pragma control sched=1

// eRoomEnvironment::~eRoomEnvironment(void) @ 0x0005c8c0
eRoomEnvironment::~eRoomEnvironment() {
    *(void **)((char *)this + 4) = eRoomEnvironmentvirtualtable;
}

// eRoomEnvironment::New(cMemPool *, cBase *) static @ 0x00203dbc
cBase *eRoomEnvironment::New(cMemPool *pool, cBase *parent) {
    eRoomEnvironment *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eRoomEnvironment *obj = (eRoomEnvironment *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        eRoomEnvironment__eRoomEnvironment_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
