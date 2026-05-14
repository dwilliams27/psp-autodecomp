// eSurfacePropertyTable_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares eSurfacePropertyTable locally with only the
// destructor + operator delete; the rest of the class lives in
// src/eSurfacePropertyTable.cpp. Keep the layout assumptions in sync if either
// file changes.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cBaseArray {
public:
    void RemoveAll(void);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void cObject_dtor(void *, int) asm("__0oHcObjectdtv");

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
}

class eSurfacePropertyTable {
public:
    ~eSurfacePropertyTable();
};

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oVeSurfacePropertyTabledtv, 0x1e0\n");

#pragma control sched=1

eSurfacePropertyTable::~eSurfacePropertyTable() {
    *(void **)((char *)this + 4) = (void *)0x381DC0;

    if ((char *)this + 0x4C != 0) {
        int outerCount = 0;
        {
            void *e0 = *(void **)((char *)this + 0x4C);
            if (e0 != 0) {
                outerCount = *(int *)((char *)e0 - 4) & 0x3FFFFFFF;
            }
        }
        int i = 0;
        if (i < outerCount) {
            int mask = 0x3FFFFFFF;
            __asm__ volatile("" : "+r"(mask));
            int offset = 0;
            do {
                void *outerEntries = *(void **)((char *)this + 0x4C);
                void **slotAddr = (void **)((char *)outerEntries + offset);
                if (slotAddr != 0) {
                    void *innerEntries = *slotAddr;
                    int innerCount = 0;
                    if (innerEntries != 0) {
                        innerCount = *(int *)((char *)innerEntries - 4) & mask;
                    }
                    int j = 0;
                    if (j < innerCount) {
                        do {
                            j++;
                        } while (j < innerCount);
                    }
                    if (innerEntries != 0) {
                        char *innerBase = (char *)innerEntries - 4;
                        if (innerBase != 0) {
                            cMemPool *pool = cMemPool::GetPoolFromPtr(innerBase);
                            char *block = ((char **)pool)[9];
                            DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                            short off = rec->offset;
                            __asm__ volatile("" ::: "memory");
                            char *base = block + off;
                            void (*fn)(void *, void *) = rec->fn;
                            fn(base, innerBase);
                        }
                        *slotAddr = 0;
                    }
                }
                i += 1;
                offset += 4;
            } while (i < outerCount);
        }
        void *e1 = *(void **)((char *)this + 0x4C);
        if (e1 != 0) {
            char *basePtr = (char *)e1 - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x4C) = 0;
        }
    }

    if ((char *)this + 0x44 != 0) {
        ((cBaseArray *)((char *)this + 0x44))->RemoveAll();
    }

    cObject_dtor(this, 0);
}
