// gcTrigger::~gcTrigger(void) @ 0x0012d464, 332B
//
// Split TU per CLAUDE.md SPLIT-TU rule. Local redeclaration of gcTrigger here
// has no base class so SNC does not auto-emit member/base destructor calls —
// all sub-object destructor calls are explicit in the body to match the
// disassembly's call order (eVolume → pairs cleanup → gcEvent×3 → cObject).
// The global `operator delete` with internal `if (p != 0)` produces the second
// null check that SNC inlines into the deleting-destructor pool-delete tail.
//
// ODR-WARNING: gcTrigger is also defined in src/gcTrigger.cpp with a different
// (cObject-inherited) layout. Layouts are compatible at the field-offset level
// referenced by both TUs.

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

extern "C" void cObject_dtor(cObject *, int) asm("__0oHcObjectdtv");
extern "C" void eVolume_dtor(void *, int) asm("__0oHeVolumedtv");
extern "C" void gcEvent_dtor(void *, int) asm("__0oHgcEventdtv");

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

class gcTrigger {
public:
    ~gcTrigger();
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oJgcTriggerdtv, 0x14C\n");

gcTrigger::~gcTrigger() {
    *(void **)((char *)this + 4) = (void *)0x38A1E0;
    eVolume_dtor((char *)this + 0xB0, 2);

    void *pairsField = (char *)this + 0x98;
    if (pairsField != 0) {
        void *entries = *(void **)((char *)this + 0x98);
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
            void *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x98) = 0;
        }
    }

    gcEvent_dtor((char *)this + 0x7C, 2);
    gcEvent_dtor((char *)this + 0x60, 2);
    gcEvent_dtor((char *)this + 0x44, 2);
    cObject_dtor((cObject *)this, 0);
}
