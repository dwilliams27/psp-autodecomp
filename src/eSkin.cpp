// eSkin.cpp - decompiled from eAll_psp.obj.
//
// Functions:
//   0x001f1724 eSkin::New(cMemPool *, cBase *) static  144B

class cBase;
class cMemPool;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eSkin {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void cObject_cObject(void *self, cBase *parent);

extern char eSkinclassdesc[];

#pragma control sched=1
// -- eSkin::New @ 0x001f1724 --
cBase *eSkin::New(cMemPool *pool, cBase *parent) {
    eSkin *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSkin *obj = (eSkin *)entry->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = eSkinclassdesc;
        *(int *)((char *)obj + 0x44) = 0;
        *(int *)((char *)obj + 0x48) = 0;
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
