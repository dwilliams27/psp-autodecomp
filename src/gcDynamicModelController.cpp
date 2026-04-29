// src/gcDynamicModelController.cpp
//
// Three methods of gcDynamicModelController (gcAll_psp.obj):
//   - Write(cFile &) const                    @ 0x0014a554
//   - ~gcDynamicModelController(void)         @ 0x0014a6a8
//   - New(cMemPool *, cBase *) static         @ 0x002d5eb4
//
// All three follow well-established patterns in the project:
//   - Write   : same as gcGeomCurveController::Write — wrap base-class write in
//               a cWriteBlock(file, 1)/End() pair.
//   - dtor    : canonical SNC destructor (sets vtable; SNC auto-generates the
//               null-guard, chain to ~gcSubGeomController, and deleting-tail
//               dispatch through operator delete). Same shape as
//               eSphereShape::~eSphereShape.
//   - New     : pool[0x24] -> block; block[0x1C] -> allocTable; entry at
//               allocTable+0x28; alloc with size=0x2C, alignment=4. Same shape
//               as gcGeomCurveController::New / eConfigBase::New.

class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

// cWriteBlock — RAII wrapper around the file format's begin/end-block pair.
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

// Base-class write helper (resolved at link time; jal relocation is masked
// during compare_func.py byte comparison). Same convention as
// gcGeomCurveController::Write.
void gcSubGeomController_Write(const void *self, cFile &file);

// Free-function wrapper for the gcDynamicModelController constructor; used by
// New() so we don't need the constructor body in this file. Relocation is
// masked during byte comparison.
void gcDynamicModelController_ctor(void *self, cBase *parent);

// Pool-layout helpers (same struct shapes as eSphereShape.cpp / eConfigBase.cpp).
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolShim {
public:
    static cMemPoolShim *GetPoolFromPtr(const void *);
};

// Local redeclaration of the base class so SNC can auto-generate the chain
// call from ~gcDynamicModelController to ~gcSubGeomController. The pad size
// only affects member access, which this file does not use.
class gcSubGeomController {
public:
    char _pad[0x24];
    gcSubGeomController(cBase *);
    ~gcSubGeomController();
};

class gcDynamicModelController : public gcSubGeomController {
public:
    char _own[8];

    gcDynamicModelController(cBase *);
    ~gcDynamicModelController();
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static gcDynamicModelController *New(cMemPool *, cBase *);

    // operator delete handles the deleting-tail dispatch that SNC emits when
    // the destructor is called with the deleting flag set.
    static void operator delete(void *p) {
        cMemPoolShim *pool = cMemPoolShim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    }
};

extern char gcDynamicModelControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F670;

struct gdm_half3 {
    short a;
    short b;
    short c;
};

// ── gcDynamicModelController::Write(cFile &) const @ 0x0014a554 ──
void gcDynamicModelController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcSubGeomController_Write(this, file);
    wb.End();
}

// ── gcDynamicModelController::~gcDynamicModelController(void) @ 0x0014a6a8 ──
gcDynamicModelController::~gcDynamicModelController() {
    *(void **)((char *)this + 4) = gcDynamicModelControllervirtualtable;
}

// ── gcDynamicModelController::AssignCopy(const cBase *) @ 0x002d5df4 ──
void gcDynamicModelController::AssignCopy(const cBase *src) {
    gcDynamicModelController *other = dcast<gcDynamicModelController>(src);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 0xC) = *(int *)((char *)other + 0xC);
    *(float *)((char *)this + 0x10) = *(float *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);

    int i = 0;
    gdm_half3 *dst = (gdm_half3 *)((char *)this + 0x1C);
    gdm_half3 *from = (gdm_half3 *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
    do {
        short a = from->a;
        short b = from->b;
        short c = from->c;
        __asm__ volatile("" ::: "memory");
        dst->a = a;
        dst->b = b;
        dst->c = c;
        i += 1;
        dst += 1;
        from += 1;
    } while (i <= 0);

    int j = 0;
    gdm_half3 *dst2 = (gdm_half3 *)((char *)this + 0x24);
    gdm_half3 *from2 = (gdm_half3 *)((char *)other + 0x24);
    do {
        short a = from2->a;
        short b = from2->b;
        short c = from2->c;
        __asm__ volatile("" ::: "memory");
        dst2->a = a;
        dst2->b = b;
        dst2->c = c;
        j += 1;
        dst2 += 1;
        from2 += 1;
    } while (j <= 0);
}

// ── gcDynamicModelController::New(cMemPool *, cBase *) static @ 0x002d5eb4 ──
gcDynamicModelController *gcDynamicModelController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDynamicModelController *result = 0;
    gcDynamicModelController *obj = (gcDynamicModelController *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcDynamicModelController_ctor(obj, parent);
        result = obj;
    }
    return result;
}

// ── gcDynamicModelController::GetType(void) const @ 0x002d5f30 ──
const cType *gcDynamicModelController::GetType(void) const {
    if (D_0009F670 == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F670 = cType::InitializeType(0, 0, 0xE2, D_0009F64C,
                                           (cBase *(*)(cMemPool *, cBase *))&gcDynamicModelController::New, 0, 0, 0);
    }
    return D_0009F670;
}
