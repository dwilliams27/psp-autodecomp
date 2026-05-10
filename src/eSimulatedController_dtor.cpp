// ODR-WARNING: split translation unit for eSimulatedController destructor only.
// Keep local declarations minimal to avoid register-allocation drift in matched siblings.

class cBaseArray {
public:
    void RemoveAll(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);
extern "C" void ePhysicsController_dtor(void *, int) asm("__0oSePhysicsControllerdtv");

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
}

class eSimulatedController {
public:
    ~eSimulatedController();
};

#pragma control sched=1

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oUeSimulatedControllerdtv, 0x16c\n");

eSimulatedController::~eSimulatedController() {
    *(void **)((char *)this + 4) = (void *)0x3834D8;
    cBaseArray *arr44 = (cBaseArray *)((char *)this + 0x44);
    cBaseArray *arr3C = (cBaseArray *)((char *)this + 0x3C);
    if (arr44 != 0) {
        arr44->RemoveAll();
    }
    if (arr3C != 0) {
        arr3C->RemoveAll();
    }
    void *bodyArr = (char *)this + 0x38;
    if (bodyArr != 0) {
        void *entries = *(void **)((char *)this + 0x38);
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
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x38) = 0;
        }
    }
    ePhysicsController_dtor(this, 0);
}

#pragma control sched=2
