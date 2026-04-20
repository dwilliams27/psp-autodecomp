#pragma control sched=1
#include "eShadow.h"
#include "cFile.h"

extern char eShadowvirtualtable[];

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

// eShadow::eShadow(cBase *)
eShadow::eShadow(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eShadowvirtualtable;
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 0xD2) |= 4;
}

// eShadow::Write(cFile &) const
void eShadow::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eDynamicGeom::Write(file);
    wb.End();
}

// eShadow::Reset(cMemPool *, bool)
void eShadow::Reset(cMemPool *, bool) {
    *(unsigned char *)((char *)this + 0x8D) = 1;
    __asm__ volatile("" ::: "memory");
    void *p = *(void **)((char *)this + 0x60);
    if (p != 0) {
        *(float *)((char *)this + 0x74) = *(float *)((char *)p + 0x5C);
    } else {
        *(float *)((char *)this + 0x74) = -1.0f;
    }
}

// eShadow::Update(cTimeValue)
void eShadow::Update(cTimeValue) {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}

// eShadow::~eShadow(void)
extern "C" {

void eShadow___dtor_eShadow_void(eShadow *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eShadowvirtualtable;
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
