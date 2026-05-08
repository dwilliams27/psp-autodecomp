// ODR-WARNING: this TU intentionally redeclares eTwoPassModelMtl with a
// minimal member list because include/eTwoPassModelMtl.h does not yet declare
// Read. Keep this split from src/eTwoPassModelMtl.cpp so existing matches in
// the canonical TU are not perturbed.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class eModelMtl {
public:
    int Read(cFile &, cMemPool *);
};

class eTwoPassModelMtl {
public:
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void CreateData(void);
};

struct ReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, int, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

// -- eTwoPassModelMtl::Read(cFile &, cMemPool *) @ 0x0008b084 --
int eTwoPassModelMtl::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);

    if ((unsigned int)rb._data[3] != 2) goto fail;
    if (((eModelMtl *)this)->Read(file, pool)) goto success;
    goto fail;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x48);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }

    *(int *)((char *)this + 0x4C) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x4C);
        h->Read(rb, cMemPool::GetPoolFromPtr(h));
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x80);
        ReadSlot *slot = (ReadSlot *)(typeInfo + 0x30);
        short off = slot->offset;
        void *base = (char *)this + 0x7C;
        void *target = (char *)base + off;
        register int f __asm__("$22") = rb._data[0];
        cMemPool *targetPool = cMemPool::GetPoolFromPtr(base);
        slot->fn(target, f, targetPool);
    }

    {
        char *typeInfo = *(char **)((char *)this + 0xF4);
        ReadSlot *slot = (ReadSlot *)(typeInfo + 0x30);
        short off = slot->offset;
        void *base = (char *)this + 0xF0;
        void *target = (char *)base + off;
        register int f __asm__("$22") = rb._data[0];
        cMemPool *targetPool = cMemPool::GetPoolFromPtr(base);
        slot->fn(target, f, targetPool);
    }

    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x74, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x78, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x164, 4);
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}
