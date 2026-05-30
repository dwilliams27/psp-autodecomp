// ODR-WARNING: split translation unit for ePortal deleting destructor only.
// Emits __0oHePortaldtv (ePortal::~ePortal(void), deleting variant).

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void cObject_dtor(void *, int) asm("__0oHcObjectdtv");

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

class ePortal {
public:
    ~ePortal();
};

#pragma control sched=2

ePortal::~ePortal() {
    *(void **)((char *)this + 4) = (void *)0x380AA8;
    cObject_dtor(this, 0);
}
