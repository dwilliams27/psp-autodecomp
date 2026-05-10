// gcStateMachine::~gcStateMachine(void) @ 0x0010cdd8, 360B
//
// Split TU per CLAUDE.md SPLIT-TU rule. Local redeclaration of gcStateMachine
// here has no base class so SNC does not auto-emit cObject base destructor;
// the body explicitly calls cObject_dtor via the SNC-mangled symbol. Global
// inline operator delete provides the deleting tail. The class layout in
// src/gcStateMachine.cpp uses different types at offsets 0x44/0x48/0x4C/0x68
// but the destructor only operates on offsets 0x50/0x58/0x60 (cBaseArrays)
// and 0x68 (cArrayBase pointer) — both views are byte-compatible at those
// offsets.
//
// ODR-WARNING: gcStateMachine is also defined in src/gcStateMachine.cpp with
// a cObject-inherited layout. The two TUs reference disjoint method sets and
// agree on the field offsets that both touch.

class cBase;
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

class cBaseArray {
public:
    void RemoveAll(void);
};

extern "C" void cObject_dtor(cObject *, int) asm("__0oHcObjectdtv");

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
}

class gcStateMachine {
public:
    ~gcStateMachine();
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oOgcStateMachinedtv, 0x168\n");

gcStateMachine::~gcStateMachine() {
    *(void **)((char *)this + 4) = (void *)0x388448;
    void *array68 = (char *)this + 0x68;
    void *array60 = (char *)this + 0x60;
    void *array58 = (char *)this + 0x58;
    void *array50 = (char *)this + 0x50;

    if (array68 != 0) {
        void *entries = *(void **)((char *)this + 0x68);
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
                DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x68) = 0;
        }
    }
    if (array60 != 0) {
        ((cBaseArray *)array60)->RemoveAll();
    }
    if (array58 != 0) {
        ((cBaseArray *)array58)->RemoveAll();
    }
    if (array50 != 0) {
        ((cBaseArray *)array50)->RemoveAll();
    }

    cObject_dtor((cObject *)this, 0);
}
