#pragma control sched=1
// eSoundWaveData: sound wave data class ctor + dtor
// gcDesiredObject: ctor (co-located at orchestrator's request)
// finitef / isnanf: libm helpers (co-located at orchestrator's request)

class cBase;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" {
    void free(void *);
}

struct DeleteRecord {
    short offset;
    short _pad;
    void *fn;
};

// ==========================================================================
// eSoundWaveData
// ==========================================================================

class eSoundWaveData {
public:
    float mVolume;
    int mUnk4;
    int mUnk8;
    void *mResource;
    int mUnk10;

    eSoundWaveData();
    ~eSoundWaveData();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec =
                (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            __asm__ volatile("" : "+r"(rec));
            short off = rec->offset;
            void *base = (char *)block + off;
            __asm__ volatile("" : "+r"(base));
            void *fn = rec->fn;
            ((void (*)(void *, void *, void *))fn)(base, p, fn);
        } else {
            free(p);
        }
    }
};

eSoundWaveData::eSoundWaveData() {
    mVolume = 0.0f;
    __asm__ volatile("" ::: "memory");
    mUnk4 = 0;
    mUnk8 = 0;
    mResource = 0;
    mUnk10 = 0;
}

eSoundWaveData::~eSoundWaveData() {
    if (mResource != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(mResource);
        void *block = *(void **)((char *)pool + 0x24);
        DeleteRecord *rec =
            (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        __asm__ volatile("" : "+r"(rec));
        short off = rec->offset;
        void *base = (char *)block + off;
        __asm__ volatile("" : "+r"(base));
        void *fn = rec->fn;
        ((void (*)(void *, void *, short, void *))fn)(base, mResource, off, fn);
        mResource = 0;
    }
}

#pragma control sched=2

// ==========================================================================
// gcDesiredObject
// ==========================================================================

extern char gcDesiredObjectvirtualtable[];

class gcDesiredObject {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcDesiredObject(cBase *);
};

gcDesiredObject::gcDesiredObject(cBase *b) {
    mOwner = b;
    mVTable = gcDesiredObjectvirtualtable;
    mNext = (unsigned int)(unsigned long)this | 1u;
}

// ==========================================================================
// libm helpers
// ==========================================================================

extern "C" {

unsigned int finitef(float x) {
    union { float f; unsigned int i; } u;
    u.f = x;
    return ((u.i & 0x7FFFFFFFu) - 0x7F800000u) >> 31;
}

unsigned int isnanf(float x) {
    union { float f; unsigned int i; } u;
    u.f = x;
    return (0x7F800000u - (u.i & 0x7FFFFFFFu)) >> 31;
}

}
