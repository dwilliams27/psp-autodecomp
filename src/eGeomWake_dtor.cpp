// eGeomWake_dtor.cpp - split TU for the deleting destructor.
// ODR-WARNING: this TU re-declares eGeomWake locally with only the destructor
// + operator delete. The rest of the class lives in src/eGeomWake.cpp.
// Adding a destructor declaration to that file's class could perturb the
// matched siblings' register allocation.

#pragma control sched=1

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void eDynamicGeom___dtor_eDynamicGeom_void(void *, int);

class eGeomWake {
public:
    ~eGeomWake();
    static void operator delete(void *p);
};

extern char eGeomWakevirtualtable[];

inline void eGeomWake::operator delete(void *p) {
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

eGeomWake::~eGeomWake() {
    *(void **)((char *)this + 4) = eGeomWakevirtualtable;
    void *arrAddr = (char *)this + 0xF4;
    if (arrAddr != 0) {
        void *entries = *(void **)((char *)this + 0xF4);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            int byteOff = 0;
            do {
                char *entry = (char *)entries + byteOff;
                if (entry != 0) {
                    char *nextAddr = entry + 4;
                    if (nextAddr != 0) {
                        void *inner = *(void **)(entry + 4);
                        int innerCount = 0;
                        if (inner != 0) {
                            innerCount = *(int *)((char *)inner - 4) & 0x3FFFFFFF;
                        }
                        int j = 0;
                        if (j < innerCount) {
                            do {
                                j++;
                            } while (j < innerCount);
                        }
                        if (inner != 0) {
                            char *basePtr = (char *)inner -
                                (((unsigned int)*(int *)((char *)inner - 4) >> 30) * 4) - 4;
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
                            *(void **)(entry + 4) = 0;
                        }
                    }
                }
                entries = *(void **)((char *)this + 0xF4);
                i++;
                byteOff += 8;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
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
            *(void **)((char *)this + 0xF4) = 0;
        }
    }
    eDynamicGeom___dtor_eDynamicGeom_void(this, 0);
}
