#include "cBase.h"

class cFile;

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
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

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
