// gcStationaryController — methods from gcAll_psp.obj.

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

class cTimeValue {
public:
    int mTime;
};

class gcEntityController {
public:
    char _pad[0x80];
    gcEntityController(cBase *);
    ~gcEntityController();
    void HandleNextAnimation(cTimeValue);
    void Write(cFile &) const;
};

class gcStationaryController : public gcEntityController {
public:
    gcStationaryController(cBase *);
    ~gcStationaryController();
    void Move(cTimeValue);
    void Write(cFile &) const;
    static gcStationaryController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcStationaryControllervirtualtable[];

extern "C" void gcEntityController_gcEntityController(void *, cBase *);

// ── Move (0x00158504) ──
void gcStationaryController::Move(cTimeValue t) {
    HandleNextAnimation(t);
}

// ── Write (0x001583fc) ──
void gcStationaryController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcEntityController::Write(file);
    wb.End();
}

// ── ~gcStationaryController (0x00320984) ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcEntityController(this, 0), and the deleting-tail
// dispatch through operator delete (which inlines the pool/block lookup).
gcStationaryController::~gcStationaryController() {
    *(void **)((char *)this + 4) = gcStationaryControllervirtualtable;
}

// ── New (0x003207dc) ──
gcStationaryController *gcStationaryController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcStationaryController *result = 0;
    gcStationaryController *obj = (gcStationaryController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController_gcEntityController(obj, parent);
        *(void **)((char *)obj + 4) = gcStationaryControllervirtualtable;
        result = obj;
    }
    return result;
}
