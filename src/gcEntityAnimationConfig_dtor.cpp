// gcEntityAnimationConfig_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcEntityAnimationConfig is also defined in
// src/gcEntityAnimationConfig.cpp. This TU redeclares only the destructor and
// operator delete so existing matched methods in the canonical TU keep their
// current codegen.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class gcEntityAnimationConfig {
public:
    static void operator delete(void *);
    ~gcEntityAnimationConfig(void);
};

extern char gcEntityAnimationConfigvirtualtable[];

inline void gcEntityAnimationConfig::operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oXgcEntityAnimationConfigdtv, 0x1e0\n");

gcEntityAnimationConfig::~gcEntityAnimationConfig(void) {
    *(void **)((char *)this + 4) = gcEntityAnimationConfigvirtualtable;

    char *arr = (char *)this + 0x1C;
    char *p14 = (char *)this + 0x14;
    char *p10 = (char *)this + 0x10;
    char *p0C = (char *)this + 0x0C;

    if ((void *)arr != 0) {
        ((cBaseArray *)arr)->RemoveAll();
    }

    if ((void *)p14 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
    }

    if ((void *)p10 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
    }

    if ((void *)p0C != 0) {
        char *data = *(char **)((char *)this + 0x0C);
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }

        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *rec =
                    (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn(block + off, data);
            }
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    if (this != 0) {
        *(void **)((char *)this + 4) = (void *)0x37E6A8;
    }
}
