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
    void Write(bool);
    void Write(float);
    void End(void);
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eCameraEffectLayout {
    char pad[0x4C];
    bool mField4C;
    bool mField4D;
    bool mField4E;
    char pad4F;
    float mField50;
    float mField54;
    float mField58;
    float mField5C;
};

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class eGeom {
public:
    eGeom(cBase *);
};

struct eCameraEffectDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct eCameraEffectPoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct eCameraEffectAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char eCameraEffectvirtualtable[];
extern char eCameraEffectGeomvirtualtable[];

extern "C" void cObject_cObject(void *, cBase *);
extern "C" void eGeom_eGeom(void *, cBase *);
extern "C" void eGeom___dtor_eGeom_void(void *, int);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00041008;

class eCameraEffect : public cObject {
public:
    static cBase *New(cMemPool *, cBase *);
    ~eCameraEffect();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        eCameraEffectDeleteRecord *rec =
            (eCameraEffectDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// 0x001e31f0 - eCameraEffect::New(cMemPool *, cBase *) static
cBase *eCameraEffect::New(cMemPool *pool, cBase *parent) {
    eCameraEffect *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((eCameraEffectPoolBlock *)block)->allocTable;
    eCameraEffectAllocEntry *entry = (eCameraEffectAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eCameraEffect *obj = (eCameraEffect *)entry->fn(base, 0xF0, 0x10, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        *(void **)((char *)obj + 4) = eCameraEffectvirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        *(void **)((char *)obj + 0x48) = obj;
        *(unsigned char *)((char *)obj + 0x4C) = 0;
        *(unsigned char *)((char *)obj + 0x4D) = 0;
        *(unsigned char *)((char *)obj + 0x4E) = 0;
        *(float *)((char *)obj + 0x50) = 0.0f;
        *(float *)((char *)obj + 0x54) = 1.0f;
        *(float *)((char *)obj + 0x58) = 10.0f;
        *(float *)((char *)obj + 0x5C) = 30.0f;
        eGeom_eGeom((char *)obj + 0x60, (cBase *)obj);
        *(void **)((char *)obj + 0x64) = eCameraEffectGeomvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x001e30d4 - eCameraEffect::AssignCopy(const cBase *)
void eCameraEffect::AssignCopy(const cBase *base) {
    eCameraEffect *other = dcast<eCameraEffect>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));
    *(unsigned char *)((char *)this + 0x4C) =
        *(const unsigned char *)((char *)other + 0x4C);
    *(unsigned char *)((char *)this + 0x4D) =
        *(const unsigned char *)((char *)other + 0x4D);
    *(unsigned char *)((char *)this + 0x4E) =
        *(const unsigned char *)((char *)other + 0x4E);
    *(float *)((char *)this + 0x50) = *(const float *)((char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(const float *)((char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0xA0) = *(const v4sf_t *)((char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0x70) = *(const v4sf_t *)((char *)other + 0x70);
    *(v4sf_t *)((char *)this + 0x80) = *(const v4sf_t *)((char *)other + 0x80);
    *(v4sf_t *)((char *)this + 0x90) = *(const v4sf_t *)((char *)other + 0x90);
    float valueBC = *(const float *)((char *)other + 0xBC);
    *(v4sf_t *)((char *)this + 0xB0) = *(const v4sf_t *)((char *)other + 0xB0);
    *(float *)((char *)this + 0xBC) = valueBC;
    *(int *)((char *)this + 0xC0) = *(const int *)((char *)other + 0xC0);
    *(int *)((char *)this + 0xC4) = *(const int *)((char *)other + 0xC4);
    *(int *)((char *)this + 0xC8) = *(const int *)((char *)other + 0xC8);
    *(int *)((char *)this + 0xCC) = *(const int *)((char *)other + 0xCC);
    *(int *)((char *)this + 0xD0) = *(const int *)((char *)other + 0xD0);
    *(float *)((char *)this + 0xD4) = *(const float *)((char *)other + 0xD4);
    *(float *)((char *)this + 0xD8) = *(const float *)((char *)other + 0xD8);
    *(int *)((char *)this + 0xDC) = *(const int *)((char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) = *(const int *)((char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) = *(const int *)((char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) = *(const int *)((char *)other + 0xE8);
    *(unsigned char *)((char *)this + 0xEC) =
        *(const unsigned char *)((char *)other + 0xEC);
    *(unsigned char *)((char *)this + 0xED) =
        *(const unsigned char *)((char *)other + 0xED);
}

// SNC emitted a branch-loop pad in this function's symbol range.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// 0x001e3674 - eCameraEffect::~eCameraEffect(void)
eCameraEffect::~eCameraEffect() {
    *(void **)((char *)this + 4) = eCameraEffectvirtualtable;
    eGeom *geom = (eGeom *)((char *)this + 0x60);
    cBaseArray *array = (cBaseArray *)((char *)this + 0x44);
    if (geom != 0) {
        *(void **)((char *)this + 0x64) = eCameraEffectGeomvirtualtable;
        eGeom___dtor_eGeom_void(geom, 0);
    }
    if (array != 0) {
        array->RemoveAll();
    }
}

// 0x0002d258 - eCameraEffect::Write(cFile &) const
void eCameraEffect::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    cObject::Write(file);
    ((const cBaseArray *)((const char *)this + 0x44))->Write(wb);
    const eCameraEffectLayout *self = (const eCameraEffectLayout *)this;
    wb.Write(self->mField4C);
    wb.Write(self->mField4D);
    wb.Write(self->mField4E);
    wb.Write(self->mField50);
    wb.Write(self->mField54);
    wb.Write(self->mField58);
    wb.Write(self->mField5C);
    wb.End();
}

const cType *eCameraEffect::GetType(void) const {
    if (D_00041008 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   (cBase *(*)(cMemPool *, cBase *))&cNamed::New,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00041008 = cType::InitializeType(0, 0, 0x61, D_000385E4,
                                           &eCameraEffect::New,
                                           (const char *)0x36CD94,
                                           (const char *)0x36CDA4, 5);
    }
    return D_00041008;
}
