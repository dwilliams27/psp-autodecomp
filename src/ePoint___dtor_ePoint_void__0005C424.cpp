// ePoint::~ePoint(void) — 0x0005c424 — emits __0oGePointdtv
// Split-TU isolated build. Canonical SNC deleting destructor:
//   (this != 0) guard, vtable store, ~cObject() base chain (flags=0),
//   then `if (flags & 1) operator delete(this)` deleting-tail.
//
// ePoint's operator delete has NO pool-null check (pool always valid),
// unlike cFilePlatform. Target tail register idiom (a0/a1/a2):
//   lw a0,36(v0); lw a1,28(a0); addiu a1,a1,48; lh a2,0(a1);
//   addu a0,a0,a2; lw a2,4(a1); jalr a2; move a1,s1(delay)

class cBase;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class ePoint : public cObject {
public:
    ePoint(cBase *);
    ~ePoint(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        char *base = block + rec->offset;
        __asm__ volatile("" ::: "memory");
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char ePoint_cBase_vtable[];   // 0x382010

#pragma control sched=1
ePoint::~ePoint(void) {
    *(void **)((char *)this + 4) = ePoint_cBase_vtable;
}
#pragma control sched=2
