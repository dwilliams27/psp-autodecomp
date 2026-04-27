// gcProjectileController.cpp — decompiled from gcAll_psp.obj
// gcProjectileController inherits from gcEntityController. Vtable at 0x38CED8.

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
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

extern "C" void gcEntityController_gcEntityController(void *, cBase *);

class gcEntityController {
public:
    char _pad[0x90];
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
};

class gcProjectileController : public gcEntityController {
public:
    gcProjectileController(cBase *);
    ~gcProjectileController();
    void Write(cFile &) const;
    static gcProjectileController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcProjectileControllervirtualtable[];

// ── gcProjectileController::Write(cFile &) const @ 0x00155ca0 ──
void gcProjectileController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcEntityController::Write(file);
    wb.End();
}

// ── gcProjectileController::~gcProjectileController(void) @ 0x0031f168 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the ~gcEntityController() base-chain call (with flags=0), and the
// deleting-tail `if (flags & 1) operator delete(this)` epilogue.
gcProjectileController::~gcProjectileController() {
    *(void **)((char *)this + 4) = gcProjectileControllervirtualtable;
}

// ── gcProjectileController::New(cMemPool *, cBase *) static @ 0x0031efc0 ──
gcProjectileController *gcProjectileController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcProjectileController *result = 0;
    gcProjectileController *obj = (gcProjectileController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController_gcEntityController(obj, parent);
        *(void **)((char *)obj + 4) = gcProjectileControllervirtualtable;
        result = obj;
    }
    return result;
}
