// ODR-WARNING: split translation unit for eDynamicFluid destructor only.
// Emits __0oNeDynamicFluiddtv (eDynamicFluid::~eDynamicFluid(void),
// deleting variant). 0x0005DDC0, 176B.
//
// Canonical SNC C++ destructor. The ABI auto-generates the (this != 0)
// guard, the chain-call to ~eDynamicGeom, and the deleting-tail dispatch
// through operator delete. We write only the body: the vtable store and
// the +0xF0 fluid-object virtual cleanup.
//
// The trailing self-branch + nop (b $; nop) at 0x0005DE68 is the TU-tail
// padding SNC emits at the end of the object. We reproduce it with the
// same dummy-dtor + .word trick used by src/eDynamicModel_dtor.cpp.

#include "eDynamicGeom.h"

extern char eDynamicFluidvirtualtable[];

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class eDynamicFluid : public eDynamicGeom {
public:
    ~eDynamicFluid();

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

#pragma control sched=1

class zzzzzzzzzzzzzzzz {
public:
    ~zzzzzzzzzzzzzzzz();
};

zzzzzzzzzzzzzzzz::~zzzzzzzzzzzzzzzz() {}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n");

eDynamicFluid::~eDynamicFluid() {
    *(void **)((char *)this + 4) = eDynamicFluidvirtualtable;
    __asm__ volatile("" ::: "memory");
    void *fluid = *(void **)((char *)this + 0xF0);
    if (fluid != 0) {
        char *type = *(char **)((char *)fluid + 4);
        DeleteRecord *slot = (DeleteRecord *)(type + 0x50);
        __asm__ volatile("" ::: "memory");
        char *base = (char *)fluid + (slot->offset);
        void (*fn)(void *, void *) = slot->fn;
        fn(base, (void *)3);
        *(void **)((char *)this + 0xF0) = 0;
    }
}
