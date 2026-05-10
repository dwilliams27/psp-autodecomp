// gcDesiredObjectT<gcDesiredTrigger, gcDesiredTriggerHelper, gcTrigger>::~gcDesiredObjectT(void)
// @ 0x002788bc, 240B, gcAll_psp.obj
//
// Split TU per CLAUDE.md SPLIT-TU rule. Local template redeclaration of
// gcDesiredObjectT here exposes only the destructor + class-local operator
// delete; the rest of the class lives in
// src/gcDesiredObjectTgcDesiredTriggergcDesiredTriggerHelpergcTrigger.cpp
// (GetText, GetDesiredType). Each TU's explicit instantiation only emits the
// methods declared locally — symbols are disjoint, so the linker is happy.
//
// ODR-WARNING: gcDesiredObjectT<gcDesiredTrigger, gcDesiredTriggerHelper,
// gcTrigger> is also declared in the sibling TU above with a different
// member list. The two TUs reference disjoint method sets and agree on the
// field offsets touched by the destructor.

class cBase;
class gcDesiredTrigger;
class gcDesiredTriggerHelper;
class gcTrigger;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

extern char gcDesiredObjectvirtualtable[];
extern char cBaseclassdesc[];

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

template <class T1, class T2, class T3>
class gcDesiredObjectT {
public:
    ~gcDesiredObjectT(void);
    static void operator delete(void *);
};

template <class T1, class T2, class T3>
inline void gcDesiredObjectT<T1, T2, T3>::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    } else {
        free(p);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oQgcDesiredObjectT76QgcDesiredTrigger6WgcDesiredTriggerHelper6JgcTrigger_dtv, 0xf0\n");

template <class T1, class T2, class T3>
gcDesiredObjectT<T1, T2, T3>::~gcDesiredObjectT(void) {
    *(char **)((char *)this + 4) = gcDesiredObjectvirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

template gcDesiredObjectT<gcDesiredTrigger, gcDesiredTriggerHelper, gcTrigger>::~gcDesiredObjectT(void);
