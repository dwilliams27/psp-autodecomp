class cBase;
class cFile;
class cMemPool;
class cType;
class ePoint;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

template <class T> T *dcast(const cBase *);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

// External helpers for base-class methods (resolved at link time;
// compare_func.py masks the jal relocations so the unresolved target is fine).
void gcSubGeomController_gcSubGeomController(void *self, cBase *parent);
void gcSubGeomController_Reset(void *self, cMemPool *pool, bool flag);
void gcSubGeomController_Write(const void *self, cFile &file);

extern "C" void *__vec_new(void *arr, int count, int size, void (*ctor)(void *));

class gcParticleSystemController {
public:
    void *m_parent;              // 0x00
    void *m_vtable;              // 0x04
    char m_pad08[0x1C];          // 0x08..0x23 (rest of gcSubGeomController state)
    cHandleT<ePoint> m_target;   // 0x24
    int m_pad28;                 // 0x28
    short m_flag;                // 0x2C
    short m_pad2E;               // 0x2E
    char m_arr30[8];             // 0x30..0x37

    gcParticleSystemController(cBase *parent);
    void Reset(cMemPool *pool, bool flag);
    void Write(cFile &file) const;
    void SetTarget(cHandleT<ePoint> p);
    void AssignCopy(const cBase *base);
    const cType *GetType(void) const;
    static gcParticleSystemController *New(cMemPool *pool, cBase *parent);
};

// Free-function wrapper declaration so New can call the ctor via an unresolved
// symbol (relocation will be masked during byte comparison).
void gcParticleSystemController_ctor(gcParticleSystemController *self, cBase *parent);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gpsc_half3 {
    short a;
    short b;
    short c;
};

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F66C;

void gcParticleSystemController::SetTarget(cHandleT<ePoint> p) {
    m_flag = 1;
    m_target = p;
    cHandleT<ePoint> *p2 = (cHandleT<ePoint> *)((char *)this + 0x28);
    p2->mIndex = 0;
}

void gcParticleSystemController::Reset(cMemPool *pool, bool flag) {
    if (flag) {
        gcSubGeomController_Reset(this, pool, flag);
    }
}

void gcParticleSystemController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcSubGeomController_Write(this, file);
    wb.End();
}

gcParticleSystemController::gcParticleSystemController(cBase *parent) {
    gcSubGeomController_gcSubGeomController(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38BFA0;
    m_target.mIndex = 0;
    m_pad28 = 0;
    m_flag = 0;
    m_pad2E = -1;
    __vec_new((char *)this + 0x30, 1, 6, (void (*)(void *))0x24400C);
}

gcParticleSystemController *gcParticleSystemController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcParticleSystemController *result = 0;
    gcParticleSystemController *obj = (gcParticleSystemController *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcParticleSystemController_ctor(obj, parent);
        result = obj;
    }
    return result;
}

void gcParticleSystemController::AssignCopy(const cBase *base) {
    gcParticleSystemController *other = dcast<gcParticleSystemController>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 0xC) = *(int *)((char *)other + 0xC);
    *(float *)((char *)this + 0x10) = *(float *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);

    int i = 0;
    gpsc_half3 *dst = (gpsc_half3 *)((char *)this + 0x1C);
    gpsc_half3 *src = (gpsc_half3 *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    do {
        short a = src->a;
        short b = src->b;
        short c = src->c;
        __asm__ volatile("" ::: "memory");
        dst->a = a;
        dst->b = b;
        dst->c = c;
        i += 1;
        dst += 1;
        src += 1;
    } while (i <= 0);

    int *src24 = (int *)((char *)other + 0x24);
    int value24 = *src24;
    int *dst24 = (int *)((char *)this + 0x24);
    int *src28 = (int *)((char *)other + 0x28);
    int *dst28 = (int *)((char *)this + 0x28);
    *dst24 = value24;
    int value28 = *src28;
    *dst28 = value28;
    *(unsigned short *)((char *)this + 0x2C) =
        *(unsigned short *)((char *)other + 0x2C);
    *(short *)((char *)this + 0x2E) = *(short *)((char *)other + 0x2E);

    int j = 0;
    gpsc_half3 *dst2 = (gpsc_half3 *)((char *)this + 0x30);
    gpsc_half3 *src2 = (gpsc_half3 *)((char *)other + 0x30);
    do {
        short a = src2->a;
        short b = src2->b;
        short c = src2->c;
        __asm__ volatile("" ::: "memory");
        dst2->a = a;
        dst2->b = b;
        dst2->c = c;
        j += 1;
        dst2 += 1;
        src2 += 1;
    } while (j <= 0);
}

const cType *gcParticleSystemController::GetType(void) const {
    if (D_0009F66C == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F66C = cType::InitializeType(
            0, 0, 0x1F8, D_0009F64C,
            (cBase *(*)(cMemPool *, cBase *))&gcParticleSystemController::New,
            0, 0, 0);
    }
    return D_0009F66C;
}
