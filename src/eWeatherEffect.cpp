class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eWeatherEffect;
class eCamera;
class mFrustum;
class eCameraBins;
class eMaterial;
template <class T> class cHandleT;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eGeom {
public:
    int base;

    ~eGeom();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct DeleteEntry {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eWeatherEffect : public eGeom {
public:
    static cBase *New(cMemPool *, cBase *);
    ~eWeatherEffect();
    static void operator delete(void *p) {
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

    void AssignCopy(const cBase *);
    void Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *,
              unsigned int, int, const cHandleT<eMaterial> *, float) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern "C" {
    void eWeatherEffect__eWeatherEffect_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *);
};

struct WeatherTemplateView {
    char pad00[0x70];
    unsigned int effectMaterial;
};

struct WeatherEffectMtlView {
    char pad00[0x30];
    unsigned int handle;
    char pad34[0x10];
    int hasDepth;
    unsigned int material;
    char pad4C[4];
    int sortKey;
    char pad54[8];
    unsigned char flags;
    unsigned char mask;
    char pad5E;
    signed char binIndex;
};

struct WeatherBinRecord {
    unsigned short flags;
    unsigned short materialId;
    unsigned short zero4;
    unsigned char category;
    unsigned char ff;
    int sortKey;
    WeatherEffectMtlView *effectMtl;
    const eWeatherEffect *effect;
    const eCamera *camera;
    float alpha;
    unsigned short count;
    unsigned short zero1E;
};

class eWeatherParticle;

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_cBase asm("D_000385DC");
static cType *type_eGeom asm("D_00040FF4");
static cType *type_eWeatherEffect asm("D_00046B9C");

extern char eWeatherEffectvirtualtable[];

#pragma control sched=1

// ── eWeatherEffect::~eWeatherEffect(void) @ 0x00060810 ──
eWeatherEffect::~eWeatherEffect() {
    *(void **)((char *)this + 4) = eWeatherEffectvirtualtable;
    void *field = (char *)this + 0x90;
    if (field != 0) {
        void *entries = *(void **)((char *)this + 0x90);
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
            char *basePtr = (char *)entries - (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) - 4;
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
            *(void **)((char *)this + 0x90) = 0;
        }
    }
}

#pragma control sched=1

// ── eWeatherEffect::Cull(...) const @ 0x00060b80 ──
void eWeatherEffect::Cull(unsigned int stamp, const eCamera &camera,
                          const mFrustum &frustum, eCameraBins *bins,
                          unsigned int, int mode,
                          const cHandleT<eMaterial> *material,
                          float alpha) const {
    unsigned int oldStamp = *(const unsigned int *)((const char *)this + 0x88);
    if (stamp == oldStamp) {
        return;
    }
    *(unsigned int *)((char *)this + 0x88) = stamp;

    if ((mode | (int)material) != 0) {
        return;
    }

    WeatherTemplateView *weatherTemplate =
        *(WeatherTemplateView **)((const char *)this + 0x60);
    if (weatherTemplate == 0) {
        return;
    }

    unsigned int handle = weatherTemplate->effectMaterial;
    WeatherEffectMtlView *effectMtl = 0;
    WeatherEffectMtlView *candidate = 0;
    if (handle == 0) {
        effectMtl = 0;
    } else {
        unsigned int offset = (handle & 0xFFFF) << 2;
        char *table = (char *)0x38890;
        void *slot;
        __asm__ volatile("addu %0,%1,%2" : "=r"(slot) : "r"(offset), "r"(table));
        WeatherEffectMtlView *entry = *(WeatherEffectMtlView **)slot;
        if (entry != 0) {
            if (entry->handle == handle) {
                candidate = entry;
            }
        }
        effectMtl = candidate;
    }

    WeatherEffectMtlView *mtl = effectMtl;
    if (mtl == 0) {
        return;
    }

    float distanceAlpha = alpha * *(const float *)((const char *)&camera + 0x1F4);
    if (distanceAlpha == 0.0f) {
        return;
    }

    if ((mtl->mask & *(unsigned short *)((char *)bins + 4)) == 0) {
        return;
    }

    int count = *(int *)((char *)bins + 8);
    if (count >= 3000) {
        return;
    }

    unsigned char flags = mtl->flags;
    if ((flags & 0x80) != 0) {
        return;
    }

    unsigned int binFlags = 0;
    if ((flags & 0x20) != 0) {
        binFlags = 8;
    }
    unsigned int alphaFlags = 4;
    if ((flags & 1) != 0) {
        alphaFlags = 0;
    }
    binFlags = (binFlags & 0xFFFF) | alphaFlags;

    int binIndex = mtl->binIndex;
    int nextCount = count + 1;
    *(int *)((char *)bins + 8) = nextCount;

    WeatherBinRecord *record =
        (WeatherBinRecord *)((char *)bins + (count << 5) + 0x0C);
    record->flags = (unsigned short)(binFlags & 0xFFFF);

    if (*(unsigned char *)((char *)bins + 0x1770C) != 0) {
        binIndex = *(int *)((char *)bins + 0x17710);
    }

    record->flags = record->flags | (unsigned short)(binIndex << 9);

    unsigned int category = *(unsigned char *)((char *)bins + 0x18400);
    int *bucket = (int *)((char *)bins + 0x18374 + binIndex * 0x14 + category * 4);
    *bucket = *bucket + 1;

    record->effectMtl = mtl;

    if (distanceAlpha < 1.0f) {
        record->flags = record->flags | 0x10;
    }

    if (mtl->hasDepth != 0) {
        record->flags = record->flags | 0x20;
    }

    unsigned int materialHandle = mtl->material;
    unsigned short materialId = 0;
    char *resolvedMaterial;
    if (materialHandle == 0) {
        resolvedMaterial = 0;
    } else {
        unsigned int offset = (materialHandle & 0xFFFF) << 2;
        char *table = (char *)0x38890;
        void *slot;
        __asm__ volatile("addu %0,%1,%2" : "=r"(slot) : "r"(offset), "r"(table));
        char *entry = *(char **)slot;
        resolvedMaterial = 0;
        if (entry != 0) {
            if (*(unsigned int *)(entry + 0x30) == materialHandle) {
                resolvedMaterial = entry;
            }
        }
    }
    if (resolvedMaterial != 0) {
        materialId = (unsigned short)(materialHandle & 0xFFFF);
    }

    record->materialId = materialId;
    record->sortKey = mtl->sortKey;
    record->zero4 = 0;
    record->effect = this;
    record->camera = &camera;
    record->alpha = distanceAlpha;
    record->category = *(unsigned char *)((char *)bins + 0x18400);
    record->zero1E = 0;
    record->ff = 0xFF;
    record->count = *(int *)((char *)bins + 8);
}

#pragma control sched=1

// ── eWeatherEffect::AssignCopy(const cBase *) @ 0x00207350 ──
void eWeatherEffect::AssignCopy(const cBase *base) {
    const eWeatherEffect *other = 0;

    if (base != 0) {
        if (type_eWeatherEffect == 0) {
            if (type_eGeom == 0) {
                if (type_cBase == 0) {
                    type_cBase = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase,
                                                   0, 0, 0, 0);
            }
            type_eWeatherEffect =
                cType::InitializeType(0, 0, 0x192, type_eGeom,
                                      &eWeatherEffect::New, 0, 0, 0);
        }

        cType *target = type_eWeatherEffect;
        void *classDesc = *(void **)((const char *)base + 4);
        void *entryVoid = (char *)classDesc + 8;
        DispatchEntry *entry = (DispatchEntry *)entryVoid;
        cType *(*fn)(void *);
        short offset = entry->offset;
        __asm__ volatile("" ::: "memory");
        fn = (cType *(*)(void *))entry->fn;
        cType *type = fn((void *)((const char *)base + offset));
        int valid;

        if (target != 0) {
            goto have_target;
        }
        valid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                valid = 1;
            } else {
                type = *(cType **)((char *)type + 0x1C);
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            valid = 0;
        }

cast_done:
        if (valid != 0) {
            other = (const eWeatherEffect *)base;
        }
    }

    *(v4sf_t *)((char *)this + 0x40) = *(const v4sf_t *)((const char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(const v4sf_t *)((const char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(const v4sf_t *)((const char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((const char *)other + 0x30);
    float value5C = *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = value5C;
    *(int *)((char *)this + 0x60) = *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) = *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) = *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) = *(const int *)((const char *)other + 0x6C);
    __asm__ volatile("" ::: "memory");
    *(int *)((char *)this + 0x70) = *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) = *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) = *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) = *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) = *(const int *)((const char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);
    ((cArrayBase<eWeatherParticle> *)((char *)this + 0x90))->operator=(
        *(const cArrayBase<eWeatherParticle> *)((const char *)other + 0x90));
}

// ── eWeatherEffect::Write(cFile &) const @ 0x000606D0 ──
void eWeatherEffect::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eGeom::Write(file);
    wb.End();
}

// ── eWeatherEffect::Read(cFile &, cMemPool *) @ 0x0006071C ──
int eWeatherEffect::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eWeatherEffect::New(cMemPool *, cBase *) static @ 0x0020754C ──
cBase *eWeatherEffect::New(cMemPool *pool, cBase *parent) {
    eWeatherEffect *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eWeatherEffect *obj = (eWeatherEffect *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eWeatherEffect__eWeatherEffect_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eWeatherEffect::GetType(void) const @ 0x002075c8 ──
const cType *eWeatherEffect::GetType(void) const {
    if (!type_eWeatherEffect) {
        if (!type_eGeom) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" ::: "memory");
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase, 0, 0, 0, 0);
        }
        const cType *parentType = type_eGeom;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20754C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eWeatherEffect =
            cType::InitializeType(0, 0, 0x192, parentType, factory, 0, 0, 0);
    }
    return type_eWeatherEffect;
}
