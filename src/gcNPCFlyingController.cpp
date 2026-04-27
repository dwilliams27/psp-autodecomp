// gcNPCFlyingController — methods from gcAll_psp.obj.
//
// gcNPCFlyingController extends gcFlyingController. Write forwards to its
// gcFlyingController base via static dispatch (cFactory/gcFlyingController
// pattern). New() and the destructor follow the SNC ABI patterns documented
// in eConfigBase.cpp / eSphereShape_dtor.cpp: pool slot lookup at +0x28
// (alloc) / +0x30 (delete), and operator delete generates the deleting tail.

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
};

class gcCreatureController : public gcEntityController {
public:
    void Write(cFile &) const;
};

class gcFlyingController : public gcCreatureController {
public:
    gcFlyingController(cBase *);
    ~gcFlyingController();
    void Write(cFile &) const;
};

class gcNPCFlyingController : public gcFlyingController {
public:
    gcNPCFlyingController(cBase *);
    ~gcNPCFlyingController();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    }
};

extern char gcNPCFlyingControllervirtualtable[];

extern "C" {
    void gcNPCFlyingController__gcNPCFlyingController_cBaseptr(void *self, cBase *parent);
}

// ── gcNPCFlyingController::Write(cFile &) const @ 0x00155334 ──
void gcNPCFlyingController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcFlyingController::Write(file);
    wb.End();
}

// ── gcNPCFlyingController::New(cMemPool *, cBase *) static @ 0x0031d9ec ──
cBase *gcNPCFlyingController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcNPCFlyingController *result = 0;
    gcNPCFlyingController *obj = (gcNPCFlyingController *)rec->fn(base, 0xB0, 0x10, 0, 0);
    if (obj != 0) {
        gcNPCFlyingController__gcNPCFlyingController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcNPCFlyingController::~gcNPCFlyingController(void) @ 0x0031dc00 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcFlyingController (which is inlined: it just resets
// the vtable, then chains to ~gcEntityController at 0x00110958), and the
// deleting tail dispatch through operator delete. After dead-store elim,
// only the gcFlyingController vtable set survives in the emitted asm
// (the gcNPCFlyingController vtable store is dead-stored away).
gcNPCFlyingController::~gcNPCFlyingController() {
    *(void **)((char *)this + 4) = gcNPCFlyingControllervirtualtable;
}
