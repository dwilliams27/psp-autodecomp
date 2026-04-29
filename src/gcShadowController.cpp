#include "cBase.h"

class cFile;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRecord {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool_shim {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

// gcSubGeomController is the single base of gcShadowController.
// We only need the methods we chain to; size only matters for total
// derived size (0x24 from New).
class gcSubGeomController {
public:
    char _pad[0x24];
    gcSubGeomController(cBase *);
    ~gcSubGeomController();
    void Write(cFile &) const;
};

extern char gcShadowControllerclassdesc[];

class gcShadowController : public gcSubGeomController {
public:
    gcShadowController(cBase *);
    ~gcShadowController();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F7B8;

// ── gcShadowController::Write(cFile &) const @ 0x00157a08 ──
void gcShadowController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcSubGeomController *)this)->Write(file);
    wb.End();
}

// ── gcShadowController::~gcShadowController(void) @ 0x00157b44 ──
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain to ~gcSubGeomController, and the deleting-tail dispatch through
// `operator delete` (defined in-class above).
gcShadowController::~gcShadowController() {
    ((void **)this)[1] = gcShadowControllerclassdesc;
}

// ── gcShadowController::New(cMemPool *, cBase *) static @ 0x0031f8c0 ──
extern "C" void gcShadowController__gcShadowController_cBaseptr(void *self, cBase *parent);

cBase *gcShadowController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcShadowController *result = 0;
    gcShadowController *obj = (gcShadowController *)rec->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcShadowController__gcShadowController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcShadowController::AssignCopy(const cBase *) @ 0x0031f83c ──
void gcShadowController::AssignCopy(const cBase *base) {
    gcShadowController *other = dcast<gcShadowController>(base);

    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(int *)((char *)other + 12);
    *(float *)((char *)this + 16) = *(float *)((char *)other + 16);
    *(float *)((char *)this + 20) = *(float *)((char *)other + 20);
    *(float *)((char *)this + 24) = *(float *)((char *)other + 24);

    int i = 0;
    short *dst = (short *)((char *)this + 28);
    short *src = (short *)((char *)other + 28);
    do {
        short x = src[0];
        short y = src[1];
        short z = src[2];
        dst[0] = x;
        dst[1] = y;
        dst[2] = z;
        i++;
        dst += 3;
        src += 3;
    } while (i <= 0);
}

// ── gcShadowController::GetType(void) const @ 0x0031f93c ──
const cType *gcShadowController::GetType(void) const {
    if (D_0009F7B8 == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7B8 = cType::InitializeType(0, 0, 0x1EB, D_0009F64C,
                                           &gcShadowController::New, 0, 0, 0);
    }
    return D_0009F7B8;
}
