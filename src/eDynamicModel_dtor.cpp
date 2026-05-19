// ODR-WARNING: split translation unit for eDynamicModel destructor only.
// Keep local declarations minimal to avoid perturbing matched siblings in
// src/eDynamicModel.cpp.

#include "eDynamicGeom.h"

extern char eDynamicModelvirtualtable[];

class cBaseArray {
public:
    void RemoveAll(void);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class eDynamicModel : public eDynamicGeom {
public:
    void PlatformFree(void);
    ~eDynamicModel();

    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
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
        ".word 0x00000000\n");

eDynamicModel::~eDynamicModel() {
    register void *vt __asm__("a1") = eDynamicModelvirtualtable;
    __asm__ volatile("" : "+r"(vt));
    void *m114 = *(void **)((char *)this + 0x114);
    *(void **)((char *)this + 4) = vt;
    register char *p148 __asm__("s2") = (char *)this + 0x148;
    register char *p118 __asm__("s3") = (char *)this + 0x118;
    register char *pF8 __asm__("s4") = (char *)this + 0xF8;

    if (m114 != 0) {
        char *type = *(char **)((char *)m114 + 4);
        DeleteRecord *slot = (DeleteRecord *)(type + 0x50);
        short off = slot->offset;
        char *base = (char *)m114 + off;
        void (*fn)(void *, void *) = slot->fn;
        fn(base, (void *)3);
        *(void **)((char *)this + 0x114) = 0;
    }

    PlatformFree();

    if (p148 != 0) {
        void *entries = *(void **)((char *)this + 0x148);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x148) = 0;
        }
    }

    if ((char *)this + 0x134 != 0) {
        char *p138 = (char *)this + 0x138;
        if (p138 != 0) {
            void *entries = *(void **)((char *)this + 0x138);
            int count = 0;
            if (entries != 0) {
                count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
            }
            int i = 0;
            if (i < count) {
                do {
                    i++;
                } while (i < count);
            }
            if (entries != 0) {
                char *basePtr = (char *)entries - 4;
                if (basePtr != 0) {
                    cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                    char *block = ((char **)pool)[9];
                    DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                    short off = rec->offset;
                    char *base = block + off;
                    void (*fn)(void *, void *) = rec->fn;
                    fn(base, basePtr);
                }
                *(void **)((char *)this + 0x138) = 0;
            }
        }
    }

    if ((char *)this + 0x130 != 0) {
        void *entries = *(void **)((char *)this + 0x130);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x130) = 0;
        }
    }

    if ((char *)this + 0x12C != 0) {
        void *entries = *(void **)((char *)this + 0x12C);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x12C) = 0;
        }
    }

    if ((char *)this + 0x128 != 0) {
        void *entries = *(void **)((char *)this + 0x128);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x128) = 0;
        }
    }

    if ((char *)this + 0x124 != 0) {
        void *entries = *(void **)((char *)this + 0x124);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x124) = 0;
        }
    }

    if (p118 != 0) {
        ((cBaseArray *)p118)->RemoveAll();
    }

    char *p110 = (char *)this + 0x110;
    if (p110 != 0) {
        void *entries = *(void **)((char *)this + 0x110);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x110) = 0;
        }
    }

    if (pF8 != 0) {
        void *entries = *(void **)((char *)this + 0xF8);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0xF8) = 0;
        }
    }
}
