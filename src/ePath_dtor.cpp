// ODR-WARNING: split translation unit for ePath destructor only.
// Keep local declarations minimal to avoid register-allocation drift in matched siblings.

class cObject;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteEntry {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void cObject_dtor(cObject *, int) asm("__0oHcObjectdtv");

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
}

class ePath {
public:
    ~ePath();
};

#pragma control sched=1

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oFePathdtv, 0x12c\n");

ePath::~ePath() {
    *(void **)((char *)this + 4) = (void *)0x381F70;
    void *field = (char *)this + 0x44;
    if (field != 0) {
        void *entries = *(void **)((char *)this + 0x44);
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
                DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x44) = 0;
        }
    }
    cObject_dtor((cObject *)this, 0);
}

#pragma control sched=2
