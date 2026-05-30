// ODR-WARNING: split translation unit for eDynamicLight deleting destructor only.
// Emits __0oNeDynamicLightdtv (eDynamicLight::~eDynamicLight(void), deleting variant).
// Structure mirrors the matched ePortal::~ePortal sibling: set vtable ptr at offset 4,
// chain to the eDynamicGeom base destructor, then conditional inline operator delete
// dispatched through cMemPool::GetPoolFromPtr.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void eDynamicGeom_dtor(void *, int) asm("__0oMeDynamicGeomdtv");

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
    __asm__ volatile("" ::: "memory");
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

class eDynamicLight {
public:
    ~eDynamicLight();
};

#pragma control sched=1

eDynamicLight::~eDynamicLight() {
    *(void **)((char *)this + 4) = (void *)0x384320;
    eDynamicGeom_dtor(this, 0);
}
