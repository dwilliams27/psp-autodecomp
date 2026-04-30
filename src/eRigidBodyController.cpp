// eRigidBodyController — engine sched=1 zone (eAll_psp.obj 0x06e000-0x0bab28).
//
// The constructor (0x000768c8) is matched in src/eInput.cpp using locally
// redeclared classes; this file holds the remaining methods (Write, dtor,
// New). Class is laid out with a tighter eSimulatedController padding than
// include/eSimulatedController.h, sized so eRigidBodyController itself is
// 0x160 bytes (matches the size literal passed by ::New).

class cBase;
class cMemPool;
class cFile;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T> T *dcast(const cBase *);

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

template <class T>
class cArrayBase {
public:
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

struct eSimulatedState;

struct eRigidBodyStateCopy {
    v4sf_t vec0;
    v4sf_t vec1;
    int word20;
    int word24;
    int word28;
    float value2C;
    float value30;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

#pragma control sched=1

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cMemPool_shim {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eSimulatedController {
public:
    char _pad[0x154];
    eSimulatedController(cBase *);
    ~eSimulatedController();
    void Write(cFile &) const;
};

class eRigidBodyController : public eSimulatedController {
public:
    char _extra[0xC];   // brings size to 0x160
    eRigidBodyController(cBase *);
    ~eRigidBodyController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern cType *D_000385DC;
extern cType *D_000469D8;
extern cType *D_00046BD4;
extern cType *D_00046C08;

// ── eRigidBodyController::AssignCopy(const cBase *) @ 0x0020f740 ──
void eRigidBodyController::AssignCopy(const cBase *base) {
    eRigidBodyController *other = dcast<eRigidBodyController>(base);

    *(int *)((char *)this + 0x08) = *(const int *)((char *)other + 0x08);
    *(int *)((char *)this + 0x0C) = *(const int *)((char *)other + 0x0C);
    *(unsigned char *)((char *)this + 0x10) =
        *(const unsigned char *)((char *)other + 0x10);
    *(unsigned char *)((char *)this + 0x11) =
        *(const unsigned char *)((char *)other + 0x11);
    *(unsigned char *)((char *)this + 0x12) =
        *(const unsigned char *)((char *)other + 0x12);
    *(v4sf_t *)((char *)this + 0x20) =
        *(const v4sf_t *)((char *)other + 0x20);
    *(float *)((char *)this + 0x30) =
        *(const float *)((char *)other + 0x30);
    *(int *)((char *)this + 0x34) = *(const int *)((char *)other + 0x34);
    __asm__ volatile("" ::: "memory");

    ((cArrayBase<eSimulatedState> *)((char *)this + 0x38))->operator=(
        *(const cArrayBase<eSimulatedState> *)((char *)other + 0x38));
    ((cBaseArray *)((char *)this + 0x3C))->operator=(
        *(const cBaseArray *)((char *)other + 0x3C));
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));

    unsigned int i = 0;
    eRigidBodyStateCopy *dst =
        (eRigidBodyStateCopy *)((char *)this + 0x50);
    const eRigidBodyStateCopy *src =
        (const eRigidBodyStateCopy *)((char *)other + 0x50);
    do {
        dst->vec0 = src->vec0;
        dst->vec1 = src->vec1;
        int word20 = src->word20;
        dst->word20 = word20;
        __asm__ volatile("" ::: "memory");
        int *dst24 = &dst->word24;
        int *src24 = (int *)&src->word24;
        int word24 = *src24;
        *dst24 = word24;
        dst->word28 = src->word28;
        dst->value2C = src->value2C;
        dst->value30 = src->value30;
        __asm__ volatile("" ::: "memory");
        i++;
        dst = (eRigidBodyStateCopy *)((char *)dst + 0x40);
        src = (const eRigidBodyStateCopy *)((const char *)src + 0x40);
    } while (i < 4);

    *(int *)((char *)this + 0x150) =
        *(const int *)((char *)other + 0x150);
    *(int *)((char *)this + 0x154) =
        *(const int *)((char *)other + 0x154);
    *(int *)((char *)this + 0x158) =
        *(const int *)((char *)other + 0x158);
}

// ── eRigidBodyController::Write(cFile &) const @ 0x000767c0 ──
void eRigidBodyController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eSimulatedController *)this)->Write(file);
    wb.End();
}

// ── eRigidBodyController::~eRigidBodyController(void) @ 0x000768fc ──
eRigidBodyController::~eRigidBodyController() {
    *(void **)((char *)this + 4) = (void *)0x384078;
}

// ── eRigidBodyController::New(cMemPool *, cBase *) static @ 0x0020f850 ──
extern "C" {
    void eRigidBodyController__eRigidBodyController_cBaseptr(void *self, cBase *parent);
}

cBase *eRigidBodyController::New(cMemPool *pool, cBase *parent) {
    eRigidBodyController *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    __asm__ volatile("" ::: "memory");
    eRigidBodyController *obj = (eRigidBodyController *)rec->fn((char *)block + rec->offset, 0x160, 0x10, 0, 0);
    if (obj != 0) {
        eRigidBodyController__eRigidBodyController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eRigidBodyController::GetType(void) const {
    if (D_00046C08 == 0) {
        if (D_00046BD4 == 0) {
            if (D_000469D8 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_000469D8 = cType::InitializeType(0, 0, 0x232, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00046BD4 = cType::InitializeType(0, 0, 0x233, D_000469D8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00046BD4;
        cBase *(*factory)(cMemPool *, cBase *) = eRigidBodyController::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C08 = cType::InitializeType(0, 0, 0x234, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C08;
}
