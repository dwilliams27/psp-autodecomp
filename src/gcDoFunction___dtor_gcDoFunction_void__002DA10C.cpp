// gcDoFunction::~gcDoFunction(void) — split TU for the deleting destructor.
// Emits the C++ mangled symbol __0oMgcDoFunctiondtv.
// Local re-declaration of gcDoFunction (dtor + operator delete) to keep the
// emitted symbol exact; the matched siblings live in src/gcDoFunction*.cpp.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoFunction {
public:
    static void operator delete(void *);
    ~gcDoFunction(void);
};

extern char gcDoFunctionvirtualtable[];
void gcAction___dtor_gcAction_void(void *, int);
void cBaseArray_RemoveAll(void *);

inline void gcDoFunction::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oMgcDoFunctiondtv, 0xec\n");

gcDoFunction::~gcDoFunction(void) {
    *(void **)((char *)this + 4) = gcDoFunctionvirtualtable;
    void *arr = (char *)this + 0x10;
    if ((char *)this + 0x18 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x18) = 0;
        }
    }
    if (arr != 0) {
        cBaseArray_RemoveAll(arr);
    }
    gcAction___dtor_gcAction_void(this, 0);
}
