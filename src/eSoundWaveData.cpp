#pragma control sched=1
// eSoundWaveData: sound wave data class ctor + dtor
// gcDesiredObject: ctor (co-located at orchestrator's request)
// finitef / isnanf: libm helpers (co-located at orchestrator's request)

class cBase;

extern "C" {
    void *cMemPool_GetPoolFromPtr(void *);
    void free(void *);
}

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
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
};

eSoundWaveData::eSoundWaveData() {
    mVolume = 0.0f;
    mUnk4 = 0;
    mUnk8 = 0;
    mResource = 0;
    mUnk10 = 0;
}

extern "C" void eSoundWaveData___dtor_eSoundWaveData_void(eSoundWaveData *self, int flags) {
    if (self != 0) {
        if (self->mResource != 0) {
            void *pool = cMemPool_GetPoolFromPtr(self->mResource);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self->mResource);
            self->mResource = 0;
        }
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
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
