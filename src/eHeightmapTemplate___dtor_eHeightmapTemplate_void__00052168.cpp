// ODR-WARNING: split translation unit for eHeightmapTemplate deleting
// destructor only. Emits __0oSeHeightmapTemplatedtv
// (eHeightmapTemplate::~eHeightmapTemplate(void), deleting variant).
// 0x00052168, 348B.
//
// Canonical SNC C++ destructor. The ABI auto-generates the (this != 0)
// guard, the chain-call to ~cObject, and the deleting-tail dispatch through
// the per-class static operator delete. We write only the body: the vtable
// store, the +0x84 handle-object virtual cleanup (mirrors eDynamicFluid's
// +0xF0 cleanup), PlatformFree, and the inlined cArrayBase<eHeightmapTile>
// destruction at +0x7C.
//
// The trailing self-branch + nop (b $; nop) at 0x000522bc is the TU-tail
// padding SNC emits at the end of the object; reproduced with the same
// dummy-dtor + .word trick used by src/eDynamicFluid_dtor.cpp.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void eHeightmapTemplate__PlatformFree_void(void *)
    asm("eHeightmapTemplate__PlatformFree_void");
extern "C" void cObject_dtor(void *, int) asm("__0oHcObjectdtv");

class eHeightmapTemplate {
public:
    ~eHeightmapTemplate();

    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            __asm__ volatile("" ::: "memory");
            char *base = block + rec->offset;
            rec->fn(base, p);
        }
    }
};

#pragma control sched=2

class zzzzzzzzzzzzzzzz {
public:
    ~zzzzzzzzzzzzzzzz();
};

zzzzzzzzzzzzzzzz::~zzzzzzzzzzzzzzzz() {}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".word 0x00000000\n");

#pragma control sched=1
eHeightmapTemplate::~eHeightmapTemplate() {
    *(void **)((char *)this + 4) = (void *)0x381A90;
    __asm__ volatile("" ::: "memory");

    // The array address at +0x7C is held live across the +0x84 cleanup and
    // PlatformFree (SNC fills the obj!=0 branch delay slot with this addiu).
    char *array = (char *)this + 0x7C;

    // +0x84 handle-object virtual deleting cleanup (flag 3).
    void *obj = *(void **)((char *)this + 0x84);
    if (obj != 0) {
        char *type = *(char **)((char *)obj + 4);
        DeleteRecord *slot = (DeleteRecord *)(type + 0x50);
        __asm__ volatile("" ::: "memory");
        char *base = (char *)obj + slot->offset;
        void (*fn)(void *, void *) = slot->fn;
        fn(base, (void *)3);
        *(void **)((char *)this + 0x84) = 0;
    }

    eHeightmapTemplate__PlatformFree_void(this);

    // Inlined cArrayBase<eHeightmapTile> destruction at +0x7C. eHeightmapTile
    // has a trivial destructor so the per-element loop only counts.
    if (array != 0) {
        void *data = *(void **)((char *)this + 0x7C);
        int count = 0;
        if (data != 0) {
            count = *((int *)data - 1) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (data != 0) {
            int *alloc = (int *)data - 1;
            if (alloc != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(alloc);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *fbase = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(fbase, alloc);
            }
            *(void **)((char *)this + 0x7C) = 0;
        }
    }

    if (this != 0) {
        *(void **)((char *)this + 4) = (void *)0x380C18;
        cObject_dtor(this, 0);
    }
}

#pragma control sched=2
