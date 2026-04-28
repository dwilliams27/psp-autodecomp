#pragma control sched=1
#include "eShadow.h"
#include "cFile.h"

extern char eShadowvirtualtable[];

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct AllocRecord {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
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
eShadow::~eShadow() {
    *(void **)((char *)this + 4) = eShadowvirtualtable;
}

cBase *eShadow::New(cMemPool *pool, cBase *parent) {
    eShadow *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadow *obj = (eShadow *)rec->fn(base, 0xF0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eShadow(parent);
        result = obj;
    }
    return (cBase *)result;
}
