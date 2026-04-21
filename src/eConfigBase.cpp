#include "eConfigBase.h"

class cBase;
class cMemPool;

extern "C" {
    int ePhysics__GetPhysicsMemPoolSize_voidstatic(void);
    void eConfigBase__eConfigBase_cBaseptr(void *self, cBase *parent);
}

class cMemPool_shim {
public:
    static void *GetPoolFromPtr(const void *);
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

// ── eConfigBase::GetPhysicsMemPoolSize(void) const @ 0x0001cb18 ──
int eConfigBase::GetPhysicsMemPoolSize(void) const {
    return ePhysics__GetPhysicsMemPoolSize_voidstatic();
}

// ── eConfigBase::FindResolution(int, int) static @ 0x0001cabc ──
int eConfigBase::FindResolution(int width, int height) {
    const int *w = (const int *)0x37D070;
    const int *h = (const int *)0x37D09C;
    int i = 0;
    do {
        if (*w == width && *h == height) return i;
        ++w;
        ++h;
        ++i;
    } while (i < 11);
    return 0;
}

// ── eConfigBase::GetAudioBytesPerSecond(unsigned int) const @ 0x001df540 ──
unsigned int eConfigBase::GetAudioBytesPerSecond(unsigned int mode) const {
    unsigned int result;
    switch (mode) {
    default: return 0x15888;
    case 1: return 0x80;
    case 2: return 0x6108;
    case 3: return 0x6270;
    case 4: return 0x6270;
    case 5: return 0x15888;
    case 6: return 0x6270;
    case 7: return 0x6108;
    case 8:
        result = 0x4800;
        __asm__ volatile("" ::: "memory");
        return result;
    }
}

// ── eConfigBase::New(cMemPool *, cBase *) static @ 0x001df384 ──
cBase *eConfigBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eConfigBase *result = 0;
    eConfigBase *obj = (eConfigBase *)rec->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eConfigBase__eConfigBase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eConfigBase::~eConfigBase(void) @ 0x0001c918 ──
extern "C" {

void eConfigBase___dtor_eConfigBase_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = (void *)0x37F8B0;
        *(int *)0x37D0C8 = 0;
        *(void **)((char *)self + 4) = (void *)0x37E6A8;
        if (flags & 1) {
            void *pool = cMemPool_shim::GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRec *rec = (DeleteRec *)(allocTable + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
