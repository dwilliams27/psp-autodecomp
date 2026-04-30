#pragma control sched=1
#include "eShadow.h"
#include "cFile.h"

extern char eShadowvirtualtable[];

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T> T *dcast(const cBase *);

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
extern cType *D_00046C58;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

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

// eShadow::Read(cFile &, cMemPool *) — 0x0007EB80
int eShadow::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
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

void eShadow::AssignCopy(const cBase *base) {
    eShadow *other = dcast<eShadow>(base);
    *(v4sf_t *)((char *)this + 0x40) = *(const v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(const v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(const v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);
    float value5C = *(const float *)((char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = value5C;
    *(int *)((char *)this + 0x60) = *(const int *)((char *)other + 0x60);
    *(int *)((char *)this + 0x64) = *(const int *)((char *)other + 0x64);
    *(int *)((char *)this + 0x68) = *(const int *)((char *)other + 0x68);
    *(int *)((char *)this + 0x6C) = *(const int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(const int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(const float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(const float *)((char *)other + 0x78);
    *(int *)((char *)this + 0x7C) = *(const int *)((char *)other + 0x7C);
    *(int *)((char *)this + 0x80) = *(const int *)((char *)other + 0x80);
    *(int *)((char *)this + 0x84) = *(const int *)((char *)other + 0x84);
    *(int *)((char *)this + 0x88) = *(const int *)((char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((char *)other + 0x8D);
    *(v4sf_t *)((char *)this + 0xC0) = *(const v4sf_t *)((char *)other + 0xC0);
    *(v4sf_t *)((char *)this + 0x90) = *(const v4sf_t *)((char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) = *(const v4sf_t *)((char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) = *(const v4sf_t *)((char *)other + 0xB0);
    *(short *)((char *)this + 0xD0) = *(const short *)((char *)other + 0xD0);
    *(unsigned char *)((char *)this + 0xD2) =
        *(const unsigned char *)((char *)other + 0xD2);
    *(float *)((char *)this + 0xD4) = *(const float *)((char *)other + 0xD4);
    __asm__ volatile("" ::: "memory");
    int *dstD8 = (int *)((char *)this + 0xD8);
    int *srcD8 = (int *)((char *)other + 0xD8);
    *dstD8 = *srcD8;
    *(int *)((char *)this + 0xDC) = *(const int *)((char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) = *(const int *)((char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) = *(const int *)((char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) = *(const int *)((char *)other + 0xE8);
}

const cType *eShadow::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046C58 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = eShadow::New;
        __asm__ volatile("" : "+r"(factory));
        D_00046C58 = cType::InitializeType(0, 0, 0x56, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C58;
}
