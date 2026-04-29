// Mixed small functions routed into this TU by the orchestrator.

class cBase;
class cFile;
class cMemPool;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *,
                                 const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

inline void *operator new(unsigned int, void *p) { return p; }

// ============================================================
// eVolume::UpdateLocalToWorld(void) @ 0x0005D928
// ============================================================

class eWorld;

class eVolume {
public:
    char _pad0[0x20];
    eWorld *mWorld;

    eVolume(cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void UpdateLocalToWorld(void);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

class eWorld {
public:
    void UpdateVolumeLocation(eVolume *);
};

#pragma control sched=1
void eVolume::UpdateLocalToWorld(void) {
    eWorld *world = mWorld;
    if (world != 0) {
        world->UpdateVolumeLocation(this);
    }
}
#pragma control sched=2

// ============================================================
// eVolume::Write(cFile &) const @ 0x0005D4A8
// ============================================================

#pragma control sched=1
void eVolume::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.Write(*(const float *)((const char *)this + 0xC));
    wb.Write(3, (const float *)((const char *)this + 0x10));
    wb.Write(3, (const float *)((const char *)this + 0x60));
    wb.Write(3, (const float *)((const char *)this + 0x30));
    wb.Write(3, (const float *)((const char *)this + 0x40));
    wb.Write(3, (const float *)((const char *)this + 0x50));
    wb.End();
}
#pragma control sched=2

// ============================================================
// eVolume::AssignCopy(const cBase *) @ 0x0020502C
// ============================================================

#pragma control sched=1
void eVolume::AssignCopy(const cBase *src) {
    eVolume *other = dcast<eVolume>(src);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(float *)((char *)this + 0xC) = *(float *)((char *)other + 0xC);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(int *)((char *)this + 0x20) = *(int *)((char *)other + 0x20);
    *(int *)((char *)this + 0x24) = *(int *)((char *)other + 0x24);
    *(v4sf_t *)((char *)this + 0x60) = *(v4sf_t *)((char *)other + 0x60);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x50) = *(v4sf_t *)((char *)other + 0x50);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(unsigned char *)((char *)this + 0x74) =
        *(unsigned char *)((char *)other + 0x74);
    *(unsigned char *)((char *)this + 0x75) =
        *(unsigned char *)((char *)other + 0x75);
    *(int *)((char *)this + 0x78) = *(int *)((char *)other + 0x78);
    *(int *)((char *)this + 0x7C) = *(int *)((char *)other + 0x7C);
    *(int *)((char *)this + 0x80) = *(int *)((char *)other + 0x80);
    *(int *)((char *)this + 0x84) = *(int *)((char *)other + 0x84);
}
#pragma control sched=2

// ============================================================
// eVolume::New(cMemPool *, cBase *) static @ 0x002050D4
// ============================================================

#pragma control sched=1
cBase *eVolume::New(cMemPool *pool, cBase *parent) {
    eVolume *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eVolume *obj = (eVolume *)rec->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eVolume(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

extern const char eVolume_base_name[];
extern const char eVolume_base_desc[];

static cType *type_base;
static cType *type_eVolume;

// ============================================================
// eVolume::GetType(void) const @ 0x00205150
// ============================================================

#pragma control sched=1
const cType *eVolume::GetType(void) const {
    if (!type_eVolume) {
        if (!type_base) {
            const char *name = eVolume_base_name;
            const char *desc = eVolume_base_desc;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            type_base = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *base = type_base;
        __asm__ volatile("" : "+r"(base));
        cBase *(*factory)(cMemPool *, cBase *) = &eVolume::New;
        __asm__ volatile("" : "+r"(factory));
        type_eVolume = cType::InitializeType(0, 0, 0x38, base, factory, 0, 0, 0);
    }
    return type_eVolume;
}
#pragma control sched=2

// ============================================================
// simple vtable-setting constructors
// ============================================================

void gcEntityGeomConfig_gcEntityGeomConfig(void *self, cBase *parent);
void gcEntityControllerTemplate_gcEntityControllerTemplate(void *self, cBase *parent);

class gcDynamicModelConfig {
public:
    gcDynamicModelConfig(cBase *);
};

gcDynamicModelConfig::gcDynamicModelConfig(cBase *parent) {
    gcEntityGeomConfig_gcEntityGeomConfig(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38C5E0;
}

class gcParticleSystemConfig {
public:
    gcParticleSystemConfig(cBase *);
};

gcParticleSystemConfig::gcParticleSystemConfig(cBase *parent) {
    gcEntityGeomConfig_gcEntityGeomConfig(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38CDE8;
}

class gcStationaryControllerTemplate {
public:
    gcStationaryControllerTemplate(cBase *);
};

gcStationaryControllerTemplate::gcStationaryControllerTemplate(cBase *parent) {
    gcEntityControllerTemplate_gcEntityControllerTemplate(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38D330;
}

// ============================================================
// gcValEntityIsActive::AssignCopy(const cBase *) @ 0x003323C4
// ============================================================

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityIsActive {
public:
    void AssignCopy(const cBase *);
};

void gcValEntityIsActive::AssignCopy(const cBase *base) {
    gcValEntityIsActive *other = dcast<gcValEntityIsActive>(base);
    gcDesiredEntity &srcDesired = *(gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(srcDesired);
}
