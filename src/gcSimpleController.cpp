// gcSimpleController.cpp — decompiled from gcAll_psp.obj
// gcSimpleController inherits from gcEntityController. Vtable at 0x38D260.
// All three methods follow the canonical pattern shared with eSoundDataMap
// (Write/dtor/New): same allocation slot layout, same delete record, and the
// canonical C++ destructor body that lets SNC's ABI auto-generate the
// (this != 0) guard, the base-chain call to ~gcEntityController(), and the
// deleting-tail dispatch via operator delete.

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

extern "C" void gcEntityController__gcEntityController_cBaseptr(void *, cBase *);

class gcEntityController {
public:
    char _pad[0x90];
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
};

class gcSimpleController : public gcEntityController {
public:
    gcSimpleController(cBase *);
    ~gcSimpleController();
    void Write(cFile &) const;
    static gcSimpleController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcSimpleControllervirtualtable[];

// ── gcSimpleController::Write(cFile &) const @ 0x00157f64 ──
void gcSimpleController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcEntityController::Write(file);
    wb.End();
}

// ── gcSimpleController::~gcSimpleController(void) @ 0x0032032c ──
gcSimpleController::~gcSimpleController() {
    *(void **)((char *)this + 4) = gcSimpleControllervirtualtable;
}

// ── gcSimpleController::New(cMemPool *, cBase *) static @ 0x00320184 ──
gcSimpleController *gcSimpleController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcSimpleController *result = 0;
    gcSimpleController *obj = (gcSimpleController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController__gcEntityController_cBaseptr(obj, parent);
        *(void **)((char *)obj + 4) = gcSimpleControllervirtualtable;
        result = obj;
    }
    return result;
}
