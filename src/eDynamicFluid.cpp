#include "eDynamicFluid.h"
#include "cBase.h"
#include "cFile.h"

extern char eDynamicFluidvirtualtable[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_00046B24;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

void eDynamicGeom___dtor_eDynamicGeom_void(void *, int);
void *cMemPool_GetPoolFromPtr(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct VCall1 {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

// eDynamicFluid::eDynamicFluid(cBase *) — 0x0005DD88
eDynamicFluid::eDynamicFluid(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eDynamicFluidvirtualtable;
    *(int *)((char *)this + 0xF0) = 0;
}

// eDynamicFluid::Write(cFile &) const — 0x0005DC80
void eDynamicFluid::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eDynamicGeom::Write(file);
    wb.End();
}

#pragma control sched=1
// eDynamicFluid::Update(cTimeValue) — 0x0005DE70
void eDynamicFluid::Update(cTimeValue) {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}

inline void *operator new(unsigned int, void *p) { return p; }

#pragma control sched=2
// eDynamicFluid::New(cMemPool *, cBase *) — 0x002057A4
cBase *eDynamicFluid::New(cMemPool *pool, cBase *parent) {
    eDynamicFluid *result = 0;
    void *block = *(void **)((char *)pool + 0x24);
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    void *(*fn)(void *, int, int, int, int) = entry->fn;
    eDynamicFluid *obj = (eDynamicFluid *)fn(base, 0x100, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eDynamicFluid(parent);
        result = obj;
    }
    return (cBase *)result;
}

// eDynamicFluid::GetType(void) const — 0x00205820
#pragma control sched=1
const cType *eDynamicFluid::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046B24 == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        __asm__ volatile("" : "+r"(parentType));
        __asm__ volatile("" ::: "memory");
        cBase *(*factory)(cMemPool *, cBase *) = eDynamicFluid::New;
        __asm__ volatile("" : "+r"(factory));
        D_00046B24 = cType::InitializeType(0, 0, 0x259, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B24;
}
#pragma control sched=2

#pragma control sched=1
// eDynamicFluid::~eDynamicFluid(void) — 0x0005DDC0
extern "C" {

void eDynamicFluid___dtor_eDynamicFluid_void(eDynamicFluid *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eDynamicFluidvirtualtable;
        __asm__ volatile("" ::: "memory");
        void *fluid = *(void **)((char *)self + 0xF0);
        if (fluid != 0) {
            VCall1 *rec = (VCall1 *)(*(char **)((char *)fluid + 4) + 0x50);
            short adj = rec->offset;
            __asm__ volatile("" ::: "memory");
            void *fbase = (char *)fluid + adj;
            void (*fn)(void *, int) = rec->fn;
            fn(fbase, 3);
            *(void **)((char *)self + 0xF0) = 0;
        }
        eDynamicGeom___dtor_eDynamicGeom_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, self);
        }
    }
}

}
