// gcDoUISendMessage_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcDoUISendMessage locally with only the
// destructor + operator delete; the rest of the class lives across other
// gcDoUISendMessage_*.cpp split TUs. Keep the layout assumptions in sync.

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

class cBaseArray {
public:
    void RemoveAll(void);
};

class gcDoUISendMessage {
public:
    static void operator delete(void *);
    ~gcDoUISendMessage(void);
};

extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoUISendMessagevirtualtable[];

inline void gcDoUISendMessage::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oRgcDoUISendMessagedtv, 0x168\n");

gcDoUISendMessage::~gcDoUISendMessage(void) {
    *(void **)((char *)this + 4) = gcDoUISendMessagevirtualtable;
    char *first = (char *)this + 0x38;
    char *array = (char *)this + 0x30;
    char *outer = (char *)this + 0x18;

    if ((void *)first != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x38);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x38) = 0;
            }
        }
    }

    if ((void *)array != 0) {
        ((cBaseArray *)array)->RemoveAll();
    }

    if ((void *)outer != 0) {
        *(void **)((char *)this + 0x1C) = (void *)0x388568;
        if ((void *)((char *)this + 0x2C) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x2C);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x2C) = 0;
            }
        }
        *(void **)((char *)this + 0x1C) = (void *)0x37E6A8;
    }

    gcAction___dtor_gcAction_void(this, 0);
}
