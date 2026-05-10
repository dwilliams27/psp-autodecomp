// gcDoTableQuery::~gcDoTableQuery(void) @ 0x00307874, 360B
//
// Split TU per CLAUDE.md SPLIT-TU rule. Local redeclaration of gcDoTableQuery
// here exposes only the destructor + class-local operator delete; the rest of
// the class lives in src/gcDoTableQuery.cpp (New/GetType/Write/Evaluate/GetText).
// Adding a destructor declaration there could perturb the matched siblings'
// vtable handling.
//
// ODR-WARNING: gcDoTableQuery is also defined in src/gcDoTableQuery.cpp and
// src/gcDoTableQuery_Read.cpp. The TUs reference disjoint method sets and
// agree on the field offsets touched by the destructor (0x0C, 0x10, 0x14,
// 0x20, 0x28, 0x30, 0x4C, 0x68).

class cBase;

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

extern "C" void gcEvent_dtor(void *, int) asm("__0oHgcEventdtv");
extern "C" void gcAction_dtor(void *, int) asm("__0oIgcActiondtv");

extern char gcDoTableQueryvirtualtable[];
extern char D_00000838[];

class gcDoTableQuery {
public:
    static void operator delete(void *);
    ~gcDoTableQuery(void);
};

inline void gcDoTableQuery::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oOgcDoTableQuerydtv, 0x168\n");

gcDoTableQuery::~gcDoTableQuery(void) {
    *(void **)((char *)this + 4) = gcDoTableQueryvirtualtable;

    gcEvent_dtor((char *)this + 0x68, 2);
    gcEvent_dtor((char *)this + 0x4C, 2);
    gcEvent_dtor((char *)this + 0x30, 2);

    char *p_28 = (char *)this + 0x28;
    char *p_20 = (char *)this + 0x20;
    char *p_C  = (char *)this + 0x0C;

    if ((void *)p_28 != 0) {
        ((cBaseArray *)p_28)->RemoveAll();
    }
    if ((void *)p_20 != 0) {
        ((cBaseArray *)p_20)->RemoveAll();
    }

    if ((void *)p_C != 0) {
        *(void *volatile *)((char *)this + 0x10) = (void *)0x38A000;
        *(void *volatile *)((char *)this + 0x10) = D_00000838;
        *(void *volatile *)((char *)this + 0x10) = (void *)0x3889A8;

        if ((void *)((char *)this + 0x14) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x14);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
        *(void **)((char *)this + 0x10) = (void *)0x37E6A8;
    }

    gcAction_dtor(this, 0);
}
