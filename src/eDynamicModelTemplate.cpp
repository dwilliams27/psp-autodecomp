#include "cObject.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cName;
class eSkin;

template <class T>
class cHandleT {
public:
    int mHandle;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cBaseArray {
public:
    void *mData;
    cBaseArray &operator=(const cBaseArray &);
};

template <class T> T *dcast(const cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct EDMWord {
    int value;
};

extern char eDynamicModelvirtualtable[];
extern char eDynamicModelTemplatevirtualtable[];

class eDynamicModelTemplate : public cObject {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

void eDynamicModelTemplate::AssignCopy(const cBase *base) {
    eDynamicModelTemplate *other = dcast<eDynamicModelTemplate>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    *(EDMWord *)((char *)this + 0x48) =
        *(const EDMWord *)((const char *)other + 0x48);
    ((cArrayBase<cHandleT<eSkin> > *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<cHandleT<eSkin> > *)((const char *)other + 0x4C));
    *(float *)((char *)this + 0x50) = *(const float *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((const char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((const char *)other + 0x58);
    *(int *)((char *)this + 0x5C) = *(const int *)((const char *)other + 0x5C);
    *(volatile unsigned char *)((char *)this + 0x60) = *(const volatile unsigned char *)((const char *)other + 0x60);
    *(volatile unsigned char *)((char *)this + 0x61) = *(const volatile unsigned char *)((const char *)other + 0x61);
    ((cBaseArray *)((char *)this + 0x64))->operator=(
        *(const cBaseArray *)((const char *)other + 0x64));
    ((cBaseArray *)((char *)this + 0x6C))->operator=(
        *(const cBaseArray *)((const char *)other + 0x6C));
    *(volatile int *)((char *)this + 0x74) = *(const volatile int *)((const char *)other + 0x74);
    ((cArrayBase<cName> *)((char *)this + 0x78))->operator=(
        *(const cArrayBase<cName> *)((const char *)other + 0x78));
    ((cArrayBase<short> *)((char *)this + 0x7C))->operator=(
        *(const cArrayBase<short> *)((const char *)other + 0x7C));
}

cBase *eDynamicModelTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicModelTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eDynamicModelTemplate *obj =
        (eDynamicModelTemplate *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        new ((cObject *)obj) cObject(parent);
        *(void **)((char *)obj + 4) = eDynamicModelvirtualtable;
        *(float *)((char *)obj + 0x44) = 1000.0f;
        __asm__ volatile("" ::: "memory");
        *(void **)((char *)obj + 4) = eDynamicModelTemplatevirtualtable;
        *(int *)((char *)obj + 0x48) = 0;
        *(int *)((char *)obj + 0x4C) = 0;
        *(float *)((char *)obj + 0x50) = 1000.0f;
        __asm__ volatile("" ::: "memory");
        *(float *)((char *)obj + 0x54) = 0.0f;
        *(float *)((char *)obj + 0x58) = 3.0f;
        __asm__ volatile("" ::: "memory");
        *(int *)((char *)obj + 0x5C) = 0;
        *(unsigned char *)((char *)obj + 0x60) = 0;
        *(unsigned char *)((char *)obj + 0x61) = 0;
        *(int *)((char *)obj + 0x64) = 0;
        *(eDynamicModelTemplate **)((char *)obj + 0x68) = obj;
        *(int *)((char *)obj + 0x6C) = 0;
        *(eDynamicModelTemplate **)((char *)obj + 0x70) = obj;
        *(int *)((char *)obj + 0x74) = 0;
        *(int *)((char *)obj + 0x78) = 0;
        *(int *)((char *)obj + 0x7C) = 0;
        result = obj;
    }
    return (cBase *)result;
}
