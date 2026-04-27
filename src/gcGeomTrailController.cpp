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

// gcSubGeomController is the base — its size determines the derived size
// (gcGeomTrailController adds no members, so total class size is 0x24).
class gcSubGeomController {
public:
    char _pad[0x24];
    gcSubGeomController(cBase *);
    ~gcSubGeomController();
    void Write(cFile &) const;
};

extern char gcGeomTrailControllerclassdesc[];

class gcGeomTrailController : public gcSubGeomController {
public:
    gcGeomTrailController(cBase *);
    ~gcGeomTrailController();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

// ── gcGeomTrailController::Write(cFile &) const @ 0x00154774 ──
void gcGeomTrailController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcSubGeomController *)this)->Write(file);
    wb.End();
}

// ── gcGeomTrailController::~gcGeomTrailController(void) @ 0x001548b0 ──
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain to ~gcSubGeomController, and the deleting-tail dispatch through
// `operator delete` (defined in-class above).
gcGeomTrailController::~gcGeomTrailController() {
    ((void **)this)[1] = gcGeomTrailControllerclassdesc;
}

// ── gcGeomTrailController::New(cMemPool *, cBase *) static @ 0x0031c8e0 ──
extern "C" void gcGeomTrailController__gcGeomTrailController_cBaseptr(void *self, cBase *parent);

cBase *gcGeomTrailController::New(cMemPool *pool, cBase *parent) {
    gcGeomTrailController *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcGeomTrailController *obj = (gcGeomTrailController *)rec->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcGeomTrailController__gcGeomTrailController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
